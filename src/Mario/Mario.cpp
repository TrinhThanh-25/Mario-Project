#include "Mario/Mario.h"
#include "Common/ResourceManager.h"
#include "Game/World.h"
#include <string>

Mario::Mario(Vector2 pos, Vector2 dim, Vector2 vel, Color color, float speedX, float maxSpeedX, float jumpSpeed) :
    Sprite(pos, dim, vel, color, 0, 2, Direction::RIGHT),
    speed(speedX), 
    maxSpeed(maxSpeedX), 
    jumpSpeed(jumpSpeed), 
    dyingSpeed(-600),  
    isRunning(false),
    isDucking(false),
    isLookingUp(false),
    lives(5),
    frameTimeWalking(0.1f),
    frameTimeRunning(0.05f),
    walkingBeforeRunningTime(0.5f),
    walkingBeforeRunningAcum(0.0f),
    drawRunning(false),
    invulnerable(false),
    invulnerableTime(2.0f),
    invulnerableAcum(0.0f),
    invulnerableBlink(false),
    invincible(false),
    invincibleTime(8.0f), 
    invincibleAcum(0.0f),
    transitionTime(0.06f),
    transitionAcum(0.0f),
    normalTransitionSteps(11),
    superToFlowerTransitionSteps(8),
    transitionCurrentFrame(0),
    transitionCurrentIndex(0),
    oldPosition(pos),
    type(MarioType::SMALL) {
    setState(SpriteState::ON_GROUND);
}

Mario::~Mario() {
    fireball.clear();
}

void Mario::update() {
    float deltaTime = GetFrameTime();
    if(state == SpriteState::DYING || velocity.x!=0) {
        frameAcum += deltaTime;
    }
    else {
        curFrame = 0;
    }
    if(state == SpriteState::DYING) {
        position.y += dyingSpeed * deltaTime;
        dyingSpeed += World::gravity * deltaTime;
    }
    if( state != SpriteState::DYING && state!= SpriteState::VICTORY && state!=SpriteState::TO_NEXT_MAP) {
        if(!transition(deltaTime)) {
            movement(deltaTime);
        }
    }
}

void Mario::draw() {
    std::string marioType;
    std::string direct;

    if(type == MarioType::SMALL) {
        marioType = "SmallMario";
    } else if(type == MarioType::SUPER) {
        marioType = "SuperMario";
    } else if(type == MarioType::FLOWER) {
        marioType = "FlowerMario";
    }

    if(direction == Direction::RIGHT) {
        direct = "Right";
    } else {
        direct = "Left";
    }

    if(state == SpriteState::DYING) {
        DrawTexture(ResourceManager::getTexture()[marioType + "Dying" + std::to_string(curFrame)], position.x, position.y, WHITE);
    }
    else if(state == SpriteState::SMALL_TO_SUPER||state==SpriteState::SUPER_TO_SMALL) {
        DrawTexture(ResourceManager::getTexture()["TransitioningSuperMario" + std::to_string(curFrame) + direct], position.x, position.y, WHITE);
    }
    else if(state == SpriteState::SMALL_TO_FLOWER || state == SpriteState::FLOWER_TO_SMALL) {
        DrawTexture(ResourceManager::getTexture()["TransitioningFlowerMario" + std::to_string(curFrame) + direct], position.x, position.y, WHITE);
    }
    else if(state == SpriteState::SUPER_TO_FLOWER) {
        if(transitionCurrentFrame==0) {
            DrawTexture(ResourceManager::getTexture()["SuperMario0" + direction], position.x, position.y, WHITE);
        }
        else {
            DrawTexture(ResourceManager::getTexture()["FlowrMario0" + direction], position.x, position.y, WHITE);
        }
    }
    else {
        if(!invulnerableBlink) {
            if(state==SpriteState::ON_GROUND) {
                if(isDucking) {
                    DrawTexture(ResourceManager::getTexture()[marioType + "Ducking0" + direct], position.x, position.y, WHITE);
                }
                else if(drawRunning) {
                    DrawTexture(ResourceManager::getTexture()[marioType + "Running" + std::to_string(curFrame) + direct], position.x, position.y, WHITE);
                }
                else{
                    if(IsKeyPressed(KEY_LEFT_CONTROL&&type==MarioType::FLOWER)) {
                        DrawTexture(ResourceManager::getTexture()[marioType +"ThrowingFireball0" + direct], position.x, position.y, WHITE);
                    }
                    else{
                        DrawTexture(ResourceManager::getTexture()[marioType + "0" + direct], position.x, position.y, WHITE);
                    }
                }

            }
            else if(state==SpriteState::JUMPING) {
                if(drawRunning) {
                    DrawTexture(ResourceManager::getTexture()[marioType + "JumpingAndRunning0" + direct], position.x, position.y, WHITE);
                }
                else {
                    DrawTexture(ResourceManager::getTexture()[marioType + "Jumping0" + direct], position.x, position.y, WHITE);
                }
            }
            else if(state==SpriteState::FALLING) {
                DrawTexture(ResourceManager::getTexture()[marioType + "Falling0" + direct], position.x, position.y, WHITE);
            }
            else if(state==SpriteState::VICTORY|| state==SpriteState::TO_NEXT_MAP) {
                DrawTexture(ResourceManager::getTexture()[marioType + "Victory0"], position.x, position.y, WHITE);
            }
        }
    }
    for (auto& fb : fireball) {
        fb.draw();
    }
}

