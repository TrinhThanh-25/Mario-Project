#include "GameState/PlayingState.h"
#include <algorithm>

PlayingState::PlayingState(World* world)
    : GameState(world), 
    characters(world->getCharacters()), 
    map(world->getMap()), 
    camera(world->getCamera()), 
    modeWorld(world->getModeWorld()), 
    gameHud(world->getGameHud()),
    pausedForTransition(world->getPausedForTransition()),
    pausedUpdateCharacters(world->getPausedUpdateCharacters()) {
}

PlayingState::~PlayingState() {
    
}

void PlayingState::update() {
    std::vector<Tile*>& tile = map->getTile();
    std::vector<Block*>& block = map->getBlock();
    std::vector<Enemy*>& backEnemy = map->getBackEnemy();
    std::vector<Enemy*>& frontEnemy = map->getFrontEnemy();
    std::vector<Item*>& item = map->getItem();
    std::vector<Item*>& staticItem = map->getStaticItem();

    if(!isOneCharactersDead() && !isAllCharactersVictory()) {
        gameHud->update();
        if (!*pausedUpdateCharacters) {
            for (auto& character : characters) {
                character->update();
                character->setActivateWidth(GetScreenWidth() * 2);
            }
        }
        if (!*pausedForTransition) {
            for (auto& b : block) {
                b->update();
            }
            for (auto& e : backEnemy) {
                e->update();
                // follow character
            }
            for (auto& e : frontEnemy) {
                e->update();
                // follow character
            }
            for (auto& i : item) {
                i->update();
            }
            for (auto& i : staticItem) {
                i->update();
            }
            for (auto& t : tile) {
                for (auto& character : characters) {
                    character->collisionTile(t);
                }
                // enemy x tile
                // item x tile
            }
            for (auto& b : block) {
                for (auto& character : characters) {
                    character->collisionBlock(b);
                }
                // enemy x block
                // item x block
            }
            // item x character
            // delete removed item

            // static item x character
            // delete removed static item

            for (auto& e : backEnemy) {
                for (auto& character : characters) {
                    character->collisionEnemy(e);
                }
            }
            // delete removed back enemy
            for (auto& e : frontEnemy) {
                for (auto& character : characters) {
                    character->collisionEnemy(e);
                }
            }
            // delete removed front enemy
        }
    }
    else if (isOneCharactersDead()) {
        world->resetWhenCharacterDead();
    } 
    else if (isAllCharactersVictory()) {
        // tính điểm
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
        // fix lỗi kéo nhau
        const float screenWidth = GetScreenWidth();
        const float screenHeight = GetScreenHeight();
        const float mapWidth = map->getWidth();
        const float mapHeight = map->getHeight();

        const float border = 100.0f;
        const float maxDistanceX = screenWidth - 2 * border; 
        const float maxDistanceY = screenHeight - 2 * border;

        const float maxZoom = 1.0f;
        const float minZoom = 0.8f;

        Vector2 p1Center = {characters[0]->getX() + characters[0]->getWidth() / 2.0f,characters[0]->getY() + characters[0]->getHeight() / 2.0f};
        Vector2 p2Center = {characters[1]->getX() + characters[1]->getWidth() / 2.0f,characters[1]->getY() + characters[1]->getHeight() / 2.0f};

        float dx = fabs(p1Center.x - p2Center.x);
        float dy = fabs(p1Center.y - p2Center.y);

        float centerX = (p1Center.x + p2Center.x) / 2.0f;
        float centerY = (p1Center.y + p2Center.y) / 2.0f;

        float zoomX = 1.0f;
        float zoomY = 1.0f;

        if (dx > maxDistanceX) {
                zoomX = maxDistanceX / dx; 
        }   
        if (dy > maxDistanceY) {
            zoomY = maxDistanceY / dy; 
        }

        float targetZoom = std::min(zoomX, zoomY);
        if(targetZoom < minZoom) {
            targetZoom = minZoom; 
        } else if (targetZoom > maxZoom) {
            targetZoom = maxZoom; 
        }
        camera->zoom = targetZoom;

        camera->offset = { 0, 0 };

        float viewWidth = screenWidth / camera->zoom;
        float viewHeight = screenHeight / camera->zoom;

        float minTargetX = viewWidth / 2.0f;
        float maxTargetX = mapWidth - viewWidth / 2.0f;
        float minTargetY = viewHeight / 2.0f;
        float maxTargetY = mapHeight - viewHeight / 2.0f;

        if (centerX < minTargetX) {
            camera->target.x = 0;
        } 
        else if (centerX > maxTargetX) {
            camera->target.x = maxTargetX - viewWidth / 2.0f;
        }
        else {
            camera->target.x = centerX - viewWidth / 2.0f;
        }

        if (centerY < minTargetY) {
            camera->target.y = 0;
        } 
        else if (centerY > maxTargetY) {
            camera->target.y = maxTargetY - viewHeight / 2.0f;
        }
        else {
            camera->target.y = centerY - viewHeight / 2.0f;
        }

        Rectangle camBounds = {camera->target.x ,camera->target.y,viewWidth,viewHeight};

        for (int i = 0; i < 2; i++) {
            Character* c = characters[i];

            if (c->getX() < camBounds.x)
                c->setX(camBounds.x);
            else if (c->getX() + c->getWidth() > camBounds.x + camBounds.width)
                c->setX(camBounds.x + camBounds.width - c->getWidth());

            if (c->getY() < camBounds.y)
                c->setY(camBounds.y);
            else if (c->getY() + c->getHeight() > camBounds.y + camBounds.height)
                c->setY(camBounds.y + camBounds.height - c->getHeight());
        }
    }
}

void PlayingState::draw() {
    map->draw();
    gameHud->draw();
}

bool PlayingState::isOneCharactersDead() const {
    for (const auto& character : characters) {
        if (character->getState() == SpriteState::DYING) {
            return true;
        }
    }
    return false;
}

bool PlayingState::isAllCharactersVictory() const {
    for (const auto& character : characters) {
        if (character->getState() != SpriteState::VICTORY) {
            return false;
        }
    }
    return true;
}