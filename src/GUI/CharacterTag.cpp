#include "GUI/CharacterTag.h"
#include "Common/ResourceManager.h"

CharacterTag::CharacterTag(CharacterName name, Rectangle rec) 
    : name(name), rec(rec), type(CharacterType::SMALL) {
}

CharacterTag::~CharacterTag() {
    
}

void CharacterTag::update() {
    if (IsKeyPressed(left)) {
        name = (CharacterName)((int)(name) - 1);
    }
    else if (IsKeyPressed(right)) {
        name = (CharacterName)((int)(name) + 1);
    }
    else if (IsKeyPressed(up)) {
        type = (CharacterType)((int)(type) + 1);
    }
    else if (IsKeyPressed(down)) {
        type = (CharacterType)((int)(type) - 1);
    }
    if(name > CharacterName::LUIGI) {
        name = CharacterName::MARIO;
    }
    else if(name < CharacterName::MARIO) {
        name = CharacterName::LUIGI;
    }
    if (type > CharacterType::FLOWER) {
        type = CharacterType::SMALL;
    }
    else if(type < CharacterType::SMALL) {
        type = CharacterType::FLOWER;
    }
}

void CharacterTag::draw() {
    //
    DrawRectangleRec(rec, DARKGRAY);
    std::string characterType;
    switch(type) {
        case CharacterType::SMALL:
            characterType = "Small";
            break;
        case CharacterType::SUPER:
            characterType = "Super";
            break;
        case CharacterType::FLOWER:
            characterType = "Flower";
            break;
    }
    std::string characterName;
    switch(name) {
        case CharacterName::MARIO:
            characterName = "Mario";
            break;
        case CharacterName::LUIGI:
            characterName = "Luigi";
            break;
    }
    Texture2D texture = ResourceManager::getTexture()[characterType + characterName + "Tag"];
    Texture2D nameTexture = ResourceManager::getTexture()[characterName + "NameTag"];
    DrawTexture(texture, rec.x + rec.width / 2.0f - texture.width / 2.0f, rec.y + rec.height - 50 - texture.height, WHITE);
    DrawTexture(nameTexture, rec.x + rec.width / 2.0f - nameTexture.width / 2.0f, rec.y + 50 - nameTexture.height/2.0f, WHITE);
}

void CharacterTag::setKeys(KeyboardKey left, KeyboardKey right, KeyboardKey up, KeyboardKey down) {
    this->left = left;
    this->right = right;
    this->up = up;
    this->down = down;
}

CharacterName CharacterTag::getName() const {
    return name;
}