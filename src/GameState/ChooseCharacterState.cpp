#include "GameState/ChooseCharacterState.h"
#include "GameState/PlayingState.h"
#include "GameState/TitleScreenState.h"
#include "Character/CharacterFactory.h"
#include "Common/ResourceManager.h"

ChooseCharacterState::ChooseCharacterState(World* world)
    : GameState(world, GameStateType::CHOOSE_CHARACTER),
      modeWorld(ModeWorld::SINGLEPLAYER),
      characterTags({
            new CharacterTag(CharacterName::MARIO, {800-50-150-100-150, 500, 150, 200}),
            new CharacterTag(CharacterName::LUIGI, {800-50-150, 500, 150, 200}),
            new CharacterTag(CharacterName::TOAD, {800 + 50, 500, 150, 200}),
            new CharacterTag(CharacterName::PEACH, {800 + 50 + 150 + 100, 500, 150, 200})
      }) {
}

ChooseCharacterState::~ChooseCharacterState() {
    for (CharacterTag* tag : characterTags) {
        delete tag;
    }
    characterTags.clear();
}

void ChooseCharacterState::update() {
    std::vector<Character*>& characters = world->getCharacters();
    if(IsKeyPressed(KEY_ESCAPE)) {
        world->setGameState(new TitleScreenState(world));
        return;
    }
    for (CharacterTag* tag : characterTags) {
        tag->update();
    }
    if(IsKeyPressed(KEY_ENTER)) {
        for (CharacterTag* tag : characterTags) {
            if (modeWorld == ModeWorld::SINGLEPLAYER && (tag->getState() == TagState::FIRSTPLAYERSELECTING || tag->getState() == TagState::FIRSTPLAYERSELECTED)) {
                characters.push_back(CharacterFactory::createCharacter(tag->getName(), ModePlayer::ONEPLAYER));
            } 
            else if (modeWorld == ModeWorld::MULTIPLAYER && (tag->getState() == TagState::FIRSTPLAYERSELECTING || tag->getState() == TagState::FIRSTPLAYERSELECTED)) {
                characters.push_back(CharacterFactory::createCharacter(tag->getName(), ModePlayer::FIRSTPLAYER));
            } 
            else if (modeWorld == ModeWorld::MULTIPLAYER && (tag->getState() == TagState::SECONDPLAYERSELECTING || tag->getState() == TagState::SECONDPLAYERSELECTED)) {
                characters.push_back(CharacterFactory::createCharacter(tag->getName(), ModePlayer::SECONDPLAYER));
            }
            else if(modeWorld == ModeWorld::MULTIPLAYER && (tag->getState() == TagState::BOTHPLAYERSELECTING || tag->getState() == TagState::BOTHPLAYERSELECTED || tag->getState() == TagState::FIRSTPLAYERSELECTED_SECONDPLAYERSELECTING || tag->getState() == TagState::SECONDPLAYERSECLECTED_FIRSTPLAYERSELECTING)) {
                characters.push_back(CharacterFactory::createCharacter(tag->getName(), ModePlayer::FIRSTPLAYER));
                characters.push_back(CharacterFactory::createCharacter(tag->getName(), ModePlayer::SECONDPLAYER));
            }
            for (Character* character : characters) {
                character->setWorld(world);
            }
        }
        world->getMap()->loadMap(1);
        world->getGameHud()->resetGame();
        world->setModeWorld(modeWorld);
        world->setGameState(new PlayingState(world));
    }
    if(modeWorld == ModeWorld::SINGLEPLAYER) {
        if(characterTags[curP1]->getState() != TagState::FIRSTPLAYERSELECTED && characterTags[curP1]->getState() != TagState::FIRSTPLAYERSELECTED_SECONDPLAYERSELECTING && characterTags[curP1]->getState() != TagState::BOTHPLAYERSELECTED) {
            if(IsKeyPressed(KEY_LEFT)) {
                characterTags[curP1]->removeSelect(TagState::FIRSTPLAYERSELECTING);
                curP1 = (curP1 - 1 + characterTags.size()) % characterTags.size();
                characterTags[curP1]->addSelect(TagState::FIRSTPLAYERSELECTING);
            }
            else if(IsKeyPressed(KEY_RIGHT)) {
                characterTags[curP1]->removeSelect(TagState::FIRSTPLAYERSELECTING);
                curP1 = (curP1 + 1) % characterTags.size();
                characterTags[curP1]->addSelect(TagState::FIRSTPLAYERSELECTING);
            }
        }
    }
    else if(modeWorld == ModeWorld::MULTIPLAYER) {
        if(characterTags[curP1]->getState() != TagState::FIRSTPLAYERSELECTED && characterTags[curP1]->getState() != TagState::FIRSTPLAYERSELECTED_SECONDPLAYERSELECTING && characterTags[curP1]->getState() != TagState::BOTHPLAYERSELECTED) {
            if(IsKeyPressed(KEY_A)) {
                characterTags[curP1]->removeSelect(TagState::FIRSTPLAYERSELECTING);
                curP1 = (curP1 - 1 + characterTags.size()) % characterTags.size();
                characterTags[curP1]->addSelect(TagState::FIRSTPLAYERSELECTING);
            }
            else if(IsKeyPressed(KEY_D)) {
                characterTags[curP1]->removeSelect(TagState::FIRSTPLAYERSELECTING);
                curP1 = (curP1 + 1) % characterTags.size();
                characterTags[curP1]->addSelect(TagState::FIRSTPLAYERSELECTING);
            }
        }
        if(characterTags[curP2]->getState() != TagState::SECONDPLAYERSELECTED && characterTags[curP2]->getState() != TagState::SECONDPLAYERSECLECTED_FIRSTPLAYERSELECTING && characterTags[curP2]->getState() != TagState::BOTHPLAYERSELECTED) {
            if(IsKeyPressed(KEY_LEFT)) {
                characterTags[curP2]->removeSelect(TagState::SECONDPLAYERSELECTING);
                curP2 = (curP2 - 1 + characterTags.size()) % characterTags.size();
                characterTags[curP2]->addSelect(TagState::SECONDPLAYERSELECTING);
            }
            else if(IsKeyPressed(KEY_RIGHT)) {
                characterTags[curP2]->removeSelect(TagState::SECONDPLAYERSELECTING);
                curP2 = (curP2 + 1) % characterTags.size();
                characterTags[curP2]->addSelect(TagState::SECONDPLAYERSELECTING);
            }
        }
    }
    if(IsKeyPressed(KEY_LEFT_CONTROL)) {
        characterTags[curP1]->selected(TagState::FIRSTPLAYERSELECTED);
    }
    if(IsKeyPressed(KEY_RIGHT_CONTROL)) {
        characterTags[curP2]->selected(TagState::SECONDPLAYERSELECTED);
    }
}

void ChooseCharacterState::draw() {
    std::unordered_map<std::string, Texture2D>& texture = ResourceManager::getTexture();
    DrawTexture(texture["ChooseCharacterBackground"], 0, 0, WHITE);
    ResourceManager::drawBigString("Please select your character", GetScreenWidth() / 2 - ResourceManager::getDrawBigStringWidth("Please select your character", 40) / 2.0f, 200, 40);
    for (CharacterTag* tag : characterTags) {
        tag->draw();
    }
}

void ChooseCharacterState::setModeWorld(ModeWorld mode) {
    modeWorld = mode;
    for (CharacterTag* tag : characterTags) {
        tag->setModeWorld(modeWorld);
    }
    if (modeWorld == ModeWorld::SINGLEPLAYER) {
        curP1 = 0;
        curP2 = 1;
        characterTags[curP1]->addSelect(TagState::FIRSTPLAYERSELECTING);
        characterTags[curP2]->removeSelect(TagState::SECONDPLAYERSELECTING);
    } else {
        curP1 = 0;
        curP2 = 1;
        characterTags[curP1]->addSelect(TagState::FIRSTPLAYERSELECTING);
        characterTags[curP2]->addSelect(TagState::SECONDPLAYERSELECTING);
    }
}