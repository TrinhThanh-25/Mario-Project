#include "Character/Character.h"
#include "Common/ResourceManager.h"
#include "Enemy/Enemy.h"
#include "Block/Block.h"
#include "Tile/Tile.h"
#include "Game/World.h"
#include "Item/Item.h"
#include "Item/ItemFactory.h"
#include <string>
#include <algorithm>

Character::Character(CharacterName characterName, ModePlayer mode, Vector2 pos, Vector2 dim, Vector2 vel, Color color, float speedX, float maxSpeedX, float acceleration, float friction, float floatTime, float jumpSpeed, int initialLives) :
    Sprite(pos, dim, vel, color, 0, 2, Direction::RIGHT),
    acceleration(acceleration),
    friction(friction),
    floatTime(floatTime),
    floatTimeAcum(0.0f),
    characterName(characterName),
    modePlayer(mode),
    speed(speedX), 
    maxSpeed(maxSpeedX), 
    jumpSpeed(jumpSpeed), 
    dyingSpeed(-600),  
    isRunning(false),
    isDucking(false),
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
    type(CharacterType::SMALL),
    activateWidth(0.0f),
    powerUpItem(CharacterType::SMALL),
    initialLives(initialLives),
    lives(initialLives),
    creativeMode(false),
    invulnerableMode(false),
    isThrowingFireball(false),
    throwingFireballTime(0.15f),
    throwingFireballAcum(0.0f),
    keyManager(nullptr){
    setState(SpriteState::ON_GROUND);
}

Character::~Character() {
    fireball.clear();
}

void Character::setWorld(World* world) {
    this->world = world;
    this->map = world->getMap();
    this->gameHud = world->getGameHud();
}

void Character::update() {
    float deltaTime = GetFrameTime();
    if(state == SpriteState::DYING || velocity.x != 0) {
        frameAcum += deltaTime;
        if(frameAcum >= frameTimeWalking) {
            curFrame = (curFrame + 1) % maxFrame;
            frameAcum = 0.0f;
        }
    }
    else {
        curFrame = 0;
    }
    if(state == SpriteState::DYING) {
        position.y += dyingSpeed * deltaTime;
        dyingSpeed += World::gravity * deltaTime;
    }
    if( state != SpriteState::DYING && state!= SpriteState::VICTORY) {
        if(!transition(deltaTime)) {
            movement(deltaTime);
        }
        for (auto i=0; i<fireball.size(); i++) {
            fireball[i].update();
        }
    }
    for (int i=0;i<fireball.size();i++) {
        if(fireball[i].getState() == SpriteState::TO_BE_REMOVED) {
            fireball.erase(fireball.begin() + i);
            i--;
        }
    }
    updateCollisionBoxes();
}

void Character::draw() {
    std::unordered_map<std::string, Texture2D>& texture = ResourceManager::getTexture();
    std::string characterType;
    std::string direct;

    if(modePlayer == ModePlayer::SECONDPLAYER) {
        DrawTexture(texture["Gui"+name + "SecondPlayer"], position.x + size.x / 2 - texture["Gui"+name + "SecondPlayer"].width / 2, position.y - texture["Gui"+name + "SecondPlayer"].height - 5, WHITE);
    }
    else {
        DrawTexture(texture["Gui"+name + "FirstPlayer"], position.x + size.x / 2 - texture["Gui"+name + "FirstPlayer"].width / 2, position.y - texture["Gui"+name + "FirstPlayer"].height - 5, WHITE);
    }

    if(type == CharacterType::SMALL) {
        characterType = "Small" + name;
    } else if(type == CharacterType::SUPER) {
        characterType = "Super" + name;
    } else if(type == CharacterType::FLOWER) {
        characterType = "Flower" + name;
    }

    if(direction == Direction::RIGHT) {
        direct = "Right";
    } else {
        direct = "Left";
    }

    if(state == SpriteState::DYING) {
        DrawTexture(texture[characterType + "Dying" + std::to_string(curFrame)], position.x, position.y, WHITE);
    }
    else if(state == SpriteState::SMALL_TO_SUPER||state==SpriteState::SUPER_TO_SMALL) {
        DrawTexture(texture["TransitioningSuper" + name + std::to_string(curFrame) + direct], position.x, position.y, WHITE);
    }
    else if(state == SpriteState::SMALL_TO_FLOWER || state == SpriteState::FLOWER_TO_SMALL) {
        DrawTexture(texture["TransitioningFlower" + name + std::to_string(curFrame) + direct], position.x, position.y, WHITE);
    }
    else if(state == SpriteState::SUPER_TO_FLOWER) {
        if(curFrame==0) {
            DrawTexture(texture["Super" + name + "0" + direct], position.x, position.y, WHITE);
        }
        else {
            DrawTexture(texture["Flower" + name + "0" + direct], position.x, position.y, WHITE);
        }
    }
    else {
        Color curColor = WHITE;
        if(invincible) {
            curColor = ColorFromHSV( 360.0f * (invincibleAcum / invincibleTime * 20), 0.3f, 1.0f);
        }
        if(!invulnerableBlink) {
            if(state==SpriteState::ON_GROUND) {
                if(isDucking) {
                    DrawTexture(texture[characterType + "Ducking0" + direct], position.x, position.y, curColor);
                }
                else if((direction == Direction::LEFT && velocity.x > 0) || (direction == Direction::RIGHT && velocity.x < 0)) {
                    DrawTexture(texture[characterType + "TurningAround0" + direct], position.x, position.y, curColor);
                }
                else if(drawRunning) {
                    DrawTexture(texture[characterType + "Running" + std::to_string(curFrame) + direct], position.x, position.y, curColor);
                }
                else {
                    if(type==CharacterType::FLOWER){
                        if(isThrowingFireball) {
                            DrawTexture(texture[characterType +"ThrowingFireball0" + direct], position.x, position.y, curColor);
                        }
                        else{
                            DrawTexture(texture[characterType + std::to_string(curFrame) + direct], position.x, position.y, curColor);
                        }
                    }
                    else{
                        DrawTexture(texture[characterType + std::to_string(curFrame) + direct], position.x, position.y, curColor);
                    }
                }

            }
            else if(state==SpriteState::JUMPING) {
                if(drawRunning) {
                    DrawTexture(texture[characterType + "JumpingAndRunning0" + direct], position.x, position.y, curColor);
                }
                else {
                    DrawTexture(texture[characterType + "Jumping0" + direct], position.x, position.y, curColor);
                }
            }
            else if(state==SpriteState::FALLING) {
                DrawTexture(texture[characterType + "Falling0" + direct], position.x, position.y, curColor);
            }
            else if(state==SpriteState::VICTORY) {
                DrawTexture(texture[characterType + "Victory0"], position.x, position.y, curColor);
            }
        }
    }
    for (auto& fb : fireball) {
        fb.draw();
    }
}

