#include "GUI/CharacterTag.h"
#include "Common/ResourceManager.h"

CharacterTag::CharacterTag(CharacterName name, Rectangle rec) 
    : name(name), rec(rec), type(CharacterType::SMALL), state(TagState::NOTSELECTED), modeWorld(ModeWorld::SINGLEPLAYER) {
}

CharacterTag::~CharacterTag() {
    
}

void CharacterTag::update() {
    if(state == TagState::FIRSTPLAYERSELECTING){
        if(modeWorld == ModeWorld::SINGLEPLAYER) {
            if(IsKeyPressed(KEY_UP)) {
                updateState(1);
            } else if(IsKeyPressed(KEY_DOWN)) {
                updateState(0);
            }
        } else {
            if(IsKeyPressed(KEY_W)) {
                updateState(1);
            } else if(IsKeyPressed(KEY_S)) {
                updateState(0);
            }
        }
    }
    else if(state == TagState::SECONDPLAYERSELECTING) {
        if(modeWorld == ModeWorld::MULTIPLAYER) {
            if(IsKeyPressed(KEY_UP)) {
                updateState(1);
            } else if(IsKeyPressed(KEY_DOWN)) {
                updateState(0);
            }
        }
    }
    else if(state == TagState::BOTHPLAYERSELECTING) {
        if(IsKeyPressed(KEY_UP)) {
            updateState(1);
        } else if(IsKeyPressed(KEY_DOWN)) {
            updateState(0);
        }
        if(IsKeyPressed(KEY_W)) {
            updateState(1);
        } else if(IsKeyPressed(KEY_S)) {
            updateState(0);
        }
    }
}

void CharacterTag::draw() {
    std::unordered_map<std::string, Texture2D>& textures = ResourceManager::getTexture();
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
        case CharacterName::TOAD:
            characterName = "Toad";
            break;
        case CharacterName::PEACH:
            characterName = "Peach";
            break;
    }
    Texture2D texture = textures[characterType + characterName + "Tag"];
    Texture2D nameTexture = textures["Gui"+ characterName];
    if(state == TagState::NOTSELECTED){
        DrawTextureEx(texture, {rec.x + rec.width / 2 - texture.width, rec.y + rec.height - 10 - nameTexture.height - 10 - texture.height*2}, 0, 2, Fade(WHITE, 0.3f));
        DrawTexture(nameTexture, rec.x + rec.width / 2 - nameTexture.width/2, rec.y + rec.height - nameTexture.height - 10, Fade(WHITE, 0.3f));
    }
    else {
        DrawTextureEx(texture, {rec.x + rec.width / 2 - texture.width, rec.y + rec.height - 10 - nameTexture.height - 10 - texture.height*2}, 0, 2, WHITE);
        DrawTexture(nameTexture, rec.x + rec.width / 2 - nameTexture.width/2, rec.y + rec.height - nameTexture.height - 10, WHITE);
    }
    switch (state) {
        case TagState::FIRSTPLAYERSELECTING:
            DrawTextureEx(textures["GuiFirstPlayerSelecting"], {rec.x + rec.width / 2 - textures["GuiFirstPlayerSelecting"].width, rec.y - textures["GuiFirstPlayerSelecting"].height*2}, 0, 2, WHITE);
            break;
        case TagState::SECONDPLAYERSELECTING:
            DrawTextureEx(textures["GuiSecondPlayerSelecting"], {rec.x + rec.width / 2 - textures["GuiSecondPlayerSelecting"].width, rec.y - textures["GuiSecondPlayerSelecting"].height*2}, 0, 2, WHITE);
            break;
        case TagState::BOTHPLAYERSELECTING:
            DrawTextureEx(textures["GuiBothPlayerSelecting"], {rec.x + rec.width / 2 - textures["GuiBothPlayerSelecting"].width, rec.y - textures["GuiBothPlayerSelecting"].height*2}, 0, 2, WHITE);
            break;
        case TagState::FIRSTPLAYERSELECTED:
            DrawTextureEx(textures["GuiFirstPlayerSelected"], {rec.x + rec.width / 2 - textures["GuiFirstPlayerSelected"].width, rec.y - textures["GuiFirstPlayerSelected"].height*2}, 0, 2, WHITE);
            break;
        case TagState::SECONDPLAYERSELECTED:
            DrawTextureEx(textures["GuiSecondPlayerSelected"], {rec.x + rec.width / 2 - textures["GuiSecondPlayerSelected"].width, rec.y - textures["GuiSecondPlayerSelected"].height*2}, 0, 2, WHITE);
            break;
        case TagState::FIRSTPLAYERSELECTED_SECONDPLAYERSELECTING:
            DrawTextureEx(textures["GuiFirstPlayerSelectedSecondPlayerSelecting"], {rec.x + rec.width / 2 - textures["GuiFirstPlayerSelectedSecondPlayerSelecting"].width, rec.y - textures["GuiFirstPlayerSelectedSecondPlayerSelecting"].height*2}, 0, 2, WHITE);
            break;
        case TagState::SECONDPLAYERSECLECTED_FIRSTPLAYERSELECTING:
            DrawTextureEx(textures["GuiSecondPlayerSelectedFirstPlayerSelecting"], {rec.x + rec.width / 2 - textures["GuiSecondPlayerSelectedFirstPlayerSelecting"].width, rec.y - textures["GuiSecondPlayerSelectedFirstPlayerSelecting"].height*2}, 0, 2, WHITE);
            break;
        case TagState::BOTHPLAYERSELECTED:
            DrawTextureEx(textures["GuiBothPlayerSelected"], {rec.x + rec.width / 2 - textures["GuiBothPlayerSelected"].width, rec.y - textures["GuiBothPlayerSelected"].height*2}, 0, 2, WHITE);
            break;
        default:
            break;
    }
}

