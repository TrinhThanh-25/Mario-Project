#include "GameState/ChooseCharacterState.h"
#include "GameState/PlayingState.h"
#include "Character/Mario.h"
#include "Character/Luigi.h"

ChooseCharacterState::ChooseCharacterState(World* world)
    : GameState(world, GameStateType::CHOOSE_CHARACTER),
    modeWorld(ModeWorld::SINGLEPLAYER),
    singlePlayerTag(CharacterName::MARIO, {650, 100, 300, 500}),
    firstPlayerTag(CharacterName::MARIO, {300, 100, 300, 500}),
    secondPlayerTag(CharacterName::LUIGI, {1000, 100, 300, 500}),
    onePlayerButton({350, 700, 200, 50}, "One Player", 20),
    twoPlayersButton({1050, 700, 200, 50}, "Two Players", 20) {
        singlePlayerTag.setKeys(KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN);
        firstPlayerTag.setKeys(KEY_A, KEY_D, KEY_W, KEY_S);
        secondPlayerTag.setKeys(KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN);
        onePlayerButton.Selected();
}

ChooseCharacterState::~ChooseCharacterState() {

}

void ChooseCharacterState::update() {
    onePlayerButton.update();
    twoPlayersButton.update();
    if(onePlayerButton.isPressed()) {
        modeWorld = ModeWorld::SINGLEPLAYER;
        onePlayerButton.Selected();
        twoPlayersButton.deSelected();
    }
    else if(twoPlayersButton.isPressed()) {
        modeWorld = ModeWorld::MULTIPLAYER;
        twoPlayersButton.Selected();
        onePlayerButton.deSelected();
    }
    if(modeWorld == ModeWorld::SINGLEPLAYER) {
        singlePlayerTag.update();
    } 
    else if(modeWorld == ModeWorld::MULTIPLAYER) {
        firstPlayerTag.update();
        secondPlayerTag.update();
    }
    if(IsKeyPressed(KEY_ENTER)) {
        if (modeWorld == ModeWorld::SINGLEPLAYER) {
            switch (singlePlayerTag.getName()) {
                case CharacterName::MARIO:
                    world->getCharacters().push_back(new Mario(ModePlayer::ONEPLAYER, {64, 0}, {0, 0}, RED, 260.0f, 360.0f, -600.0f));
                    world->getCharacters().back()->setWorld(world);
                    break;
                case CharacterName::LUIGI:
                    world->getCharacters().push_back(new Luigi(ModePlayer::ONEPLAYER, {64, 0}, {0, 0}, GREEN, 260.0f, 360.0f, -600.0f));
                    world->getCharacters().back()->setWorld(world);
                    break;
            }
        } else {
            switch (firstPlayerTag.getName()) {
                case CharacterName::MARIO:
                    world->getCharacters().push_back(new Mario(ModePlayer::FIRSTPLAYER, {64, 0}, {0, 0}, RED, 260.0f, 360.0f, -600.0f));
                    world->getCharacters().back()->setWorld(world);
                    break;
                case CharacterName::LUIGI:
                    world->getCharacters().push_back(new Luigi(ModePlayer::FIRSTPLAYER, {64, 0}, {0, 0}, GREEN, 260.0f, 360.0f, -600.0f));
                    world->getCharacters().back()->setWorld(world);
                    break;
            }
            switch (secondPlayerTag.getName()) {
                case CharacterName::MARIO:
                    world->getCharacters().push_back(new Mario(ModePlayer::SECONDPLAYER, {200, 100}, {0, 0}, RED, 260.0f, 360.0f, -600.0f));
                    world->getCharacters().back()->setWorld(world);
                    break;
                case CharacterName::LUIGI:
                    world->getCharacters().push_back(new Luigi(ModePlayer::SECONDPLAYER, {200, 100}, {0, 0}, GREEN, 260.0f, 360.0f, -600.0f));
                    world->getCharacters().back()->setWorld(world);
                    break;
            }
        }
        world->setModeWorld(modeWorld);
        world->setGameState(new PlayingState(world));
    }
}

void ChooseCharacterState::draw() {
    onePlayerButton.draw();
    twoPlayersButton.draw();
    if(modeWorld == ModeWorld::SINGLEPLAYER) {
        singlePlayerTag.draw();
    } else {
        firstPlayerTag.draw();
        secondPlayerTag.draw();
    }
}