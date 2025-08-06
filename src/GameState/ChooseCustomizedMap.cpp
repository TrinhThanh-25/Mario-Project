#include "GameState/ChooseCustomizedMap.h"
#include "GameState/TitleScreenState.h"
#include "Common/ResourceManager.h"
#include "GameState/PlayingState.h"
#include "GameState/CustomMapState.h"
#include <filesystem>

void MyButton::draw()
{
    if (!isDrawn) return;
        DrawTexture(ResourceManager::getTexture()[textureName+"0"], rectangle.x, rectangle.y, color);
    if (isHovered && isHandled)
    {
        DrawTexture(ResourceManager::getTexture()[textureName+"1"], rectangle.x, rectangle.y, color);
    }
    ResourceManager::drawBigString(this->text, this->rectangle.x + (this->rectangle.width - ResourceManager::getDrawBigStringWidth(this->text, fontSize)) / 2, this->rectangle.y + (this->rectangle.height - ResourceManager::getDrawBigStringHeight(fontSize)) / 2, fontSize);
}

void MyButton::update()
{
    if (isHandled)
    {
        Vector2 mousePos = GetMousePosition();
        isHovered = CheckCollisionPointRec(mousePos, rectangle);
    }
}

void MyButton::setPos(Vector2 newPos) {
    rectangle.x = newPos.x;
    rectangle.y = newPos.y;
}

ChooseCustomizedMapState::ChooseCustomizedMapState(World *world) : GameState(world, GameStateType::CHOOSE_CUSTOMIZED_MAP)
{
    mapButtons.emplace_back(Rectangle{219, 64, 1160, 173}, "Map 1", 30, WHITE, true, true);
    mapButtons.emplace_back(Rectangle{222, 264, 1160, 173}, "Map 2", 30, WHITE, true, true);
    mapButtons.emplace_back(Rectangle{222, 464, 1160, 173}, "Map 3", 30, WHITE, true, true);
    mapButtons.emplace_back(Rectangle{222, 664, 1160, 173}, "Map 4", 30, WHITE, true, true);
    for (int i = 0; i < mapButtons.size(); i++)
    {
        mapButtons[i].setTextureName("CMButton");
    }

    ESCMenuButtons.emplace_back(Rectangle{577, 355, 442, 94}, "Resume", 20, WHITE, true, true);
    ESCMenuButtons.emplace_back(Rectangle{577, 504, 442, 94}, "Return", 20, WHITE, true, true);
    for (int i = 0; i < ESCMenuButtons.size(); i++)
    {
        ESCMenuButtons[i].setTextureName("ESCMenuButton");
    }

    OptionButtons.emplace_back(Rectangle{577, 288, 442, 94}, "Play", 20, WHITE, true, true);
    OptionButtons.emplace_back(Rectangle{577, 432, 442, 94}, "Edit", 20, WHITE, true, true);
    OptionButtons.emplace_back(Rectangle{577, 576, 442, 94}, "Delete", 20, WHITE, true, true);
    for (int i = 0; i < OptionButtons.size(); i++)
    {
        OptionButtons[i].setTextureName("ESCMenuButton");
    }
    ESCMenuActive = false;
    OptionActive = false;
    currentIndex = 0;

    // Test
    numberOfFile = 4;
    fileName.resize(numberOfFile);
    for (int i=0;i<fileName.size();i++) {
        fileName[i] = std::to_string(i);
    }

    // loadFromJson(fileName)

    currentIndex = -1;
    setMap(true);
}

