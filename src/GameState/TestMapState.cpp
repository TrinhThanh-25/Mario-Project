#include "GameState/TestMapState.h"
#include "GameState/CustomMapState.h"
#include "GameState/GameStateFactory.h"
#include "Game/World.h"
#include "Character/Mario.h"
#include "Character/Character.h"
#include "Character/CharacterFactory.h"
#include "Block/Block.h"
#include "Tile/Tile.h"
#include "Enemy/Enemy.h"
#include "Item/Item.h"
#include "raygui.h"

TestMapState::TestMapState(World* world)
    : GameState(world, GameStateType::TEST_MAP),
      map(world->getMap()), 
      camera(world->getCamera()), 
      characters(world->getCharacters()),
      pausedForTransition(world->getPausedForTransition()),
      pausedUpdateCharacters(world->getPausedUpdateCharacters()) {
    characters.clear();
    Character* character = CharacterFactory::createCharacter(CharacterName::MARIO, ModePlayer::FIRSTPLAYER);
    character->setWorld(world);
    world->getKeyManager()->setKeyManagerForCharacter(character, ModePlayer::FIRSTPLAYER);
    world->setGameMode(GameMode::TESTER);
    world->setGamePlay(GamePlay::PLAYCUSTOMMAP);
    characters.push_back(character);
    offsetX = camera->target.x - characters[0]->getX() - characters[0]->getWidth() / 2.0f;
    offsetY = camera->target.y - characters[0]->getY() - characters[0]->getHeight() / 2.0f;
}

TestMapState::TestMapState(World* world, std::string mapFileName)
    : TestMapState(world) {
    setMapFileName(mapFileName);
}

void TestMapState::setMapFileName(const std::string& mapFileName) {
    this->mapFileName = mapFileName;
    map->loadMap(mapFileName);
}

TestMapState::TestMapState(World* world, std::string mapFileName, int width, int height, const std::vector<int>& mapGrid)
    : TestMapState(world) {
    map->setMap(width, height, mapGrid);
    map->setMapFileName(mapFileName);
}

TestMapState::~TestMapState() {
    for (auto& character : characters) {
        delete character;
        character = nullptr;
    }
    characters.clear();
    map->setNetMode(false);
}

void TestMapState::update() {
    std::vector<Tile*>& tile = map->getTile();
    std::vector<Block*>& block = map->getBlock();
    std::vector<Enemy*>& backEnemy = map->getBackEnemy();
    std::vector<Enemy*>& frontEnemy = map->getFrontEnemy();
    std::vector<Item*>& item = map->getItem();
    std::vector<Item*>& staticItem = map->getStaticItem();
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        camera->target.x -= GetMouseDelta().x / camera->zoom;
        camera->target.y -= GetMouseDelta().y / camera->zoom;
        offsetX = camera->target.x - characters[0]->getX() - characters[0]->getWidth() / 2.0f;
        offsetY = camera->target.y - characters[0]->getY() - characters[0]->getHeight() / 2.0f;
    }
    else {
        camera->target.x = characters[0]->getX() + characters[0]->getWidth() / 2.0f + offsetX;
        camera->target.y = characters[0]->getY() + characters[0]->getHeight() / 2.0f + offsetY;
    }
    float wheel = GetMouseWheelMove();
    if (wheel != 0.0f) {
        if(wheel > 0.0f) {
            camera->zoom *= 1.1f;
        } else {
            camera->zoom /= 1.1f;
        }
    }
    characters[0]->update();
    if(characters[0]->getState() != SpriteState::DYING) {
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
    }
    else {
        map->reset(true);
        characters[0]->reset(true);
    }
}

void TestMapState::changeCharacter(CharacterName newCharacter) {
    Character* character = CharacterFactory::createCharacter(newCharacter, ModePlayer::FIRSTPLAYER);
    character->setWorld(world);
    world->getKeyManager()->setKeyManagerForCharacter(character, ModePlayer::FIRSTPLAYER);
    character->copyState(*characters[0]);
    
    delete characters[0];
    characters[0] = character;
}

void TestMapState::draw() {
    BeginMode2D(*camera);
    map->draw();
    EndMode2D();
    
    int originalTextSize = GuiGetStyle(DEFAULT, TEXT_SIZE);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 18);
    
    int compMargin = 20;
    Rectangle guiPanelRect = { (float)GetScreenWidth() - 250, (float)GetScreenHeight() - 420, 230, 400 };
    GuiPanel( guiPanelRect, "Controls" );
    
    std::string characterButtonText = "Character: " + characterNames[currentCharacterIndex];
    if (GuiButton({ guiPanelRect.x + compMargin, guiPanelRect.y + 40, 190, 35 }, characterButtonText.c_str())) {
        currentCharacterIndex = (currentCharacterIndex + 1) % availableCharacters.size();
        changeCharacter(availableCharacters[currentCharacterIndex]);
    }
    
    switch (characters[0]->getType()) {
        case CharacterType::SMALL:
            currentTypeIndex = 0;
            break;
        case CharacterType::SUPER:
            currentTypeIndex = 1;
            break;
        case CharacterType::FLOWER:
            currentTypeIndex = 2;
            break;
    }
    std::string typeButtonText = "Type: " + typeNames[currentTypeIndex];
    if (GuiButton({ guiPanelRect.x + compMargin, guiPanelRect.y + 90, 190, 35 }, typeButtonText.c_str())) {
        currentTypeIndex = (currentTypeIndex + 1) % availableTypes.size();
        characters[0]->setType(availableTypes[currentTypeIndex]);
    }
    
    GuiCheckBox( { guiPanelRect.x + compMargin, guiPanelRect.y + 140, 30, 30 }, "creative", &creative );
    GuiCheckBox( { guiPanelRect.x + compMargin, guiPanelRect.y + 190, 30, 30 }, "immortal", &immortal );
    GuiCheckBox( { guiPanelRect.x + compMargin, guiPanelRect.y + 240, 30, 30 }, "net", &net );
    
    if (GuiButton({ guiPanelRect.x + compMargin, guiPanelRect.y + 290, 190, 35 }, "Reset Map")) {
        map->reset(true);
    }

    if(GuiButton({ guiPanelRect.x + compMargin, guiPanelRect.y + 340, 190, 35 }, "Return") || IsKeyPressed(KEY_ESCAPE)) {
        GameState* customMapState = GameStateFactory::createGameState(world, GameStateType::CUSTOM_MAP);
        customMapState->setMapFileName(map->getMapFileName());
        customMapState->setMap(map->getWidth() / 32, map->getHeight() / 32, map->getMapGrid());
        customMapState->setIsSaved(isSaved);
        world->setGameState(customMapState);
        return;
    }
    
    GuiSetStyle(DEFAULT, TEXT_SIZE, originalTextSize);
    
    characters[0]->setCreativeMode(creative);
    characters[0]->setInvulnerableMode(immortal);
    map->setNetMode(net);
    characters[0]->drawGameHud();
}

void TestMapState::setIsSaved(bool saved) {
    isSaved = saved;
}

void TestMapState::setMap(int width, int height, const std::vector<int>& mapGrid) {
    map->setMap(width, height, mapGrid);
}

void TestMapState::setCameraPosition(float x, float y) {
    camera->target.x = x;
    camera->target.y = y;
    offsetX = camera->target.x - characters[0]->getX() - characters[0]->getWidth() / 2.0f;
    offsetY = camera->target.y - characters[0]->getY() - characters[0]->getHeight() / 2.0f;
}