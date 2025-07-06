#include "Enemy/BuzzyBettle.h"
#include <string>
#include "Common/ResourceManager.h"

BuzzyBettle::BuzzyBettle(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Enemy(pos, dim, vel, color) {

    dyingFrameAcum = 0.0f;
    dyingFrameTime = 0.1f;
    maxDyingFrame = 2;      
    currentDyingFrame = 0;

    pointFrameAcum = 0.0f;
    pointFrameTime = 0.5f;

    diePosition = pos;

    shellMoving = false;
    shellSpeed = 200.0f;
    isFacingLeft = true;
}



void BuzzyBettle::draw() {
    std::string textureKey;

    if (state == SpriteState::ACTIVE) {
        textureKey = "BuzzyBettle_0";
    }
    else if (state == SpriteState::SHELL || state == SpriteState::DYING) {
        textureKey = "BuzzyBettle_1";
    }
    else {
        textureKey = "BuzzyBettle_0";
    }

    DrawTexture(ResourceManager::getTexture()[textureKey], position.x, position.y, WHITE);

    if (state == SpriteState::DYING) {
        //  Tính vị trí point bay lên
        float offsetY = 50.0f * pointFrameAcum / pointFrameTime;  // Đi lên tối đa 50 pixel

        // Vẽ points (ví dụ "Point100" là key resource)
        DrawTexture(ResourceManager::getTexture()["Point100"],
                    diePosition.x,
                    diePosition.y - offsetY,
                    WHITE);
    }
}



void BuzzyBettle::update(){
    float delta = GetFrameTime();

    if (state == SpriteState::ACTIVE){
        position.x += velocity.x * delta;
        position.y += velocity.y * delta;
        updateCollisionBoxes();
    }

    else if (state == SpriteState::SHELL){
        if (shellMoving) {
            float dir = isFacingLeft ? -1.0f : 1.0f;
            position.x += shellSpeed * dir * delta;
            updateCollisionBoxes();
        }
    }

    else if (state == SpriteState::DYING){
        dyingFrameAcum += delta;
        if (dyingFrameAcum >= dyingFrameTime){
            dyingFrameAcum = 0.0f;
            currentDyingFrame++;

            if (currentDyingFrame >= maxDyingFrame){
                setState(SpriteState::TO_BE_REMOVED);
            }
        }
    }

    pointFrameAcum += delta;
    if (pointFrameAcum >= pointFrameTime){
        pointFrameAcum = pointFrameTime;
    }
}


void BuzzyBettle::beingHit(){
    if (state == SpriteState::ACTIVE){
        setState(SpriteState::SHELL);
        velocity.x = 0;
        shellMoving = false;
    }
    else if (state == SpriteState::SHELL){
        setState(SpriteState::DYING);
        diePosition = position;
        currentDyingFrame = 0;
        dyingFrameAcum = 0.0f;
        pointFrameAcum = 0.0f;
        shellMoving = false;
    }
}

void BuzzyBettle::kickShell(bool faceLeft) {
    if (state == SpriteState::SHELL) {
        shellMoving = true;
        isFacingLeft = faceLeft;
    }
}
 
bool BuzzyBettle::isShellMoving() const {
    return shellMoving;
}

void BuzzyBettle::collisionSound(){
    // PlaySound(ResourceManager::getSound()["BuzzyHit"]);
}


