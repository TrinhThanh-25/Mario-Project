#include "GameState/PlayingState.h"
#include "GameState/CountingPointState.h"
#include "GameState/SettingState.h"
#include "raylib.h"
#include <algorithm>

PlayingState::PlayingState(World* world)
    : GameState(world, GameStateType::PLAYING), 
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
    if(IsKeyPressed(KEY_ESCAPE)) {
        SettingState* settingState = new SettingState(world);
        settingState->setStateBeforeSetting(GameStateType::PLAYING);
        world->setGameState(settingState);
        return;
    }
    std::vector<Tile*>& tile = map->getTile();
    std::vector<Block*>& block = map->getBlock();
    std::vector<Enemy*>& backEnemy = map->getBackEnemy();
    std::vector<Enemy*>& frontEnemy = map->getFrontEnemy();
    std::vector<Item*>& item = map->getItem();
    std::vector<Item*>& staticItem = map->getStaticItem();

    if(!isOneCharactersDead() && !isAllCharactersVictory()) {
        if(*modeWorld == ModeWorld::SINGLEPLAYER) {
            float centerX = GetScreenWidth() / 2.0f;
            float centerY = GetScreenHeight() / 2.0f;
            float charactersX = characters[0]->getX() + characters[0]->getWidth() / 2.0f;
            float charactersY = characters[0]->getY() + characters[0]->getHeight() / 2.0f;
            camera->offset = {centerX, centerY};
            if(charactersX < centerX) {
                camera->target.x = centerX; 
                map->setOffset(0);
            } else if(charactersX >= map->getWidth() - centerX) {
                camera->target.x = map->getWidth() - GetScreenWidth() / 2.0f;
            } else {
                camera->target.x = charactersX;
                map->setOffset(charactersX - centerX);
            }
            if(charactersY < centerY) {
                camera->target.y = centerY;
            } else if(charactersY >= map->getHeight() - centerY) {
                camera->target.y = map->getHeight() - GetScreenHeight() / 2.0f;
            } else {
                camera->target.y = charactersY;
            }
        }
        else {
            const float screenWidth = GetScreenWidth();
            const float screenHeight = GetScreenHeight();
            const float mapWidth = map->getWidth();
            const float mapHeight = map->getHeight();
            const float border = 100.0f;
            const float maxZoom = 1.0f, minZoom = 0.8f;
            const float maxDistanceX = screenWidth - 2 * border;
            const float maxDistanceY = screenHeight - 2 * border;

            Vector2 p1 = {characters[0]->getX() + characters[0]->getWidth() / 2.0f, characters[0]->getY() + characters[0]->getHeight() / 2.0f};
            Vector2 p2 = {characters[1]->getX() + characters[1]->getWidth() / 2.0f, characters[1]->getY() + characters[1]->getHeight() / 2.0f};
            Vector2 center = {(p1.x + p2.x) / 2.0f, (p1.y + p2.y) / 2.0f};
            float dx = fabs(p1.x - p2.x), dy = fabs(p1.y - p2.y);

            float zoomX = (dx > maxDistanceX) ? maxDistanceX / dx : 1.0f;
            float zoomY = (dy > maxDistanceY) ? maxDistanceY / dy : 1.0f;
            float targetZoom = std::min(zoomX, zoomY);
            if(targetZoom < minZoom) {
                targetZoom = minZoom;
            } else if(targetZoom > maxZoom) {
                targetZoom = maxZoom;
            }
            camera->zoom += (targetZoom - camera->zoom) * 0.1f;

            camera->offset = {screenWidth / 2.0f, screenHeight / 2.0f};
            float viewWidth = screenWidth / camera->zoom;
            float viewHeight = screenHeight / camera->zoom;

            if (center.x < viewWidth / 2.0f) {
                camera->target.x = viewWidth / 2.0f;
                map->setOffset(0);
            } else if (center.x > mapWidth - viewWidth / 2.0f) {
                camera->target.x = mapWidth - viewWidth / 2.0f;
            } else {
                camera->target.x = center.x;
                map->setOffset(center.x - viewWidth / 2.0f);
            }

            if (center.y < viewHeight / 2.0f) {
                camera->target.y = viewHeight / 2.0f;
            } else if (center.y >= mapHeight - viewHeight / 2.0f) {
                camera->target.y = mapHeight - viewHeight / 2.0f;
            } else {
                camera->target.y = center.y;
            }
        }
    }

    if (!*pausedUpdateCharacters) {
        for (auto& character : characters) {
            if((!isOneCharactersDead() || (isOneCharactersDead() && character->getState() == SpriteState::DYING)) && character->getState() != SpriteState::VICTORY) {
                character->update();
                character->setActivateWidth(GetScreenWidth() * 2.0f);
            }
        }
    }

    if(!isOneCharactersDead() && !isAllCharactersVictory()) {
        gameHud->update();
        map->playMusic();
        if (!*pausedForTransition) {
            for (auto& b : block) {
                b->update();
            }
            for (auto& e : backEnemy) {
                //e->update();
                // follow character
            }
            for (auto& e : frontEnemy) {
                //e->update();
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
                for (auto& i : item) {
                    i->collisionTile(t);
                }
                for (auto& e : backEnemy) {
                    e->collisionTile(t);
                }
                for (auto& e : frontEnemy) {
                    e->collisionTile(t);
                }
            }
            for (auto& b : block) {
                for (auto& character : characters) {
                    character->collisionBlock(b);
                }
                for (auto& i : item) {
                    i->collisionBlock(b);
                }
                for (auto& e : backEnemy) {
                    e->collisionBlock(b);
                }
                for (auto& e : frontEnemy) {
                    e->collisionBlock(b);
                }
            }
            for (auto& i : item) {
                for (auto& character : characters) {
                    i->collisionCharacter(character);
                }
            }
            for (int i = 0; i < item.size(); ++i) {
                if (item[i]->getState() == SpriteState::TO_BE_REMOVED) {
                    delete item[i];
                    item.erase(item.begin() + i);
                    --i;
                }
            }
            for (auto& i : staticItem) {
                for (auto& character : characters) {
                    i->collisionCharacter(character);
                }
            }
            for (int i = 0; i < staticItem.size(); ++i) {
                if (staticItem[i]->getState() == SpriteState::TO_BE_REMOVED) {
                    delete staticItem[i];
                    staticItem.erase(staticItem.begin() + i);
                    --i;
                }
            }
            for (auto& e : backEnemy) {
                for (auto& character : characters) {
                    character->collisionEnemy(e);
                }
            }
            for (int i = 0; i < backEnemy.size(); ++i) {
                if (backEnemy[i]->getState() == SpriteState::TO_BE_REMOVED) {
                    delete backEnemy[i];
                    backEnemy.erase(backEnemy.begin() + i);
                    --i;
                }
            }
            for (auto& e : frontEnemy) {
                for (auto& character : characters) {
                    character->collisionEnemy(e);
                }
            }
            for (int i = 0; i < frontEnemy.size(); ++i) {
                if (frontEnemy[i]->getState() == SpriteState::TO_BE_REMOVED) {
                    delete frontEnemy[i];
                    frontEnemy.erase(frontEnemy.begin() + i);
                    --i;
                }
            }
        }
    }
    else if (isOneCharactersDead()) {
        world->resetWhenCharacterDead();
    } 
    else if (isAllCharactersVictory()) {
        world->setGameState(new CountingPointState(world));
    }
}

void PlayingState::draw() {
    BeginMode2D(*camera);
    map->draw();
    EndMode2D();
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