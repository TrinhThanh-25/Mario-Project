#include "Character/Character.h"
#include "Common/ResourceManager.h"
#include "Enemy/Enemy.h"
#include "Block/Block.h"
#include "Tile/Tile.h"
#include "Game/World.h"
#include <string>

Character::Character(NamePlayer namePlayer, ModePlayer mode, Vector2 pos, Vector2 dim, Vector2 vel, Color color, float speedX, float maxSpeedX, float jumpSpeed) :
    Sprite(pos, dim, vel, color, 0, 2, Direction::RIGHT),
    namePlayer(namePlayer),
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
    activateWidth(0.0f) {
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

    std::string name;

    switch (namePlayer) {
        case NamePlayer::MARIO:
            name = "Mario";
            break;
        case NamePlayer::LUIGI:
            name = "Luigi";
            break;
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
                else if(drawRunning) {
                    DrawTexture(texture[characterType + "Running" + std::to_string(curFrame) + direct], position.x, position.y, curColor);
                }
                else{
                    if((modePlayer == ModePlayer::FIRSTPLAYER|| *(world->getModeWorld()) == SINGLEPLAYER) && type==CharacterType::FLOWER){
                        if(IsKeyPressed(KEY_LEFT_CONTROL)) {
                            DrawTexture(texture[characterType +"ThrowingFireball0" + direct], position.x, position.y, curColor);
                        }
                        else{
                            DrawTexture(texture[characterType + std::to_string(curFrame) + direct], position.x, position.y, curColor);
                        }
                    }
                    else if(modePlayer== ModePlayer::SECONDPLAYER && type==CharacterType::FLOWER){
                        if(IsKeyPressed(KEY_RIGHT_CONTROL)) {
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
            } else if(state == SpriteState::SUPER_TO_FLOWER || state == SpriteState::SMALL_TO_FLOWER) {
                transitionToFlower();
            } else if(state == SpriteState::SUPER_TO_SMALL || state == SpriteState::FLOWER_TO_SMALL) {
                transitionToSmall();
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

void Character::movement(float deltaTime) {
    KeyboardKey up, down, left, right, control;
    if(modePlayer == ModePlayer::ONEPLAYER) {
        up = KEY_SPACE;
        down = KEY_DOWN;
        left = KEY_LEFT;
        right = KEY_RIGHT;
        control = KEY_LEFT_CONTROL;
    } else if(modePlayer == ModePlayer::FIRSTPLAYER) {
        up = KEY_W;
        down = KEY_S;
        left = KEY_A;
        right = KEY_D;
        control = KEY_LEFT_CONTROL;
    } else if(modePlayer == ModePlayer::SECONDPLAYER) {
        up = KEY_UP;
        down = KEY_DOWN;
        left = KEY_LEFT;
        right = KEY_RIGHT;
        control = KEY_RIGHT_CONTROL;
    }
    float currentSpeedX = isRunning ? ( drawRunning ? maxSpeed * 1.3f : maxSpeed ) : speed;
    float frameTimeAct = isRunning ? frameTimeRunning : frameTimeWalking;
    if(IsKeyDown(control)&&velocity.x!=0) {
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
    if(IsKeyDown(left) || IsKeyDown(right)) {
        if(IsKeyDown(left) && IsKeyDown(right)) {
            walkingAcum = 0.0f;
            if(velocity.x>=-10 && velocity.x<=10) {
                velocity.x = 0;
            }
            else {
                velocity.x *= 0.9f;
            }
        } 
        else if(IsKeyDown(right)) {
            walkingAcum += deltaTime;
            direction = Direction::RIGHT;
            if(isRunning) {
                if(drawRunning) {
                    velocity.x = maxSpeed * 1.3f * (walkingAcum*2<1.0f ? walkingAcum*2 : 1.0f);
                }
                else {
                    velocity.x = maxSpeed * (walkingAcum*2<1.0f ? walkingAcum*2 : 1.0f);
                }
            }
            else {
                velocity.x = speed * (walkingAcum*2<1.0f ? walkingAcum*2 : 1.0f);
            }
        }
        else if(IsKeyDown(left)) {
            walkingAcum += deltaTime;
            direction = Direction::LEFT;
            if(isRunning) {
                if(drawRunning) {
                    velocity.x = -maxSpeed * 1.3f * (walkingAcum*2<1.0f ? walkingAcum*2 : 1.0f);
                }
                else {
                    velocity.x = -maxSpeed * (walkingAcum*2<1.0f ? walkingAcum*2 : 1.0f);
                }
            }
            else {
                velocity.x = -speed * (walkingAcum*2<1.0f ? walkingAcum*2 : 1.0f);
            }
        }
    } 
    else {
        walkingAcum = 0.0f;
        if(velocity.x>=-10 && velocity.x<=10) {
            velocity.x = 0;
        }
        else {
            velocity.x *= 0.9f;
        }
    }
    if(state==SpriteState::ON_GROUND) {
        if(IsKeyDown(down)) {
            isDucking = true;
            velocity.x = 0;
        } else {
            isDucking = false;
        }
    } else {
        isDucking = false;
    }
    if(IsKeyPressed(up) && state == SpriteState::ON_GROUND) {
        velocity.y = jumpSpeed;
        state = SpriteState::JUMPING;
        PlaySound(ResourceManager::getSound()["Jump"]);
    }
    if(IsKeyPressed(control) && type==CharacterType::FLOWER) {
        if(direction == Direction::RIGHT) {
            fireball.push_back(Fireball({position.x + size.x - 4, position.y+size.y - 34}, Direction::RIGHT, 2.0f));
        } else {
            fireball.push_back(Fireball({position.x - 16 + 4, position.y+size.y - 34}, Direction::LEFT, 2.0f));
        }
        PlaySound(ResourceManager::getSound()["Fireball"]);
    }
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
    velocity.y += World::gravity * deltaTime; 
    if(oldPosition.y < position.y) {
        state = SpriteState::FALLING;
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
    if(state == SpriteState::JUMPING || velocity.y > 0) {
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
        if(checkCollision(tile) == CollisionType::SOUTH&& state == SpriteState::JUMPING) {
            position.y = tile->getY() - size.y;
            velocity.y = 0;
            state = SpriteState::ON_GROUND;
            updateCollisionBoxes();
        }
    }
    else if(tile->getType() == SLOPE_DOWN || tile->getType() == SLOPE_UP) {
        //
    }
}

void Character::collisionBlock(Block* block) {
    if(state == SpriteState::DYING || state == SpriteState::VICTORY) return;
    switch(checkCollision(block)) {
        case CollisionType::NORTH:
            position.y = block->getY() + block->getHeight();
            velocity.y = 0;
            updateCollisionBoxes();
            block->doHit(*this, map);
            break;
        case CollisionType::SOUTH:
            position.y = block->getY() - size.y;
            velocity.y = 0;
            state = SpriteState::ON_GROUND;
            updateCollisionBoxes();
            break;
        case CollisionType::WEST:
            position.x = block->getX() + block->getWidth();
            velocity.x = 0;
            updateCollisionBoxes();
            break;
        case CollisionType::EAST:
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
    if(enemy->getState() == SpriteState::IDLE){
        // activate enemy
    }
    if(enemy->getState() != SpriteState::DYING && enemy->getState() != SpriteState::TO_BE_REMOVED) {
        CollisionType collision = checkCollisionEnemy(enemy);
        if(invincible == true && collision != CollisionType::NONE){
            // enemy is defeated
            PlaySound(ResourceManager::getSound()["Stomp"]);
            // character earn points of enemy
        }
        else if (collision == CollisionType::FIREBALL) {
            // enemy is defeated by fireball
            PlaySound(ResourceManager::getSound()["Stomp"]);
            // character earn points of enemy 
        }
        else if( enemy->getAuxiliaryState() != SpriteState::INVULNERABLE && collision == CollisionType::SOUTH ) {
            if( state == SpriteState::FALLING && enemy->getState() != SpriteState::DYING && enemy->getState() != SpriteState::TO_BE_REMOVED) {
                position.y = enemy->getY() - size.y;
                if(((modePlayer == ModePlayer::FIRSTPLAYER || modePlayer == ModePlayer::ONEPLAYER) && IsKeyDown(KEY_LEFT_CONTROL)) || (modePlayer == ModePlayer::SECONDPLAYER && IsKeyDown(KEY_RIGHT_CONTROL))) {
                    velocity.y = -400.0f;
                }
                else {
                    velocity.y = -200.0f;
                }
                state = SpriteState::JUMPING;
                // enemy is defeated
                PlaySound(ResourceManager::getSound()["Stomp"]);
                // character earn points of enemy
            }
        }
        else if( collision != CollisionType::NONE && invulnerable == false ) {
            switch(type) {
                case CharacterType::SMALL:
                    state = SpriteState::DYING;
                    world->playPlayerDownMusic();
                    gameHud->removeLives(1);
                    break;
                case CharacterType::SUPER:
                    PlaySound(ResourceManager::getSound()["Pipe"]);
                    previousState = state;
                    state = SpriteState::SUPER_TO_SMALL;
                    invulnerable = true;
                    break;
                case CharacterType::FLOWER:
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
    }
    velocity = {0, 0};
    state = SpriteState::ON_GROUND;
    direction = Direction::RIGHT;
    isDucking = false;
    isRunning = false;
    invulnerable = false;
    invulnerableAcum = 0.0f;
    invulnerableBlink = false;
    fireball.clear();
}

void Character::resetGame() {
    reset(true);
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

json Character::saveToJson() const {
    json j = Sprite::saveToJson();
    j["namePlayer"] = static_cast<int>(namePlayer);
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
    return j;
}

void Character::loadFromJson(const json& j) {
    Sprite::loadFromJson(j);
    namePlayer = static_cast<NamePlayer>(j["namePlayer"].get<int>());
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
}