void Mario::updateCollisionBoxes() {


}

bool Mario::transition(float deltaTime) {
    const int* currentFrame = nullptr;
    int transitionSteps = 0;
    if(state==SpriteState::SMALL_TO_SUPER || state==SpriteState::SMALL_TO_FLOWER) {
        currentFrame = normalTransitionFrame;
        transitionSteps = normalTransitionSteps;
    }
    else if(state==SpriteState::SUPER_TO_FLOWER) {
        currentFrame = superToFlowerTransitionFrame;
        transitionSteps = superToFlowerTransitionSteps;
    }
    else if(state==SpriteState::SUPER_TO_SMALL || state==SpriteState::FLOWER_TO_SMALL) {
        currentFrame = reverseTransitionFrame;
        transitionSteps = normalTransitionSteps;
    }
    else return false;
    transitionAcum += deltaTime;
    if(transitionAcum >= transitionTime) {
        transitionCurrentIndex++;
        transitionAcum = 0.0f;
        if(transitionCurrentIndex >= transitionSteps) {
            transitionCurrentIndex = 0;
            if(state == SpriteState::SMALL_TO_SUPER) {
                transitionToSuper();
            } else if(state == SpriteState::SUPER_TO_FLOWER || state == SpriteState::SMALL_TO_FLOWER) {
                transitionToFlower();
            } else if(state == SpriteState::SUPER_TO_SMALL || state == SpriteState::FLOWER_TO_SMALL) {
                transitionToSmall();
            }
        }
        else {
            curFrame = currentFrame[transitionCurrentIndex];
        }
        return true;
    }
    return false;
}

