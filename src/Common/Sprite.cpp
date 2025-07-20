#include "Common/Sprite.h"
#include <string>

Sprite::Sprite() 
    : position({0, 0}), 
    size({5, 5}), 
    velocity({0, 0}), 
    color(WHITE),
    state(SpriteState::IDLE), 
    auxiliaryState(SpriteState::NEITHER), 
    direction(Direction::DOWN), 
    frameTime(0.1f), 
    frameAcum(0.0f), 
    curFrame(0), 
    maxFrame(1) {
    updateCollisionBoxes();
}

Sprite::Sprite(Vector2 positon, Vector2 size, Vector2 vel, Color color, float frameTime, int maxFrame, Direction facingDirection)
    : position(positon), 
    size(size), 
    velocity(vel), 
    color(color), 
    state(SpriteState::IDLE), 
    auxiliaryState(SpriteState::NEITHER), 
    direction(facingDirection),
    frameTime(frameTime), 
    frameAcum(0.0f), 
    curFrame(0), 
    maxFrame(maxFrame) {
    north.setColor(color);
    south.setColor(color);
    west.setColor(color);
    east.setColor(color);
}

Sprite::Sprite(Vector2 positon, Vector2 size, Color color)
    : position(positon), 
    size(size), 
    velocity({0, 0}), 
    color(color), 
    direction(Direction::RIGHT),
    frameTime(0.0f), 
    frameAcum(0.0f), 
    curFrame(0), 
    maxFrame(0) {
    north.setColor(color);
    south.setColor(color);
    west.setColor(color);
    east.setColor(color);
}

Sprite::~Sprite() = default;

CollisionType Sprite::checkCollision(Sprite* sprite){
    if (sprite == nullptr) return CollisionType::NONE;
    if (this->north.checkCollision(sprite->getRect())) return CollisionType::NORTH;
    if (this->south.checkCollision(sprite->getRect())) return CollisionType::SOUTH;
    if (this->west.checkCollision(sprite->getRect())) return CollisionType::WEST;
    if (this->east.checkCollision(sprite->getRect())) return CollisionType::EAST;
    return CollisionType::NONE;
}

void Sprite::updateCollisionBoxes() {
    north.setPosition({position.x + size.x/2 - north.getWidth()/2, position.y});
    south.setPosition({position.x + size.x/2 - south.getWidth()/2, position.y + size.y - south.getHeight()});
    west.setPosition({position.x, position.y + size.y/2 - west.getHeight()/2});
    east.setPosition({position.x + size.x - east.getWidth(), position.y + size.y/2 - east.getHeight()/2});
}

void Sprite::setPosition(Vector2 position) {
    this->position = position;
}

void Sprite::setPosition(float x, float y) {
    this->position = {x, y};
}

void Sprite::setX(float x) {
    this->position.x = x;
}

void Sprite::setY(float y) {
    this->position.y = y;
}

void Sprite::setSize(Vector2 size) {
    this->size = size;
}

void Sprite::setSize(float width, float height) {
    this->size = {width, height};
}

void Sprite::setWidth(float width) {
    this->size.x = width;
}

void Sprite::setHeight(float height) {
    this->size.y = height;
}

void Sprite::setVelocity(Vector2 velocity) {
    this->velocity = velocity;
}

void Sprite::setVelocity(float x, float y) {
    this->velocity = {x, y};
}

void Sprite::setVelocityX(float x) {
    this->velocity.x = x;
}

void Sprite::setVelocityY(float y) {
    this->velocity.y = y;
}

void Sprite::setColor(Color color) {
    this->color = color;
}

void Sprite::setState(SpriteState state) {
    this->state = state;
}

void Sprite::setAuxiliaryState(SpriteState auxiliaryState) {
    this->auxiliaryState = auxiliaryState;
}

void Sprite::setDirection(Direction direction) {
    this->direction = direction;
}

Vector2 Sprite::getPosition() const {
    return this->position;
}

float Sprite::getX() const {
    return this->position.x;
}

float Sprite::getY() const {
    return this->position.y;
}

Vector2 Sprite::getSize() const {
    return this->size;
}

float Sprite::getWidth() const {
    return this->size.x;
}

float Sprite::getHeight() const {
    return this->size.y;
}

Vector2 Sprite::getVelocity() const {
    return this->velocity;
}

float Sprite::getVelocityX() const {
    return this->velocity.x;
}

float Sprite::getVelocityY() const {
    return this->velocity.y;
}

Color Sprite::getColor() const {
    return this->color;
}

SpriteState Sprite::getState() const {
    return this->state;
}

SpriteState Sprite::getAuxiliaryState() const {
    return this->auxiliaryState;
}

Direction Sprite::getDirection() const {
    return this->direction;
}

Rectangle Sprite::getRect() const {
    return Rectangle{position.x, position.y, size.x, size.y};
}

json Sprite::saveToJson() const {
    json j;
    j["position"] = {position.x, position.y};
    j["size"] = {size.x, size.y};
    j["velocity"] = {velocity.x, velocity.y};
    j["color"] = {color.r, color.g, color.b, color.a};
    j["state"] = static_cast<int>(state);
    j["auxiliaryState"] = static_cast<int>(auxiliaryState);
    j["direction"] = static_cast<int>(direction);
    j["frameTime"] = frameTime;
    j["frameAcum"] = frameAcum;
    j["curFrame"] = curFrame;
    j["maxFrame"] = maxFrame;
    return j;
}

void Sprite::loadFromJson(const json& j) {
    position.x = j["position"][0].get<float>();
    position.y = j["position"][1].get<float>();
    size.x = j["size"][0].get<float>();
    size.y = j["size"][1].get<float>();
    velocity.x = j["velocity"][0].get<float>();
    velocity.y = j["velocity"][1].get<float>();
    color.r = j["color"][0].get<unsigned char>();
    color.g = j["color"][1].get<unsigned char>();
    color.b = j["color"][2].get<unsigned char>();
    color.a = j["color"][3].get<unsigned char>();
    state = static_cast<SpriteState>(j["state"].get<int>());
    auxiliaryState = static_cast<SpriteState>(j["auxiliaryState"].get<int>());
    direction = static_cast<Direction>(j["direction"].get<int>());
    frameTime = j["frameTime"].get<float>();
    frameAcum = j["frameAcum"].get<float>();
    curFrame = j["curFrame"].get<int>();
    maxFrame = j["maxFrame"].get<int>();
}