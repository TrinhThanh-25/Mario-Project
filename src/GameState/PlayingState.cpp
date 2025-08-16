#include "GameState/PlayingState.h"
#include "GameState/CountingPointState.h"
#include "GameState/SettingState.h"
#include "GameState/GoNextMapState.h"
#include "GameState/GameStateFactory.h"
#include "Block/Block.h"
#include "Tile/Tile.h"
#include "Enemy/Enemy.h"
#include "Item/Item.h"
#include "raylib.h"
#include "SaveGame.h"
#include <algorithm>

PlayingState::PlayingState(World* world)
    : GameState(world, GameStateType::PLAYING), 
    characters(world->getCharacters()), 
    map(world->getMap()), 
    camera(world->getCamera()), 
    modeWorld(world->getModeWorld()), 
    gameHud(world->getGameHud()),
    pausedForTransition(world->getPausedForTransition()),
    pausedUpdateCharacters(world->getPausedUpdateCharacters()),
    skipFirstFrame(true) {
        world->setGameMode(GameMode::PLAYER);
        
        // Reset raylib internal timer để tránh physics explosion
        // Gọi GetTime() để lấy thời gian hiện tại và "sync" lại timer
        static double lastTime = 0.0;
        lastTime = GetTime();
}

PlayingState::~PlayingState() {
    
}

void PlayingState::update() {
    if (skipFirstFrame) {
        GetFrameTime();
        skipFirstFrame = false;
        return;
    }
    if(IsKeyPressed(KEY_ESCAPE)) {
        if(world->getGamePlay() == GamePlay::PLAYDEVELOPEDMAP) {
            SaveGame::saveGame(*world);
        }
        else {
            SaveGame::saveGame(*world, "../resources/SaveGame/" + map->getMapFileName() + ".json");
        }
        world->stopPlayerDownMusic();
        world->stopGameOverMusic();
        GameState* settingState = GameStateFactory::createGameState(world, GameStateType::SETTING);
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
            float mapWidth = map->getWidth();
            float mapHeight = map->getHeight();
            camera->offset = {centerX, centerY};
            if(charactersX <= centerX + 32) {
                camera->target.x = centerX + 32; 
                map->setOffset(0);
            } else if(charactersX >= mapWidth - centerX - 32) {
                camera->target.x = mapWidth - centerX - 32;
            } else {
                camera->target.x = charactersX;
                map->setOffset(charactersX - centerX - 32);
            }
            if(charactersY <= centerY + 32) {
                camera->target.y = centerY + 32;
            } else if(charactersY >= mapHeight - centerY) {
                camera->target.y = mapHeight - centerY;
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

            if (center.x < viewWidth / 2.0f + 32 / zoomX) {
                camera->target.x = viewWidth / 2.0f + 32 / zoomX;
                map->setOffset(0);
            } else if (center.x > mapWidth - viewWidth / 2.0f - 32 / zoomX) {
                camera->target.x = mapWidth - viewWidth / 2.0f - 32 / zoomX;
            } else {
                camera->target.x = center.x;
                map->setOffset(center.x - viewWidth / 2.0f - 32 / zoomX);
            }

            if (center.y < viewHeight / 2.0f + 32 / zoomY) {
                camera->target.y = viewHeight / 2.0f + 32 / zoomY;
            } else if (center.y >= mapHeight - viewHeight / 2.0f) {
                camera->target.y = mapHeight - viewHeight / 2.0f;
            } else {
                camera->target.y = center.y;
            }
        }
    }

    if (!*pausedUpdateCharacters) {
        if(isOneCharactersTransitioning()) {
            for (auto& character : characters) {
                if(character->getState() == SpriteState::SMALL_TO_SUPER || 
                    character->getState() == SpriteState::SMALL_TO_FLOWER || 
                    character->getState() == SpriteState::SUPER_TO_FLOWER || 
                    character->getState() == SpriteState::SUPER_TO_SMALL || 
                    character->getState() == SpriteState::FLOWER_TO_SMALL) {
                    character->update();
                }
            }
        }
        else {
            for (auto& character : characters) {
                if((!isOneCharactersDead() || (isOneCharactersDead() && character->getState() == SpriteState::DYING)) && character->getState() != SpriteState::VICTORY) {
                    character->update();
                }
            }
        }
    }

    if(!isOneCharactersDead() && !isAllCharactersVictory()) {
        if(world->getGamePlay() == GamePlay::PLAYDEVELOPEDMAP) {
            map->playMusic();
        }
        if (!*pausedForTransition) {
            for (auto& b : block) {
                b->update();
            }
            for (auto& e : backEnemy) {
                e->update(characters);
            }
            for (auto& e : frontEnemy) {
                e->update(characters);
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
            for (auto& i : staticItem) {
                for (auto& character : characters) {
                    i->collisionCharacter(character);
                }
            }
            for (auto& e : backEnemy) {
                for (auto& character : characters) {
                    character->collisionEnemy(e);
                }
                for (auto& be : backEnemy) {
                    be->collisionEnemy(e);
                }
                for (auto& fe : frontEnemy) {
                    fe->collisionEnemy(e);
                }
            }
            for (auto& e : frontEnemy) {
                for (auto& character : characters) {
                    character->collisionEnemy(e);
                }
                for (auto& be : backEnemy) {
                    be->collisionEnemy(e);
                }
                for (auto& fe : frontEnemy) {
                    fe->collisionEnemy(e);
                }
            }
            
            for (int i = item.size() - 1; i >= 0; --i) {
                if (item[i]->getState() == SpriteState::TO_BE_REMOVED) {
                    delete item[i];
                    item.erase(item.begin() + i);
                }
            }
            for (int i = staticItem.size() - 1; i >= 0; --i) {
                if (staticItem[i]->getState() == SpriteState::TO_BE_REMOVED) {
                    delete staticItem[i];
                    staticItem.erase(staticItem.begin() + i);
                }
            }
            for (int i = backEnemy.size() - 1; i >= 0; --i) {
                if (backEnemy[i]->getState() == SpriteState::TO_BE_REMOVED) {
                    delete backEnemy[i];
                    backEnemy.erase(backEnemy.begin() + i);
                }
            }
            for (int i = frontEnemy.size() - 1; i >= 0; --i) {
                if (frontEnemy[i]->getState() == SpriteState::TO_BE_REMOVED) {
                    delete frontEnemy[i];
                    frontEnemy.erase(frontEnemy.begin() + i);
                }
            }
        }
        gameHud->update();
    }
    else if (isOneCharactersDead()) {
        world->resetWhenCharacterDead();
    } 
    else if (isAllCharactersVictory()) {
        world->setGameState(GameStateFactory::createGameState(world, GameStateType::COUNTING_POINT));
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

bool PlayingState::isOneCharactersTransitioning() const {
    for (const auto& character : characters) {
        if (character->getState() == SpriteState::SMALL_TO_SUPER || 
            character->getState() == SpriteState::SMALL_TO_FLOWER || 
            character->getState() == SpriteState::SUPER_TO_FLOWER || 
            character->getState() == SpriteState::SUPER_TO_SMALL || 
            character->getState() == SpriteState::FLOWER_TO_SMALL) {
            return true;
        }
    }
    return false;
}

void PlayingState::enter() {
    skipFirstFrame = true;
    if(world->getGamePlay() == GamePlay::PLAYDEVELOPEDMAP) {
        map->playMusic();
    }
}

void PlayingState::exit() {
    map->stopMusic();
}

json PlayingState::saveToJson() const {
    json j = GameState::saveToJson();
    return j;
}

void PlayingState::loadFromJson(const json& j) {
    GameState::loadFromJson(j);
}