void CharacterTag::addSelect(TagState state) {
    if(this->state == TagState::NOTSELECTED) {
        this->state = state;
    } else if(this->state == TagState::FIRSTPLAYERSELECTING && state == TagState::SECONDPLAYERSELECTING) {
        this->state = TagState::BOTHPLAYERSELECTING;
    } else if(this->state == TagState::SECONDPLAYERSELECTING && state == TagState::FIRSTPLAYERSELECTING) {
        this->state = TagState::BOTHPLAYERSELECTING;
    } else if(this->state == TagState::FIRSTPLAYERSELECTED && state == TagState::SECONDPLAYERSELECTING) {
        this->state = TagState::FIRSTPLAYERSELECTED_SECONDPLAYERSELECTING;
    } else if(this->state == TagState::SECONDPLAYERSELECTED && state == TagState::FIRSTPLAYERSELECTING) {
        this->state = TagState::SECONDPLAYERSECLECTED_FIRSTPLAYERSELECTING;
    }
}

void CharacterTag::removeSelect(TagState state) {
    if(this->state == state) {
        this->state = TagState::NOTSELECTED;
    } else if(this->state == TagState::BOTHPLAYERSELECTING && state == TagState::FIRSTPLAYERSELECTING) {
        this->state = TagState::SECONDPLAYERSELECTING;
    } else if(this->state == TagState::BOTHPLAYERSELECTING && state == TagState::SECONDPLAYERSELECTING) {
        this->state = TagState::FIRSTPLAYERSELECTING;
    } else if(this->state == TagState::FIRSTPLAYERSELECTED_SECONDPLAYERSELECTING && state == TagState::SECONDPLAYERSELECTING) {
        this->state = TagState::FIRSTPLAYERSELECTED;
    } else if(this->state == TagState::SECONDPLAYERSECLECTED_FIRSTPLAYERSELECTING && state == TagState::FIRSTPLAYERSELECTING) {
        this->state = TagState::SECONDPLAYERSELECTED;
    }
}

void CharacterTag::updateState(int isUp) {
    if (isUp) {
        type = (CharacterType)((int)(type) + 1);
    }
    else {
        type = (CharacterType)((int)(type) - 1);
    }
    if (type > CharacterType::FLOWER) {
        type = CharacterType::SMALL;
    }
    else if(type < CharacterType::SMALL) {
        type = CharacterType::FLOWER;
    }
}

CharacterName CharacterTag::getName() const {
    return name;
}

void CharacterTag::setModeWorld(ModeWorld mode) {
    modeWorld = mode;
}

TagState CharacterTag::getState() const {
    return state;
}

void CharacterTag::selected(TagState state) {
    if (this->state == TagState::FIRSTPLAYERSELECTING && state == TagState::FIRSTPLAYERSELECTED) {
        this->state = TagState::FIRSTPLAYERSELECTED;
    } else if (this->state == TagState::SECONDPLAYERSELECTING && state == TagState::SECONDPLAYERSELECTED) {
        this->state = TagState::SECONDPLAYERSELECTED;
    } else if (this->state == TagState::BOTHPLAYERSELECTING && state == TagState::FIRSTPLAYERSELECTED) {
        this->state = TagState::FIRSTPLAYERSELECTED_SECONDPLAYERSELECTING;
    } else if (this->state == TagState::BOTHPLAYERSELECTING && state == TagState::SECONDPLAYERSELECTED) {
        this->state = TagState::SECONDPLAYERSECLECTED_FIRSTPLAYERSELECTING;
    } else if (this->state == TagState::FIRSTPLAYERSELECTED_SECONDPLAYERSELECTING && state == TagState::SECONDPLAYERSELECTED) {
        this->state = TagState::BOTHPLAYERSELECTED;
    } else if (this->state == TagState::SECONDPLAYERSECLECTED_FIRSTPLAYERSELECTING && state == TagState::FIRSTPLAYERSELECTED) {
        this->state = TagState::BOTHPLAYERSELECTED;
    } else if (this->state == TagState::FIRSTPLAYERSELECTED && state == TagState::FIRSTPLAYERSELECTED) {
        this->state = TagState::FIRSTPLAYERSELECTING;
    } else if (this->state == TagState::SECONDPLAYERSELECTED && state == TagState::SECONDPLAYERSELECTED) {
        this->state = TagState::SECONDPLAYERSELECTING;
    } else if (this->state == TagState::BOTHPLAYERSELECTED && state == TagState::FIRSTPLAYERSELECTED) {
        this->state = TagState::SECONDPLAYERSECLECTED_FIRSTPLAYERSELECTING;
    } else if (this->state == TagState::BOTHPLAYERSELECTED && state == TagState::SECONDPLAYERSELECTED) {
        this->state = TagState::FIRSTPLAYERSELECTED_SECONDPLAYERSELECTING;
    } else if (this->state == TagState::FIRSTPLAYERSELECTED_SECONDPLAYERSELECTING && state == TagState::FIRSTPLAYERSELECTED) {
        this->state = TagState::BOTHPLAYERSELECTING;
    } else if (this->state == TagState::SECONDPLAYERSECLECTED_FIRSTPLAYERSELECTING && state == TagState::SECONDPLAYERSELECTED) {
        this->state = TagState::BOTHPLAYERSELECTING;
    }
}