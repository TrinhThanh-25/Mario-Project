#include "GameState/ChooseCustomizedMap.h"
#include "GameState/TitleScreenState.h"
#include "Common/ResourceManager.h"
#include "GameState/PlayingState.h"
#include "GameState/CustomMapState.h"
#include "GameState/ChooseCharacterState.h"
#include "GameState/GameStateFactory.h"
#include "SaveGame.h"
#include "json.hpp"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <unordered_set>

using json = nlohmann::json;

MyButton::MyButton(Rectangle rectangle, std::string text, int fontSize, Color color, bool isHandled, bool isDrawn)
    : rectangle(rectangle), text(text), fontSize(fontSize), color(color), isHandled(isHandled), isDrawn(isDrawn), isHovered(false), wasHovered(false) {}

void MyButton::draw() {
    if (!isDrawn) return;
        DrawTexture(ResourceManager::getTexture()[textureName+"0"], rectangle.x, rectangle.y, color);
    if (isHovered && isHandled)
    {
        DrawTexture(ResourceManager::getTexture()[textureName+"1"], rectangle.x, rectangle.y, color);
    }
    ResourceManager::drawBigString(this->text, this->rectangle.x + (this->rectangle.width - ResourceManager::getDrawBigStringWidth(this->text, fontSize)) / 2, this->rectangle.y + (this->rectangle.height - ResourceManager::getDrawBigStringHeight(fontSize)) / 2, fontSize);
}

void MyButton::update() {
    if (isHandled) {
        Vector2 mousePos = GetMousePosition();
        wasHovered = isHovered;
        isHovered = CheckCollisionPointRec(mousePos, rectangle);
        if (isHovered && !wasHovered) {
            PlaySound(ResourceManager::getSound()["HoverButton"]);
        }
    }
}

void MyButton::toggleHandle() {
    isHandled = !isHandled;
}

bool MyButton::isHandle() const {
    return isHandled;
}

bool MyButton::isHover() const {
    return isHovered;
}

bool MyButton::isDraw() const {
    return isDrawn;
}

void MyButton::toggleDraw() {
    isDrawn = !isDrawn;
}

void MyButton::setTextureName(std::string name) {
    textureName = name;
}

void MyButton::setColor(Color new_color){
    color = new_color;
}

void MyButton::setText(const std::string &new_text){
    text = new_text;
}

void MyButton::setPos(Vector2 newPos) {
    rectangle.x = newPos.x;
    rectangle.y = newPos.y;
}

std::string MyButton::getText() {
    return text;
}

bool MyButton::isClicked() {
    if (isHandled && isHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        PlaySound(ResourceManager::getSound()["ClickButton"]);
        return true;
    }
    return false;
}

ChooseCustomizedMapState::ChooseCustomizedMapState(World *world) : GameState(world, GameStateType::CHOOSE_CUSTOMIZED_MAP)
{
    load();
    mapButtons.emplace_back(Rectangle{219, 64, 1160, 173}, "EMPTY SLOT", 30, WHITE, true, true);
    mapButtons.emplace_back(Rectangle{222, 264, 1160, 173}, "EMPTY SLOT", 30, WHITE, true, true);
    mapButtons.emplace_back(Rectangle{222, 464, 1160, 173}, "EMPTY SLOT", 30, WHITE, true, true);
    mapButtons.emplace_back(Rectangle{222, 664, 1160, 173}, "EMPTY SLOT", 30, WHITE, true, true);
    for (int i = 0; i < mapButtons.size(); i++)
    {
        mapButtons[i].setTextureName("CMButton");
    }
    OptionButtons.emplace_back(Rectangle{577, 250, 442, 94}, "1 Player Game", 20, WHITE, true, true);
    OptionButtons.emplace_back(Rectangle{577, 360, 442, 94}, "2 Player Game", 20, WHITE, true, true);
    OptionButtons.emplace_back(Rectangle{577, 470, 442, 94}, "Edit", 20, WHITE, true, true);
    OptionButtons.emplace_back(Rectangle{577, 580, 442, 94}, "Delete", 20, WHITE, true, true);
    for (int i = 0; i < OptionButtons.size(); i++)
    {
        OptionButtons[i].setTextureName("ESCMenuButton");
    }
    OptionActive = false;
    currentIndex = 0;
    currentIndex = -1;
    setMap(true);
}