void Mario::movement(float deltaTime) {
    const float currentSpeedX = isRunning ? ( drawRunning ? maxSpeed * 1.3f : maxSpeed ) : speed;
    if(IsKeyDown(KEY_LEFT_CONTROL)&&velocity.x!=0) {
        isRunning = true;;
    }
    else {
        isRunning = false;
    }
    if(invulnerable) {
        invulnerableAcum += deltaTime;
        invulnerableBlink = !invulnerableBlink;
        if(invulnerableAcum >= invulnerableTime) {
            invulnerable = false;
            invulnerableAcum = 0.0f;
            invulnerableBlink = false;
        }
    }
    if(invincible) {
        invincibleAcum += deltaTime;
        if(invincibleAcum >= invincibleTime) {
            invincible = false;
            invincibleAcum = 0.0f;
        }
    }
    if(IsKeyDown(KEY_RIGHT)) {
        if (frameAcum >= frameTimeWalking) {
            curFrame = (curFrame + 1) % maxFrame;
            frameAcum = 0.0f;
        }
        walkingAcum += deltaTime;
        direction = Direction::RIGHT;
        if(isRunning) {
            walkingBeforeRunningAcum += deltaTime;
            if(walkingBeforeRunningAcum >= walkingBeforeRunningTime) {
                drawRunning = true;
            }
            if(drawRunning) {
                velocity.x = maxSpeed * 1.3f * (walkingAcum<1.0f ? walkingAcum : 1.0f);
            }
            else {
                velocity.x = maxSpeed * (walkingAcum<1.0f ? walkingAcum : 1.0f);
            }
        }
        else {
            drawRunning = false;
            walkingBeforeRunningAcum = 0.0f;
            velocity.x = speed * (walkingAcum<1.0f ? walkingAcum : 1.0f);
        }
    }
    else if(IsKeyDown(KEY_LEFT)) {
        if (frameAcum >= frameTimeWalking) {
            curFrame = (curFrame + 1) % maxFrame;
            frameAcum = 0.0f;
        }
        walkingAcum += deltaTime;
        direction = Direction::LEFT;
        if(isRunning) {
            walkingBeforeRunningAcum += deltaTime;
            if(walkingBeforeRunningAcum >= walkingBeforeRunningTime) {
                drawRunning = true;
            }
            if(drawRunning) {
                velocity.x = -maxSpeed * 1.3f * (walkingAcum<1.0f ? walkingAcum : 1.0f);
            }
            else {
                velocity.x = -maxSpeed * (walkingAcum<1.0f ? walkingAcum : 1.0f);
            }
        }
        else {
            drawRunning = false;
            walkingBeforeRunningAcum = 0.0f;
            velocity.x = -speed * (walkingAcum<1.0f ? walkingAcum : 1.0f);
        }
    } 
    else {
        walkingAcum = 0.0f;
        if(velocity.x > 0) {
            velocity.x -= speed * deltaTime;
            if(velocity.x < 0) velocity.x = 0;
        } else if(velocity.x < 0) {
            velocity.x += speed * deltaTime;
            if(velocity.x > 0) velocity.x = 0;
        }
    }
    if(state==SpriteState::ON_GROUND) {
        if(IsKeyDown(KEY_DOWN)) {
            isDucking = true;
            velocity.x = 0;
        } else {
            isDucking = false;
        }
    } else {
        isDucking = false;
    }
    if(IsKeyDown(KEY_SPACE)&& state == SpriteState::ON_GROUND) {
        velocity.y = jumpSpeed;
        state = SpriteState::JUMPING;
        PlaySound(ResourceManager::getSound()["Jump"]);
    }
    if(IsKeyPressed(KEY_LEFT_CONTROL) && type==MarioType::FLOWER) {
        if(direction == Direction::RIGHT) {
            fireball.push_back(Fireball({position.x+size.x/2.0f, position.y+size.y/2.0f}, Direction::RIGHT, 2.0f));
        } else {
            fireball.push_back(Fireball({position.x+size.x/2.0f, position.y+size.y/2.0f}, Direction::LEFT, 2.0f));
        }
        PlaySound(ResourceManager::getSound()["Fireball"]);
    }
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
    velocity.y += World::gravity * deltaTime; 
    if(oldPosition.y < position.y) {
        state = SpriteState::ON_GROUND;
    }
    oldPosition = position;
}

CollisionType Mario::checkCollision(Sprite* sprite) const {
    
}

void Mario::collisionTile(Sprite* sprite) const {
    
}

void Mario::collisionBlock(Sprite* sprite) const {
    
}

void Mario::collisionEnemy(Sprite* sprite) const {

}

void Mario::setLives(int lives) {
    this->lives = lives;
}

int Mario::getLives() const {
    return this->lives;
}

void Mario::transitionToSmall() {
    type = MarioType::SMALL;
    size = {32, 40};
    maxFrame = 2;
    transitionCurrentFrame = 0;
    transitionCurrentIndex = 0;
    transitionAcum = 0.0f;
    transitionTime = 0.06f;
    invulnerable = true;
    invulnerableAcum = 0.0f;
    invincible = false;
    invincibleAcum = 0.0f;
}

void Mario::transitionToSuper() {
    type = MarioType::SUPER;
    size = {32, 56};
    maxFrame = 3;
    transitionCurrentFrame = 0;
    transitionCurrentIndex = 0;
    transitionAcum = 0.0f;
    transitionTime = 0.06f;
    invulnerable = false;
    invulnerableAcum = 0.0f;
    invincible = false;
    invincibleAcum = 0.0f;
}

void Mario::transitionToFlower() {
    type = MarioType::FLOWER;
    size = {32, 56};
    maxFrame = 3;
    transitionCurrentFrame = 0;
    transitionCurrentIndex = 0;
    transitionAcum = 0.0f;
    transitionTime = 0.06f;
    invulnerable = false;
    invulnerableAcum = 0.0f;
    invincible = false;
    invincibleAcum = 0.0f;
}

void Mario::setInvulnerable(bool invulnerable) {
    this->invulnerable = invulnerable;
}

bool Mario::isInvulnerable() const {
    return this->invulnerable;
}

void Mario::setInvincible(bool invincible) {
    this->invincible = invincible;
}

bool Mario::isInvincible() const {
    return this->invincible;
}

void Mario::setType(MarioType type) {
    this->type = type;
}

MarioType Mario::getType() const {
    return this->type;
}