ChooseCustomizedMapState::~ChooseCustomizedMapState()
{
    mapButtons.clear();
    ESCMenuButtons.clear();
    OptionButtons.clear();
    // save(fileName)
}
void ChooseCustomizedMapState::update()
{
    if (ESCMenuActive)
    {
        if (IsKeyPressed(KEY_ESCAPE))
        {
            ESCMenuActive = false;
            return;
        }
        for (auto &button : ESCMenuButtons)
        {
            button.update();
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (ESCMenuButtons[0].isHover())
            {
                ESCMenuActive = false;
                return;
            }
            else if (ESCMenuButtons[1].isHover())
            {
                world->setGameState(new TitleScreenState(world));
            }
        }
    }
    else if (OptionActive)
    {
        if (IsKeyPressed(KEY_ESCAPE))
        {
            OptionActive = false;
            return;
        }
        if (mapButtons[MapChosen].getText() == "EMPTY SLOT") {
            OptionButtons[1].update();
        } else {
            for (auto &button : OptionButtons)
            {
                button.update();
            }
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (OptionButtons[0].isHover()) {
                // Play mapButtons[MapChosen].getText()
                // world->setGamePlay(GamePlay::PLAYCUSTOMMAP);
            } else if (OptionButtons[1].isHover()) {   
                // world->setGameState(new CustomMapState(world,mapButtons[MapChosen].getText()));
            } else if (OptionButtons[2].isHover()) {
                // std::filesystem::remove(mapButtons[MapChosen].getText());
                numberOfFile--;
                fileName.erase(fileName.begin()+currentIndex+MapChosen);
                if (currentIndex + 4 > numberOfFile) {
                    currentIndex = numberOfFile - 4;
                } else currentIndex--;
                setMap(true);
                OptionActive = false;
            } else {
                OptionActive = false;
                return;
            }
        }
    } else {
        if (IsKeyPressed(KEY_ESCAPE))
        {
            ESCMenuActive = true;
            return;
        }
        for (auto& button:mapButtons) {
            button.update();
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            for (int i=0;i<mapButtons.size();i++) {
                if (mapButtons[i].isHover()) {
                    MapChosen = i;
                    if (mapButtons[MapChosen].getText() == "EMPTY SLOT") {
                        numberOfFile++;
                        fileName.push_back("New Map " + std::to_string(fileName.size()));
                        // New file with "New Map" name
                        // world->setGameState(new CustomMapState(world,fileName[fileName.size()-1]));
                        return;
                    }
                    OptionActive = true;
                }
            }
        }
        // Mouse Whell handling
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

void ChooseCustomizedMapState::draw()
{
    ClearBackground(WHITE);
    DrawTexture(ResourceManager::getTexture()["CMPad"], 124, 9, WHITE);
    for (auto &button : mapButtons)
    {
        button.draw();
    }
    if (ESCMenuActive)
    {
        DrawRectangle(0, 0, 1600, 900, Fade({159, 139, 121, 255}, 0.5f));
        DrawTexture(ResourceManager::getTexture()["ESCMenuPad"],444,170,WHITE);
        for (auto &button : ESCMenuButtons)
        {
            button.draw();
        }
    } else if (OptionActive)
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

void ChooseCustomizedMapState::loadFromJson(std::string fileName)
{
    //
}

void ChooseCustomizedMapState::save(std::string fileName)
{
    //
}
void ChooseCustomizedMapState::setMap(bool Up)
{
    if (Up) {
        if (numberOfFile < 4) {
            currentIndex = 0;
            int i = 0;
            for (i;i<numberOfFile;i++) {
                mapButtons[i].setText(fileName[i]);
            }
            for (i;i<4;i++) {
                mapButtons[i].setText("EMPTY SLOT");
            }
        }
        if (currentIndex + 4 == numberOfFile) {
            currentIndex++;
            mapButtons[0].setText(fileName[numberOfFile-3]);
            mapButtons[1].setText(fileName[numberOfFile-2]);
            mapButtons[2].setText(fileName[numberOfFile-1]);
            mapButtons[3].setText("EMPTY SLOT");
        } else if (currentIndex + 4 < numberOfFile) {
            currentIndex++;
            mapButtons[0].setText(fileName[currentIndex]);
            mapButtons[1].setText(fileName[currentIndex+1]);
            mapButtons[2].setText(fileName[currentIndex+2]);
            mapButtons[3].setText(fileName[currentIndex+3]);
        }
    } else {
        if (currentIndex == 0) return;
        currentIndex--;
        mapButtons[0].setText(fileName[currentIndex]);
        mapButtons[1].setText(fileName[currentIndex+1]);
        mapButtons[2].setText(fileName[currentIndex+2]);
        mapButtons[3].setText(fileName[currentIndex+3]);
    }
}