#include "Common/CollisionBox.h"

CollisionBox::CollisionBox() 
    : position({0, 0}), 
    size({5, 5}), 
    rect({0,0,5,5}), 
    color(YELLOW) {}

CollisionBox::CollisionBox(Vector2 position, Vector2 size, Color color)
    : position(position), 
    size(size), 
    rect({position.x, position.y, size.x, size.y}), 
    color(color) {}

bool CollisionBox::checkCollision(const Rectangle& other) const {
    return CheckCollisionRecs(this->rect, other);
}

void CollisionBox::draw(){
    DrawRectangleRec(this->rect, color);
}

void CollisionBox::setPosition(Vector2 position) {
    this->position = position;
    this->rect.x = position.x;
    this->rect.y = position.y;
}

void CollisionBox::setPosition(float x, float y) {
    this->position = {x, y};
    this->rect.x = x;
    this->rect.y = y;
}

void CollisionBox::setX(float x) {
    this->position.x = x;
    this->rect.x = x;
}

void CollisionBox::setY(float y) {
    this->position.y = y;
    this->rect.y = y;
}

void CollisionBox::setSize(Vector2 size) {
    this->size = size;
    this->rect.width = size.x;
    this->rect.height = size.y;
}

void CollisionBox::setSize(float width, float height) {
    this->size = {width, height};
    this->rect.width = width;
    this->rect.height = height;
}

void CollisionBox::setWidth(float width) {
    this->size.x = width;
    this->rect.width = width;
}

void CollisionBox::setHeight(float height) {
    this->size.y = height;
    this->rect.height = height;
}

void CollisionBox::setColor(Color color) {
    this->color = color;
}

Vector2 CollisionBox::getPosition() const {
    return this->position;
}

float CollisionBox::getX() const {
    return this->position.x;
}

float CollisionBox::getY() const {
    return this->position.y;
}

Vector2 CollisionBox::getSize() const {
    return this->size;
}

float CollisionBox::getWidth() const {
    return this->size.x;
}

float CollisionBox::getHeight() const {
    return this->size.y;
}

Color CollisionBox::getColor() const {
    return this->color;
}

Rectangle CollisionBox::getRect() const {
    return this->rect;
}