#include "GameState/PlayingState.h"
#include <algorithm>

PlayingState::PlayingState(World* world)
    : GameState(world), characters(world->getCharacters()), map(world->getMap()), camera(world->getCamera()), modeWorld(world->getModeWorld()) {
}

PlayingState::~PlayingState() {
    
}

void PlayingState::update() {
    std::vector<Tile*>& tile = map->getTile();
    for (auto& character : characters) {
        character->update();
    }
    for (auto& t : tile) {
        for (auto& character : characters) {
            character->collisionTile(t);
        }
    }
    if(*modeWorld == ModeWorld::SINGLEPLAYER) {
        float centerX = GetScreenWidth() / 2.0f;
        float centerY = GetScreenHeight() / 2.0f;
        float charactersX = characters[0]->getX() + characters[0]->getWidth() / 2.0f;
        float charactersY = characters[0]->getY() + characters[0]->getHeight() / 2.0f;
        camera->offset.x = 0;
        if(charactersX < centerX) {
            camera->target.x = 0;
            map->setOffset(0);
        } else if(charactersX > map->getWidth() - centerX) {
            camera->target.x = map->getWidth() - GetScreenWidth();
        } else {
            camera->target.x = charactersX - centerX;
            map->setOffset(charactersX - centerX);
        }
        camera->offset.y = 0;
        if(charactersY < centerY) {
            camera->target.y = 0;
        } else if(charactersY > map->getHeight() - centerY) {
            camera->target.y = map->getHeight() - GetScreenHeight();
        } else {
            camera->target.y = charactersY - centerY;
        }
    }
    else {
        // MULTIPLAYER mode
    }
}

void PlayingState::draw() {
    map->draw();
}