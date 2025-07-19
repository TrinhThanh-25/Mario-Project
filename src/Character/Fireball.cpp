#include "Character/Fireball.h"
#include "Common/ResourceManager.h"
#include "Game/World.h"

Fireball::Fireball(Vector2 pos, Direction Direction, float fireTime)
    : Sprite(pos, {16, 16}, {300, 240}, BLUE, 0.1f, 4, Direction), 
    fireTime(fireTime), 
    fireAcum(0.0f) {
    if(Direction == Direction::LEFT) {
        setVelocity(-300, 240);
    } else {
        setVelocity(300, 240);
    }
    setState(SpriteState::ACTIVE);
    updateCollisionBoxes();
}

Fireball::~Fireball() = default;

void Fireball::update() {
    updateFrame();
    updatePosition();
    updateVelocity();
    updateState();
    updateCollisionBoxes();
}

void Fireball::draw() {
    DrawTexture(ResourceManager::getTexture()["Fireball" + std::to_string(curFrame) + (direction == Direction::LEFT ? "Left" : "Right")], position.x, position.y, WHITE);
}

void Fireball::updateFrame() {
    frameAcum += GetFrameTime();
    if (frameAcum >= frameTime) {
        curFrame = curFrame % maxFrame;
        frameAcum = 0.0f;
    }
}

void Fireball::updatePosition() {
    position.x += velocity.x * GetFrameTime();
    position.y += velocity.y * GetFrameTime();
}

void Fireball::updateVelocity() {
    velocity.y += World::gravity * GetFrameTime();
}

void Fireball::updateState() {
    fireAcum += GetFrameTime();
    if (fireAcum >= fireTime) {
        state = SpriteState::TO_BE_REMOVED;
    }
}

json Fireball::saveToJson() const {
    json j = Sprite::saveToJson();
    j["fireAcum"] = fireAcum;
    j["fireTime"] = fireTime;
    return j;
}

void Fireball::loadFromJson(const json& j) {
    Sprite::loadFromJson(j);
    fireAcum = j["fireAcum"];
    fireTime = j["fireTime"];
}