ChooseCustomizedMapState::~ChooseCustomizedMapState()
{
    mapButtons.clear();
    OptionButtons.clear();
    save();
}

void ChooseCustomizedMapState::update()
{
    if (OptionActive) {
        if (IsKeyPressed(KEY_ESCAPE)) {
            OptionActive = false;
            return;
        }
        
        if (mapButtons[MapChosen].getText() == "EMPTY SLOT") {
            OptionButtons[1].update();
        } else {
            for (auto &button : OptionButtons) {
                button.update();
            }
        }
        
        // Check for button clicks
        if (OptionButtons[0].isClicked()) {
            GameState* newState = GameStateFactory::createGameState(world, GameStateType::CHOOSE_CHARACTER);
            newState->setMapFileName(mapButtons[MapChosen].getText());
            newState->setModeWorld(ModeWorld::SINGLEPLAYER);
            world->setGameState(newState);
            return;
        } else if (OptionButtons[1].isClicked()) {   
            GameState* newState = GameStateFactory::createGameState(world, GameStateType::CHOOSE_CHARACTER);
            newState->setMapFileName(mapButtons[MapChosen].getText());
            newState->setModeWorld(ModeWorld::MULTIPLAYER);
            world->setGameState(newState);
            return;
        } else if (OptionButtons[2].isClicked()) {
            GameState* newState = GameStateFactory::createGameState(world, GameStateType::CUSTOM_MAP);
            newState->setMapFileName(mapButtons[MapChosen].getText());
            newState->saveHistory();
            world->setGameState(newState);
            return;
        } else if (OptionButtons[3].isClicked()) {
            std::string mapToDelete = mapButtons[MapChosen].getText();
            std::filesystem::remove("../resources/Map/" + mapToDelete + ".json");
            numberOfFile--;
            listFileName.erase(listFileName.begin()+currentIndex+MapChosen);
            mapNameSet.erase(mapToDelete);
            if (currentIndex + 4 > numberOfFile) {
                currentIndex = numberOfFile - 4;
            } else {
                currentIndex--;
            }
            setMap(true);
            OptionActive = false;
        } else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            OptionActive = false;
            return;
        }
    } else {
        if (IsKeyPressed(KEY_ESCAPE)) {
            world->setGameState(GameStateFactory::createGameState(world, GameStateType::TITLE_SCREEN));
            return;
        }
        
        for (auto& button : mapButtons) {
            button.update();
        }
        
        for (int i = 0; i < mapButtons.size(); i++) {
            if (mapButtons[i].isClicked()) {
                MapChosen = i;
                if (mapButtons[MapChosen].getText() == "EMPTY SLOT") {
                    int newMapIndex = 1;
                    std::string newMapName;
                    do {
                        newMapName = "New Map " + std::to_string(newMapIndex);
                        newMapIndex++;
                    } while (mapNameSet.find(newMapName) != mapNameSet.end());
                    
                    numberOfFile++;
                    listFileName.push_back(newMapName);
                    mapNameSet.insert(newMapName);
                    save();
                    createDefaultMapFile(newMapName);
                    GameState* newState = GameStateFactory::createGameState(world, GameStateType::CUSTOM_MAP);
                    newState->setMapFileName(newMapName);
                    newState->saveHistory();
                    world->setGameState(newState);
                    return;
                }
                OptionActive = true;
                break;
            }
        }
        
        float mouse = GetMouseWheelMove();
        MouseWhellAcum += mouse;
        if (MouseWhellAcum >= MouseWhellTime) {
            MouseWhellAcum = 0;
            setMap(false);
        } else if (MouseWhellAcum <= -MouseWhellTime) {
            MouseWhellAcum = 0;
            setMap(true);
        }
    }
}