bool Character::transition(float deltaTime) {
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
        if(transitionCurrentIndex > transitionSteps) {
            transitionCurrentIndex = 0;
            if(state == SpriteState::SMALL_TO_SUPER) {
                transitionToSuper();
                world->resumeWorld();
            } else if(state == SpriteState::SUPER_TO_FLOWER || state == SpriteState::SMALL_TO_FLOWER) {
                transitionToFlower();
                world->resumeWorld();
            } else if(state == SpriteState::SUPER_TO_SMALL || state == SpriteState::FLOWER_TO_SMALL) {
                transitionToSmall();
                world->resumeWorld();
            }
            state = previousState;
        }
        else {
            curFrame = currentFrame[transitionCurrentIndex];
        }
        return true;
    }
    return true;
}

bool Character::isTransitioning() const {
    return state == SpriteState::SMALL_TO_SUPER || state == SpriteState::SMALL_TO_FLOWER ||
           state == SpriteState::SUPER_TO_FLOWER || state == SpriteState::SUPER_TO_SMALL ||
           state == SpriteState::FLOWER_TO_SMALL;
}

void Character::movement(float deltaTime) {
    if (state != SpriteState::DYING && position.y + size.y >= map->getHeight()) {
        if(world->getGameMode() == GameMode::TESTER && creativeMode) {
            position.y = map->getHeight() - size.y;
            velocity.y = 0;
        } else {   
            state = SpriteState::DYING;
            if(world->getGameMode() == GameMode::TESTER) {
                map->reset(true);
                reset(true);
            } else {
                world->playPlayerDownMusic();
                removeLives(1);
            }
            return;
        }
    }
    
    float currentSpeedX = isRunning ? ( drawRunning ? maxSpeed * 1.3f : maxSpeed ) : speed;
    float frameTimeAct = isRunning ? frameTimeRunning : frameTimeWalking;
    
    if(IsKeyDown(getKeys()["SHIFT"])&&velocity.x!=0) {
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
    if(isRunning){
        walkingBeforeRunningAcum +=deltaTime;
        if(walkingBeforeRunningAcum >= walkingBeforeRunningTime) {
            drawRunning = true;
        }
    }
    else {
        drawRunning = false;
        walkingBeforeRunningAcum = 0.0f;
    }
    if(velocity.x!=0) {
        if (frameAcum >= frameTimeAct) {
            curFrame = (curFrame + 1) % maxFrame;
            frameAcum = 0.0f;
        }
    }
    if(world->getGameMode() == GameMode::TESTER && creativeMode) {
        float creativeSpeed = 260.0f;
        float creativeFastSpeed = 400.0f;
        float currentCreativeSpeed = isRunning ? creativeFastSpeed : creativeSpeed;
        
        velocity.x = 0;
        velocity.y = 0;
        
        if(IsKeyDown(getKeys()["LEFT"]) && IsKeyDown(getKeys()["RIGHT"])) {
            
        } else if(IsKeyDown(getKeys()["RIGHT"])) {
            velocity.x = currentCreativeSpeed;
            direction = Direction::RIGHT;
        } else if(IsKeyDown(getKeys()["LEFT"])) {
            velocity.x = -currentCreativeSpeed;
            direction = Direction::LEFT;
        }
        
        if(IsKeyDown(getKeys()["UP"]) && IsKeyDown(getKeys()["DOWN"])) {
            
        } else if(IsKeyDown(getKeys()["UP"])) {
            velocity.y = -currentCreativeSpeed;
        } else if(IsKeyDown(getKeys()["DOWN"])) {
            velocity.y = currentCreativeSpeed;
        }
        
        if(velocity.x != 0 || velocity.y != 0) {
            if (frameAcum >= frameTimeWalking) {
                curFrame = (curFrame + 1) % maxFrame;
                frameAcum = 0.0f;
            }
        } else {
            curFrame = 0;
        }
        
        position.x += velocity.x * deltaTime;
        position.y += velocity.y * deltaTime;
        
        if(position.x < 0) position.x = 0;
        if(position.y < 0) position.y = 0;
        if(position.x + size.x > map->getWidth()) position.x = map->getWidth() - size.x;
        if(position.y > map->getHeight() + 200) position.y = map->getHeight() + 200;
        
        if(velocity.y < 0) {
            state = SpriteState::JUMPING;
        } else if(velocity.y > 0) {
            state = SpriteState::FALLING;
        } else {
            state = SpriteState::ON_GROUND;
        }
    }
    else {
        if(IsKeyDown(getKeys()["LEFT"]) || IsKeyDown(getKeys()["RIGHT"])) {
            if(IsKeyDown(getKeys()["LEFT"]) && IsKeyDown(getKeys()["RIGHT"])) {
                if(velocity.x > 0) {
                    velocity.x = std::max(0.0f, velocity.x - friction * deltaTime);
                } else if(velocity.x < 0) {
                    velocity.x = std::min(0.0f, velocity.x + friction * deltaTime);
                }
            } 
            else if(IsKeyDown(getKeys()["RIGHT"])) {
                direction = Direction::RIGHT;
                float targetSpeed = isRunning ? (drawRunning ? maxSpeed * 1.3f : maxSpeed) : speed;
                
                if(velocity.x < targetSpeed) {
                    velocity.x = std::min(targetSpeed, velocity.x + acceleration * deltaTime);
                }
            }
            else if(IsKeyDown(getKeys()["LEFT"])) {
                direction = Direction::LEFT;
                float targetSpeed = isRunning ? (drawRunning ? -maxSpeed * 1.3f : -maxSpeed) : -speed;
                
                if(velocity.x > targetSpeed) {
                    velocity.x = std::max(targetSpeed, velocity.x - acceleration * deltaTime);
                }
            }
        } 
        else {
            if(std::abs(velocity.x) <= 10.0f) {
                velocity.x = 0;
            } else {
                if(velocity.x > 0) {
                    velocity.x = std::max(0.0f, velocity.x - friction * deltaTime);
                } else {
                    velocity.x = std::min(0.0f, velocity.x + friction * deltaTime);
                }
            }
        }
        if(state==SpriteState::ON_GROUND) {
            if(IsKeyDown(getKeys()["DOWN"])) {
                isDucking = true;
                velocity.x = 0;
            } else {
                isDucking = false;
            }
        } else {
            isDucking = false;
        }
        if(IsKeyPressed(getKeys()["UP"]) && state == SpriteState::ON_GROUND) {
            velocity.y = jumpSpeed;
            state = SpriteState::JUMPING;
            floatTimeAcum = 0.0f;
            PlaySound(ResourceManager::getSound()["Jump"]);
        }
        if(state == SpriteState::JUMPING) {
            bool isNearPeak = (velocity.y > -50.0f && velocity.y <= 0.0f);
            if(isNearPeak && floatTimeAcum < floatTime && IsKeyDown(getKeys()["UP"])) {
                velocity.y += World::gravity * deltaTime * 0.1f;
                floatTimeAcum += deltaTime;
            } else {
                velocity.y += World::gravity * deltaTime;
                if(!isNearPeak) {
                    floatTimeAcum = 0.0f;
                }
            }
        }
    }
    if(IsKeyPressed(getKeys()["CONTROL"]) && type==CharacterType::FLOWER) {
        if(direction == Direction::RIGHT) {
            fireball.push_back(Fireball({position.x + size.x - 4, position.y+size.y - 34}, Direction::RIGHT, 2.0f));
        } else {
            fireball.push_back(Fireball({position.x - 16 + 4, position.y+size.y - 34}, Direction::LEFT, 2.0f));
        }
        PlaySound(ResourceManager::getSound()["Fireball"]);
    }
    
    if(!(world->getGameMode() == GameMode::TESTER && creativeMode)) {
        position.x += velocity.x * deltaTime;
        position.y += velocity.y * deltaTime;
        
        if(state != SpriteState::JUMPING) {
            velocity.y += World::gravity * deltaTime; 
        }
        
        if(oldPosition.y < position.y) {
            state = SpriteState::FALLING;
        }
    }
    oldPosition = position;
}

CollisionType Character::checkCollision(Sprite* sprite) {
    if(sprite->getState() == SpriteState::NO_COLLIDABLE) return CollisionType::NONE;
    Rectangle rect = sprite->getRect();
    for (auto& fb : fireball){
        switch(fb.checkCollision(sprite)) {
            case CollisionType::NORTH:
                fb.setVelocityY(-fb.getVelocityY());
                break;
            case CollisionType::SOUTH:
                fb.setVelocityY(-320);
                break;
            case CollisionType::WEST:
                fb.setState(SpriteState::TO_BE_REMOVED);
                break;
            case CollisionType::EAST:
                fb.setState(SpriteState::TO_BE_REMOVED);
                break;
            default:
                break;
        }
    }
    if(north.checkCollision(rect)) {
        return CollisionType::NORTH;
    }
    else if(south.checkCollision(rect)){
        return CollisionType::SOUTH;
    } 
    else if(west.checkCollision(rect)) {
        return CollisionType::WEST;
    }
    else if(east.checkCollision(rect)) {
        return CollisionType::EAST;
    }
    return CollisionType::NONE;
}

CollisionType Character::checkCollisionEnemy(Sprite* sprite) {
    if(sprite->getState() == SpriteState::NO_COLLIDABLE || sprite->getState() == SpriteState::DYING || sprite->getState() == SpriteState::TO_BE_REMOVED) return CollisionType::NONE;
    Rectangle rect = sprite->getRect();
    for (auto& fb : fireball){
        if(fb.checkCollision(sprite) != CollisionType::NONE && sprite->getState() != SpriteState::DYING) {
            fb.setState(SpriteState::TO_BE_REMOVED);
            return CollisionType::FIREBALL;
        }
    }
    if(north.checkCollision(rect)) {
        return CollisionType::NORTH;
    }
    else if(south.checkCollision(rect)) {
        return CollisionType::SOUTH;
    }
    else if(west.checkCollision(rect)) {
        return CollisionType::WEST;
    }
    else if(east.checkCollision(rect)) {
        return CollisionType::EAST;
    }
    return CollisionType::NONE;
}

void Character::collisionTile(Tile* tile) {
    if(state == SpriteState::DYING || state == SpriteState::VICTORY) return;
    if(tile->getType() == TileType::SOLID){
        switch(checkCollision(tile)) {
            case CollisionType::NORTH:
                position.y = tile->getY() + tile->getHeight();
                velocity.y = 0;
                updateCollisionBoxes();
                break;
            case CollisionType::SOUTH:
                position.y = tile->getY() - size.y;
                velocity.y = 0;
                state = SpriteState::ON_GROUND;
                updateCollisionBoxes();
                break;
            case CollisionType::WEST:
                position.x = tile->getX() + tile->getWidth();
                velocity.x = 0;
                updateCollisionBoxes();
                break;
            case CollisionType::EAST:
                position.x = tile->getX() - size.x;
                velocity.x = 0;
                updateCollisionBoxes();
                break;
            default:
                break;
        }
    }
    else if(tile->getType() == TileType::SOLID_ABOVE){
        if(checkCollision(tile) == CollisionType::SOUTH && velocity.y >= 0) {
            position.y = tile->getY() - size.y;
            velocity.y = 0;
            state = SpriteState::ON_GROUND;
            updateCollisionBoxes();
        }
    }
    else if(tile->getType() == SLOPE_DOWN || tile->getType() == SLOPE_UP) {
        //
    }
    // checkpoint
    // dây leo
    // tile chạm vào gây chết
    // tile có thể leo lên
    // lò xo nhúng
    // BLOCK có thể làm bể
}

void Character::collisionBlock(Block* block) {
    if(state == SpriteState::DYING || state == SpriteState::VICTORY || block->getState() == SpriteState::NO_COLLIDABLE) return;
    switch(checkCollision(block)) {
        case CollisionType::NORTH:
            if(block->getState() == SpriteState::SOLID_ABOVE || (block->getState() == SpriteState::INVISIBLE && velocity.y >= 0)) {
                break;
            }
            position.y = block->getY() + block->getHeight();
            velocity.y = 0;
            updateCollisionBoxes();
            block->doHit(*this, map);
            break;
        case CollisionType::SOUTH:
            if(block->getState() == SpriteState::INVISIBLE || (block->getState() == SpriteState::SOLID_ABOVE && velocity.y <= 0)) {
                return;
            }
            position.y = block->getY() - size.y;
            velocity.y = 0;
            state = SpriteState::ON_GROUND;
            updateCollisionBoxes();
            break;
        case CollisionType::WEST:
            if(block->getState() == SpriteState::INVISIBLE || block->getState() == SpriteState::SOLID_ABOVE) {
                return;
            }
            position.x = block->getX() + block->getWidth();
            velocity.x = 0;
            updateCollisionBoxes();
            break;
        case CollisionType::EAST:
            if(block->getState() == SpriteState::INVISIBLE || block->getState() == SpriteState::SOLID_ABOVE) {
                return;
            }
            position.x = block->getX() - size.x;
            velocity.x = 0;
            updateCollisionBoxes();
            break;
        default:
            break;
    }
}

void Character::collisionEnemy(Enemy* enemy) {
    if(state == SpriteState::DYING || state == SpriteState::VICTORY) return;
    if(enemy->getState() != SpriteState::DYING && enemy->getState() != SpriteState::TO_BE_REMOVED) {
        CollisionType collision = checkCollisionEnemy(enemy);
        if(invincible == true && collision != CollisionType::NONE){
            enemy->beingHit(HitType::STOMP);
            PlaySound(ResourceManager::getSound()["Stomp"]);
            gameHud->addPoints(enemy->getPoint());
        }
        else if (collision == CollisionType::FIREBALL) {
            enemy->beingHit(HitType::FIREBALL);
            PlaySound(ResourceManager::getSound()["Stomp"]);
            gameHud->addPoints(enemy->getPoint());
        }
        else if(collision == CollisionType::SOUTH && enemy->getAuxiliaryState() != SpriteState::INVULNERABLE) {
            if( state == SpriteState::FALLING && enemy->getState() != SpriteState::DYING && enemy->getState() != SpriteState::TO_BE_REMOVED) {
                position.y = enemy->getY() - size.y;
                if(IsKeyDown(getKeys()["CONTROL"])) {
                    velocity.y = -400.0f;
                }
                else {
                    velocity.y = -200.0f;
                }
                state = SpriteState::JUMPING;
                enemy->beingHit(HitType::STOMP);
                enemy->setIsFacingLeft(direction == Direction::LEFT);
                PlaySound(ResourceManager::getSound()["Stomp"]);
                gameHud->addPoints(enemy->getPoint());
            }
        }
        else if(( collision == CollisionType::EAST || collision == CollisionType::WEST) && enemy->getState() == SpriteState::SHELL) {
            switch (collision) {
                case CollisionType::EAST:
                    position.x = enemy->getX() - size.x;
                    enemy->beingHit(HitType::STOMP);
                    enemy->setIsFacingLeft(false);
                    velocity.x = 0;
                    break;
                case CollisionType::WEST:
                    position.x = enemy->getX() + enemy->getSize().x;
                    enemy->beingHit(HitType::STOMP);
                    enemy->setDirection(direction);
                    enemy->setIsFacingLeft(true);
                    velocity.x = 0;
                    break;
                default:
                    break;
            }
        }
        else if( collision != CollisionType::NONE && invulnerable == false ) {
            switch(type) {
                case CharacterType::SMALL:
                    if(world->getGameMode() == GameMode::TESTER && invulnerableMode) {
                        break;
                    }
                    state = SpriteState::DYING;
                    if(world->getGameMode() == GameMode::TESTER) {
                        map->reset(true);
                        reset(true);
                    } else {
                        world->playPlayerDownMusic();
                        removeLives(1);
                    }
                    break;
                case CharacterType::SUPER:
                    if(world->getGameMode() == GameMode::TESTER && invulnerableMode) {
                        break;
                    }
                    PlaySound(ResourceManager::getSound()["Pipe"]);
                    previousState = state;
                    state = SpriteState::SUPER_TO_SMALL;
                    invulnerable = true;
                    break;
                case CharacterType::FLOWER:
                    if(world->getGameMode() == GameMode::TESTER && invulnerableMode) {
                        break;
                    }
                    PlaySound(ResourceManager::getSound()["Pipe"]);
                    previousState = state;
                    state = SpriteState::FLOWER_TO_SMALL;
                    invulnerable = true;
                    break;
            }
        }
    }
}

void Character::setInvulnerable(bool invulnerable) {
    this->invulnerable = invulnerable;
}

bool Character::isInvulnerable() const {
    return this->invulnerable;
}

void Character::setInvincible(bool invincible) {
    this->invincible = invincible;
}

bool Character::isInvincible() const {
    return this->invincible;
}

void Character::setActivateWidth(float width) {
    this->activateWidth = width;
}

float Character::getActivateWidth() const {
    return this->activateWidth;
}

void Character::setType(CharacterType type) {
    this->type = type;
    if(type == CharacterType::SMALL) {
        transitionToSmall();
    } else if(type == CharacterType::SUPER) {
        transitionToSuper();
    } else if(type == CharacterType::FLOWER) {
        transitionToFlower();
    }
}

CharacterType Character::getType() const {
    return this->type;
}

void Character::setPreviousState(SpriteState state) {
    this->previousState = state;
}

SpriteState Character::getPreviousState() const {
    return this->previousState;
}

void Character::reset(bool isPowerOff) {
    if(isPowerOff) {
        transitionToSmall();
        powerUpItem = CharacterType::SMALL;
    }
    velocity = {0, 0};
    dyingSpeed = -600;
    state = SpriteState::ON_GROUND;
    direction = Direction::RIGHT;
    isDucking = false;
    isRunning = false;
    invulnerable = false;
    invulnerableAcum = 0.0f;
    invulnerableBlink = false;
    floatTimeAcum = 0.0f;
    isThrowingFireball = false;
    throwingFireballAcum = 0.0f;
    fireball.clear();
}

GameHud *Character::getGameHud() const
{
    return gameHud;
}

Map *Character::getMap() const
{
    return map;
}

World *Character::getWorld() const
{
    return world;
}

void Character::drawGameHud() const {
    std::unordered_map<std::string, Texture2D>& textures = ResourceManager::getTexture();\
    if(modePlayer == ModePlayer::ONEPLAYER) {
        DrawTexture( textures["Gui" + name], 34, 32, WHITE );
        DrawTexture( textures["GuiX"], 54, 49, WHITE );
        ResourceManager::drawWhiteSmallNumber( lives < 0 ? 0 : lives, 68, 49 );

        if ( powerUpItem == CharacterType::SUPER ) {
            DrawTexture( textures["Mushroom"], GetScreenWidth() / 2 - textures["Mushroom"].width / 2, 32, WHITE );
        } else if ( powerUpItem == CharacterType::FLOWER ) {
            DrawTexture( textures["FireFlower0"], GetScreenWidth() / 2 - textures["FireFlower0"].width / 2, 32, WHITE );
        }
        DrawTexture( textures["GuiNextItem" + name], GetScreenWidth() / 2 - textures["GuiNextItem" + name].width / 2, 20, WHITE );
    }
    else if (modePlayer == ModePlayer::FIRSTPLAYER) {
        if ( powerUpItem == CharacterType::SUPER ) {
            DrawTexture( textures["Mushroom"], GetScreenWidth() / 2 - textures["Mushroom"].width / 2 - 100, 32, WHITE );
        } else if ( powerUpItem == CharacterType::FLOWER ) {
            DrawTexture( textures["FireFlower0"], GetScreenWidth() / 2 - textures["FireFlower0"].width / 2 - 100, 32, WHITE );
        }
        DrawTexture( textures["GuiNextItem" + name], GetScreenWidth() / 2 - textures["GuiNextItem" + name].width / 2 - 100, 20, WHITE );
        
        DrawTexture( textures["Gui" + name], GetScreenWidth() / 2 - textures["Mushroom"].width / 2 - 100 - 80 - 50, 32, WHITE );
        DrawTexture( textures["GuiX"], GetScreenWidth() / 2 - textures["Mushroom"].width / 2 - 100 - 80 - 50 + 20, 49, WHITE );
        ResourceManager::drawWhiteSmallNumber( lives < 0 ? 0 : lives, GetScreenWidth() / 2 - textures["Mushroom"].width / 2 - 100 - 80 - 50 + 34, 49 );
    }
    else if (modePlayer == ModePlayer::SECONDPLAYER) {
        if ( powerUpItem == CharacterType::SUPER ) {
            DrawTexture( textures["Mushroom"], GetScreenWidth() / 2 - textures["Mushroom"].width / 2 + 100, 32, WHITE );
        } else if ( powerUpItem == CharacterType::FLOWER ) {
            DrawTexture( textures["FireFlower0"], GetScreenWidth() / 2 - textures["FireFlower0"].width / 2 + 100, 32, WHITE );
        }
        DrawTexture( textures["GuiNextItem" + name], GetScreenWidth() / 2 - textures["GuiNextItem" + name].width / 2 + 100, 20, WHITE );
        
        DrawTexture( textures["Gui" + name], GetScreenWidth() / 2 - textures["Mushroom"].width / 2 + 100 + 56 + 50, 32, WHITE );
        DrawTexture( textures["GuiX"], GetScreenWidth() / 2 - textures["Mushroom"].width / 2 + 100 + 56 + 50 + 20, 49, WHITE );
        ResourceManager::drawWhiteSmallNumber( lives < 0 ? 0 : lives, GetScreenWidth() / 2 - textures["Mushroom"].width / 2 + 100 + 56 + 50 + 34, 49 );
    }
}

void Character::setLives(int lives) {
    if(lives < 0) {
        this->lives = 0;
    } else {
        this->lives = lives;
    }
}

int Character::getLives() const {
    return this->lives;
}

void Character::addLives(int lives) {
    this->lives += lives;
}

void Character::removeLives(int lives) {
    this->lives -= lives;
    if(this->lives < 0) {
        this->lives = 0;
    }
}

void Character::setPowerUpItem(CharacterType type) {
    this->powerUpItem = type;
}

CharacterType Character::getPowerUpItem() const {
    return this->powerUpItem;
}

void Character::releasePowerUpItem() {
    Item* item = nullptr;
    Vector2 position;
    switch (modePlayer) {
        case ModePlayer::ONEPLAYER:
            if(powerUpItem == CharacterType::SUPER) {
                position = GetScreenToWorld2D({(float) GetScreenWidth() / 2 - ResourceManager::getTexture()["Mushroom"].width / 2, 32}, *world->getCamera());
            } else if(powerUpItem == CharacterType::FLOWER) {
                position = GetScreenToWorld2D({(float) GetScreenWidth() / 2 - ResourceManager::getTexture()["FireFlower0"].width / 2, 32}, *world->getCamera());
            }
            break;
        case ModePlayer::FIRSTPLAYER:
            if(powerUpItem == CharacterType::SUPER) {
                position = GetScreenToWorld2D({(float) GetScreenWidth() / 2 - ResourceManager::getTexture()["Mushroom"].width / 2 - 100, 32}, *world->getCamera());
            } else if(powerUpItem == CharacterType::FLOWER) {
                position = GetScreenToWorld2D({(float) GetScreenWidth() / 2 - ResourceManager::getTexture()["FireFlower0"].width / 2 - 100, 32}, *world->getCamera());
            }
            break;
        case ModePlayer::SECONDPLAYER:
            if(powerUpItem == CharacterType::SUPER) {
                position = GetScreenToWorld2D({(float) GetScreenWidth() / 2 - ResourceManager::getTexture()["Mushroom"].width / 2 + 100, 32}, *world->getCamera());
            } else if(powerUpItem == CharacterType::FLOWER) {
                position = GetScreenToWorld2D({(float) GetScreenWidth() / 2 - ResourceManager::getTexture()["FireFlower0"].width / 2 + 100, 32}, *world->getCamera());
            }
            break;
    }

    if(powerUpItem == CharacterType::SUPER) {
        item = ItemFactory::createItem(ItemType::MUSHROOM, Source::INVENTORY, position, Direction::RIGHT);
    } else if(powerUpItem == CharacterType::FLOWER) {
        item = ItemFactory::createItem(ItemType::FLOWER, Source::INVENTORY, position, Direction::RIGHT);
    }
    powerUpItem = CharacterType::SMALL;   

    if(item) {
        item->setState(SpriteState::ACTIVE);
        item->setDirection(Direction::RIGHT);
        map->getItem().push_back(item);
        PlaySound(ResourceManager::getSound()["ReleasePowerUpItem"]);
    }
}

void Character::setInitialLives(int lives) {
    if(lives < 0) {
        this->lives = 0;
    } else {
        this->lives = lives;
    }
}

json Character::saveToJson() const {
    json j = Sprite::saveToJson();
    j["characterName"] = static_cast<int>(characterName);
    j["modePlayer"] = static_cast<int>(modePlayer);
    j["speed"] = speed;
    j["maxSpeed"] = maxSpeed;
    j["jumpSpeed"] = jumpSpeed;
    j["dyingSpeed"] = dyingSpeed;
    j["isRunning"] = isRunning;
    j["isDucking"] = isDucking;
    j["frameTimeWalking"] = frameTimeWalking;
    j["walkingAcum"] = walkingAcum;
    j["frameTimeRunning"] = frameTimeRunning;
    j["walkingBeforeRunningTime"] = walkingBeforeRunningTime;
    j["walkingBeforeRunningAcum"] = walkingBeforeRunningAcum;
    j["drawRunning"] = drawRunning;
    j["invulnerable"] = invulnerable;
    j["invulnerableTime"] = invulnerableTime;
    j["invulnerableAcum"] = invulnerableAcum;
    j["invulnerableBlink"] = invulnerableBlink;
    j["invincible"] = invincible;
    j["invincibleTime"] = invincibleTime;
    j["invincibleAcum"] = invincibleAcum;
    j["transitionTime"] = transitionTime;
    j["transitionAcum"] = transitionAcum;
    j["normalTransitionSteps"] = normalTransitionSteps;
    j["superToFlowerTransitionSteps"] = superToFlowerTransitionSteps;
    j["transitionCurrentFrame"] = transitionCurrentFrame;
    j["transitionCurrentIndex"] = transitionCurrentIndex;
    j["oldPosition"] = {oldPosition.x, oldPosition.y};
    j["type"] = static_cast<int>(type);
    j["fireball"] = json::array();
    for (const auto& fb : fireball) {
        j["fireball"].push_back(fb.saveToJson());
    }
    j["previousState"] = static_cast<int>(previousState);
    j["lives"] = lives;
    j["powerUpItem"] = static_cast<int>(powerUpItem);
    j["initialLives"] = initialLives;
    j["acceleration"] = acceleration;
    j["friction"] = friction;
    j["floatTime"] = floatTime;
    j["floatTimeAcum"] = floatTimeAcum;
    j["isThrowingFireball"] = isThrowingFireball;
    j["throwingFireballTime"] = throwingFireballTime;
    j["throwingFireballAcum"] = throwingFireballAcum;
    return j;
}

void Character::loadFromJson(const json& j) {
    Sprite::loadFromJson(j);
    characterName = static_cast<CharacterName>(j["characterName"].get<int>());
    modePlayer = static_cast<ModePlayer>(j["modePlayer"].get<int>());
    speed = j["speed"].get<float>();
    maxSpeed = j["maxSpeed"].get<float>();
    jumpSpeed = j["jumpSpeed"].get<float>();
    dyingSpeed = j["dyingSpeed"].get<float>();
    isRunning = j["isRunning"].get<bool>();
    isDucking = j["isDucking"].get<bool>();
    frameTimeWalking = j["frameTimeWalking"].get<float>();
    walkingAcum = j["walkingAcum"].get<float>();
    frameTimeRunning = j["frameTimeRunning"].get<float>();
    walkingBeforeRunningTime = j["walkingBeforeRunningTime"].get<float>();
    walkingBeforeRunningAcum = j["walkingBeforeRunningAcum"].get<float>();
    drawRunning = j["drawRunning"].get<bool>();
    invulnerable = j["invulnerable"].get<bool>();
    invulnerableTime = j["invulnerableTime"].get<float>();
    invulnerableAcum = j["invulnerableAcum"].get<float>();
    invulnerableBlink = j["invulnerableBlink"].get<bool>();
    invincible = j["invincible"].get<bool>();
    invincibleTime = j["invincibleTime"].get<float>();
    invincibleAcum = j["invincibleAcum"].get<float>();
    transitionTime = j["transitionTime"].get<float>();
    transitionAcum = j["transitionAcum"].get<float>();
    normalTransitionSteps = j["normalTransitionSteps"].get<int>();
    superToFlowerTransitionSteps = j["superToFlowerTransitionSteps"].get<int>();
    transitionCurrentFrame = j["transitionCurrentFrame"].get<int>();
    transitionCurrentIndex = j["transitionCurrentIndex"].get<int>();
    oldPosition.x = j["oldPosition"][0].get<float>();
    oldPosition.y = j["oldPosition"][1].get<float>();
    type = static_cast<CharacterType>(j["type"].get<int>());
    fireball.clear();
    for (const auto& fb : j.at("fireball")) {
        Fireball fireballInstance({fb["position"][0].get<float>(), fb["position"][1].get<float>()}, 
                                    static_cast<Direction>(fb["direction"].get<int>()), 
                                    fb["fireTime"].get<float>());
        fireballInstance.loadFromJson(fb);
        fireball.push_back(fireballInstance);
    }
    previousState = static_cast<SpriteState>(j["previousState"].get<int>());
    lives = j["lives"].get<int>();
    powerUpItem = static_cast<CharacterType>(j["powerUpItem"].get<int>());
    initialLives = j["initialLives"].get<int>();
    acceleration = j["acceleration"].get<float>();
    friction = j["friction"].get<float>();
    floatTime = j["floatTime"].get<float>();
    floatTimeAcum = j["floatTimeAcum"].get<float>();
    isThrowingFireball = j["isThrowingFireball"].get<bool>();
    throwingFireballTime = j["throwingFireballTime"].get<float>();
    throwingFireballAcum = j["throwingFireballAcum"].get<float>();
}

void Character::setCreativeMode(bool creative) {
    this->creativeMode = creative;
}

bool Character::isCreativeMode() const {
    return this->creativeMode;
}

void Character::setInvulnerableMode(bool invulnerable) {
    this->invulnerableMode = invulnerable;
}

bool Character::isInvulnerableMode() const {
    return this->invulnerableMode;
}

void Character::setCharacterName(CharacterName name) {
    this->characterName = name;
}

CharacterName Character::getCharacterName() const {
    return this->characterName;
}

GameMode Character::getGameMode() const {
    return world->getGameMode();
}

ModePlayer Character::getModePlayer() const {
    return modePlayer;
}

void Character::copyState(const Character& other) {
    position = other.position;
    velocity = other.velocity;
    state = other.state;
    auxiliaryState = other.auxiliaryState;
    direction = other.direction;
    frameAcum = other.frameAcum;
    curFrame = other.curFrame;
    world = other.world;
    map = other.map;
    gameHud = other.gameHud;
    modePlayer = other.modePlayer;
    dyingSpeed = other.dyingSpeed;
    isRunning = other.isRunning;
    isDucking = other.isDucking;
    frameTimeWalking = other.frameTimeWalking;
    walkingAcum = other.walkingAcum;
    frameTimeRunning = other.frameTimeRunning;
    walkingBeforeRunningTime = other.walkingBeforeRunningTime;
    walkingBeforeRunningAcum = other.walkingBeforeRunningAcum;
    drawRunning = other.drawRunning;
    invulnerable = other.invulnerable;
    invulnerableTime = other.invulnerableTime;
    invulnerableAcum = other.invulnerableAcum;
    invulnerableBlink = other.invulnerableBlink;
    invincible = other.invincible;
    invincibleTime = other.invincibleTime;
    invincibleAcum = other.invincibleAcum;
    transitionTime = other.transitionTime;
    transitionAcum = other.transitionAcum;
    normalTransitionSteps = other.normalTransitionSteps;
    superToFlowerTransitionSteps = other.superToFlowerTransitionSteps;
    transitionCurrentFrame = other.transitionCurrentFrame;
    transitionCurrentIndex = other.transitionCurrentIndex;
    oldPosition.x = other.oldPosition.x;
    oldPosition.y = other.oldPosition.y;
    type = other.type;
    if(type == CharacterType::FLOWER) {
        transitionToFlower();
    } else if(type == CharacterType::SUPER) {
        transitionToSuper();
    }
    previousState = other.previousState;
    lives = other.lives;
    powerUpItem = other.powerUpItem;
    creativeMode = other.creativeMode;
    invulnerableMode = other.invulnerableMode;
    isThrowingFireball = other.isThrowingFireball;
    throwingFireballTime = other.throwingFireballTime;
    throwingFireballAcum = other.throwingFireballAcum;
}

void Character::jump(float deltaTime) {
    if(world->getGameMode() == GameMode::TESTER && creativeMode) {
        float creativeSpeed = 260.0f;
        float creativeFastSpeed = 400.0f;
        float currentCreativeSpeed = isRunning ? creativeFastSpeed : creativeSpeed;
        velocity.y = -currentCreativeSpeed;
        state = SpriteState::JUMPING;
    } else {
        if (state == SpriteState::ON_GROUND) {
            velocity.y = jumpSpeed;
            state = SpriteState::JUMPING;
            floatTimeAcum = 0.0f;
            PlaySound(ResourceManager::getSound()["Jump"]);
        }
    }
}

void Character::duck(float deltaTime) {
    if(world->getGameMode() == GameMode::TESTER && creativeMode) {
        float creativeSpeed = 260.0f;
        float creativeFastSpeed = 400.0f;
        float currentCreativeSpeed = isRunning ? creativeFastSpeed : creativeSpeed;
        velocity.y = currentCreativeSpeed;
        state = SpriteState::FALLING;
    } else {
        if (state == SpriteState::ON_GROUND) {
            isDucking = true;
            velocity.x = 0;
        }
    }
}

void Character::moveLeft(float deltaTime) {
    if(world->getGameMode() == GameMode::TESTER && creativeMode) {
        float creativeSpeed = 260.0f;
        velocity.x = -creativeSpeed;
        direction = Direction::LEFT;
        isRunning = false;
    } else {
        direction = Direction::LEFT;
        isRunning = false;
        float targetSpeed = -speed;
        
        if (velocity.x > targetSpeed) {
            velocity.x = std::max(targetSpeed, velocity.x - acceleration * deltaTime);
        }
    }
}

void Character::moveRight(float deltaTime) {
    if(world->getGameMode() == GameMode::TESTER && creativeMode) {
        float creativeSpeed = 260.0f;
        velocity.x = creativeSpeed;
        direction = Direction::RIGHT;
        isRunning = false;
    } else {
        direction = Direction::RIGHT;
        isRunning = false;
        float targetSpeed = speed;
        
        if (velocity.x < targetSpeed) {
            velocity.x = std::min(targetSpeed, velocity.x + acceleration * deltaTime);
        }
    }
}

void Character::runFastLeft(float deltaTime) {
    if(world->getGameMode() == GameMode::TESTER && creativeMode) {
        float creativeFastSpeed = 400.0f;
        velocity.x = -creativeFastSpeed;
        direction = Direction::LEFT;
        isRunning = true;
    } else {
        direction = Direction::LEFT;
        isRunning = true;
        float targetSpeed = drawRunning ? -maxSpeed * 1.3f : -maxSpeed;
        
        if (velocity.x > targetSpeed) {
            velocity.x = std::max(targetSpeed, velocity.x - acceleration * deltaTime);
        }
    }
}

void Character::runFastRight(float deltaTime) {
    if(world->getGameMode() == GameMode::TESTER && creativeMode) {
        float creativeFastSpeed = 400.0f;
        velocity.x = creativeFastSpeed;
        direction = Direction::RIGHT;
        isRunning = true;
    } else {
        direction = Direction::RIGHT;
        isRunning = true;
        float targetSpeed = drawRunning ? maxSpeed * 1.3f : maxSpeed;
        
        if (velocity.x < targetSpeed) {
            velocity.x = std::min(targetSpeed, velocity.x + acceleration * deltaTime);
        }
    }
}

void Character::throwFireball(float deltaTime) {
    if (type == CharacterType::FLOWER) {
        isThrowingFireball = true;
        throwingFireballAcum = 0.0f;
        
        if (direction == Direction::RIGHT) {
            fireball.push_back(Fireball({position.x + size.x - 4, position.y + size.y - 34}, Direction::RIGHT, 2.0f));
        } else {
            fireball.push_back(Fireball({position.x - 16 + 4, position.y + size.y - 34}, Direction::LEFT, 2.0f));
        }
        PlaySound(ResourceManager::getSound()["Fireball"]);
    }
}

void Character::setModePlayer(ModePlayer mode) {
    this->modePlayer = mode;
}

void Character::setKeyManager(KeyManager* keyManager) {
    this->keyManager = keyManager;
}

std::unordered_map<std::string, int>& Character::getKeys() {
    if (keyManager) {
        return keyManager->getKeys(modePlayer);
    }
    static std::unordered_map<std::string, int> defaultKeys = {
        {"UP", KEY_SPACE},
        {"DOWN", KEY_DOWN},
        {"LEFT", KEY_LEFT},
        {"RIGHT", KEY_RIGHT},
        {"CONTROL", KEY_LEFT_CONTROL},
        {"SHIFT", KEY_LEFT_SHIFT}
    };
    return defaultKeys;
}