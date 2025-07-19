#include "Character/Luigi.h"

Luigi::Luigi(ModePlayer mode, Vector2 pos, Vector2 vel, Color color, float speedX, float maxSpeedX, float jumpSpeed) :
    Character(NamePlayer::LUIGI, mode, pos, {32, 44}, vel, color, speedX, maxSpeedX, jumpSpeed) {
}

Luigi::~Luigi() {
    
}

void Luigi::updateCollisionBoxes() {
    north.setWidth(size.x/4);
    north.setX( position.x + size.x / 2 - north.getWidth() / 2 );
    if(isDucking){
        if ( type == CharacterType::SMALL ) {
            north.setY( position.y + 16 );
        } else {
            north.setY( position.y + 30 );
        }
    }
    else{
        north.setY( position.y );
    }
    south.setWidth(size.x/4);
    south.setX( position.x + size.x / 2 - south.getWidth() / 2 );
    south.setY( position.y + size.y - south.getHeight() );
    east.setHeight(size.y/1.5f);
    east.setX( position.x + size.x - east.getWidth() );
    west.setHeight(size.y/1.5f);
    west.setX( position.x );
    if(isDucking){
        east.setHeight(size.y/3);
        west.setHeight(size.y/3);
    }
    if ( type == CharacterType::SMALL ) {
        if ( isDucking ) {
            east.setY( position.y + 26 - east.getHeight() / 2 );
            west.setY( position.y + 26 - west.getHeight() / 2 );
        } else {
            east.setY( position.y + size.y/2 - east.getHeight() / 2 );
            west.setY( position.y + size.y/2- west.getHeight() / 2 );
        }
    } 
    else {
        if ( isDucking ) {
            east.setY( position.y + 40 - east.getHeight() / 2 );
            west.setY( position.y + 40 - west.getHeight() / 2 );
        } else {
            east.setY( position.y + size.y/2 - east.getHeight() / 2 );
            west.setY( position.y + size.y/2 - west.getHeight() / 2 );
        }
    }
}

void Luigi::transitionToSmall() {
    type = CharacterType::SMALL;
    size = {32, 44};
    maxFrame = 2;
    invulnerable = true;
    invulnerableAcum = 0.0f;
}

void Luigi::transitionToSuper() {
    type = CharacterType::SUPER;
    size = {32, 60};
    maxFrame = 3;
}

void Luigi::transitionToFlower() {
    type = CharacterType::FLOWER;
    size = {32, 60};
    maxFrame = 3;
}

json Luigi::saveToJson() const {
    json j = Character::saveToJson();
    j["type"] = "Luigi";
    return j;
}

void Luigi::loadFromJson(const json& j) {
    Character::loadFromJson(j);
}