void ChooseCustomizedMapState::draw() {
    ClearBackground(WHITE);
    DrawTexture(ResourceManager::getTexture()["CMPad"], 124, 9, WHITE);
    for (auto &button : mapButtons)
    {
        button.draw();
    }
    if (OptionActive)
    {
        DrawRectangle(0, 0, 1600, 900, Fade({159, 139, 121, 255}, 0.5f));
        DrawTexture(ResourceManager::getTexture()["ESCMenuPad"],444,170,WHITE);
        if (mapButtons[MapChosen].getText() == "EMPTY SLOT") {
            OptionButtons[1].draw();
        } else {
            for (auto& button:OptionButtons) {
                button.draw();
            }
        }
    }
}

void ChooseCustomizedMapState::load(std::string fileName)
{
    std::ifstream file(fileName);
    if (!file) {
        std::cerr << "Could not open json file " << fileName << std::endl;
        return;
    }
    json j;
    file >> j;

    if (j.contains("listMapName")) {
        this->listFileName = j["listMapName"].get<std::vector<std::string>>();
        numberOfFile = this->listFileName.size();
        mapNameSet.clear();
        for (const auto& name : listFileName) {
            mapNameSet.insert(name);
        }
    } else {
        std::cerr << "Invalid JSON format in " << fileName << std::endl;
    }
}

void ChooseCustomizedMapState::save(std::string fileName)
{
    json j;
    j["listMapName"] = this->listFileName;
    std::ofstream outFile(fileName);
    if (!outFile) {
        std::cerr << "Could not open file " << fileName << " for saving." << std::endl;
        return;
    }
    outFile << j.dump(4);
    if (!outFile) {
        std::cerr << "Error writing to file " << fileName << std::endl;
    } else {
        std::cout << "Map list saved successfully to " << fileName << std::endl;
    }
    outFile.close();
}

void ChooseCustomizedMapState::setMap(bool Up)
{
    if (Up) {
        if (numberOfFile < 4) {
            currentIndex = 0;
            int i = 0;
            for (i;i<numberOfFile;i++) {
                mapButtons[i].setText(listFileName[i]);
            }
            for (i;i<4;i++) {
                mapButtons[i].setText("EMPTY SLOT");
            }
        }
        if (currentIndex + 4 == numberOfFile) {
            currentIndex++;
            mapButtons[0].setText(listFileName[numberOfFile-3]);
            mapButtons[1].setText(listFileName[numberOfFile-2]);
            mapButtons[2].setText(listFileName[numberOfFile-1]);
            mapButtons[3].setText("EMPTY SLOT");
        } else if (currentIndex + 4 < numberOfFile) {
            currentIndex++;
            mapButtons[0].setText(listFileName[currentIndex]);
            mapButtons[1].setText(listFileName[currentIndex+1]);
            mapButtons[2].setText(listFileName[currentIndex+2]);
            mapButtons[3].setText(listFileName[currentIndex+3]);
        }
    } else {
        if (currentIndex == 0) return;
        currentIndex--;
        mapButtons[0].setText(listFileName[currentIndex]);
        mapButtons[1].setText(listFileName[currentIndex+1]);
        mapButtons[2].setText(listFileName[currentIndex+2]);
        mapButtons[3].setText(listFileName[currentIndex+3]);
    }
}

void ChooseCustomizedMapState::createDefaultMapFile(const std::string& mapName) {
    std::ofstream newMapFile("../resources/Map/" + mapName + ".json");
    if (!newMapFile) {
        std::cerr << "Failed to create new map file: " << mapName << std::endl;
        return;
    }
    
    int width = 35;
    int height = 20;
    
    newMapFile << "{\n";
    newMapFile << "    \"width\": " << width << ",\n";
    newMapFile << "    \"height\": " << height << ",\n";
    newMapFile << "    \"layers\": [\n";
    newMapFile << "        {\n";
    newMapFile << "            \"data\": [";
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = y * width + x;
            newMapFile << "0";
            if (index < (height * width) - 1) newMapFile << ", ";
            if ((index + 1) % width == 0 && index < (height * width) - 1) newMapFile << "\n                ";
        }
    }
    newMapFile << "]\n";
    newMapFile << "        }\n";
    newMapFile << "    ]\n";
    newMapFile << "}";
    newMapFile.close();
}