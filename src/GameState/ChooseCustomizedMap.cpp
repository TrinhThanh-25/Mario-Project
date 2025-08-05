#include "GameState/ChooseCustomizedMap.h"

ChooseCustomizedMapState::ChooseCustomizedMapState(World *world, GameStateType type) : GameState(world, type)
{
    // Map buttons initialization
    mapButtons.emplace_back(Rectangle{219, 64, 1160, 173}, "Map 1", 20, WHITE, true, true);
    mapButtons.emplace_back(Rectangle{222, 264, 1160, 173}, "Map 2", 20, WHITE, true, true);
    mapButtons.emplace_back(Rectangle{222, 464, 1160, 173}, "Map 3", 20, WHITE, true, true);
    mapButtons.emplace_back(Rectangle{222, 664, 1160, 173}, "Map 4", 20, WHITE, true, true);

    for (int i = 0; i < mapButtons.size(); i++)
    {
        mapButtons[i].setTextureName("CMButton");
    }

    // ESC menu buttons initialization
    ESCMenuButtons.emplace_back(Rectangle{577, 288, 442, 94}, "Resume", 20, WHITE, true, true);
    ESCMenuButtons.emplace_back(Rectangle{577, 432, 442, 94}, "Exit to Map choosing", 20, WHITE, true, true);
    ESCMenuButtons.emplace_back(Rectangle{577, 576, 442, 94}, "Exit to Desktop", 20, WHITE, true, true);

    for (int i = 0; i < ESCMenuButtons.size(); i++)
    {
        ESCMenuButtons[i].setTextureName("ESCMenuButton");
    }

    ESCMenuActive = false;
    PopUpActive = false;
}

ChooseCustomizedMapState::~ChooseCustomizedMapState()
{
    mapButtons.clear();
    ESCMenuButtons.clear();
}
void ChooseCustomizedMapState::update()
{
    if (ESCMenuActive)
    {
        if (IsKeyPressed(KEY_ESCAPE))
        {
            ESCMenuActive = false;
            for (int i = 0; i < mapButtons.size(); i++)
            {
                mapButtons[i].toggleHandle();
            }
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
                for (int i = 0; i < mapButtons.size(); i++)
                {
                    mapButtons[i].toggleHandle();
                }
                return;
            }
            else if (ESCMenuButtons[1].isHover())
            {
                world->setGameState(new ChooseMapState(world,GameStateType::CHOOSE_MAP));
            }
            else if (ESCMenuButtons[2].isHover())
            {
                world->setIsClosed(true);
            }
        }
    }
    else
    {
        if (IsKeyPressed(KEY_ESCAPE))
        {
            ESCMenuActive = !ESCMenuActive;
            for (int i = 0; i < mapButtons.size(); i++)
            {
                mapButtons[i].toggleHandle();
            }
            return;
        }
        if (PopUpActive)
        {
            popUp.update();
            if (!popUp.isHover())
            {
                PopUpActive = false;
            }
            return;
        }
        else
        {
            for (auto &button : mapButtons)
            {
                button.update();
            }
            for (int i = 0; i < mapButtons.size(); i++)
            {
                if (mapButtons[i].isHover())
                {
                    PopUpActive = true;
                    popUp.setMapChosen(i + 1);
                    popUp.setPos(GetMousePosition());
                }
            }
        }
    }
}

void ChooseCustomizedMapState::draw()
{
    ClearBackground(WHITE);
    DrawTexture(ResourceManager::getTexture()["CMPad"], 124, 9, WHITE);
    // Draw buttons for each map
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
    }
    else if (PopUpActive)
    {
        popUp.draw();
    }
}

void ChooseCustomizedMapState::loadFromJson(const json &j)
{
}
popUpMenu::popUpMenu(): rectangle({0,0,682,380})
{
    popupMenuButtons.emplace_back(Rectangle{0, 0, 495, 156}, "Play", 20, WHITE, true, true);
    popupMenuButtons.emplace_back(Rectangle{0, 0, 495, 156}, "Edit", 20, WHITE, true, true);
    for (int i=0;i<popupMenuButtons.size();i++) {
        popupMenuButtons[i].setTextureName("PUButton");
    }
}

popUpMenu::~popUpMenu()
{
    popupMenuButtons.clear();
}
void popUpMenu::draw()
{
    if (Upper) {
        DrawTexture(ResourceManager::getTexture()["PUPad"],rectangle.x,rectangle.y,WHITE);
    } else {
        DrawTexture(ResourceManager::getTexture()["PUPad1"],rectangle.x,rectangle.y,WHITE);
    }
    popupMenuButtons[0].draw();
    popupMenuButtons[1].draw();
}

void popUpMenu::setPos(Vector2 newPos)
{
    if (newPos.y < 450) {
        rectangle.x = newPos.x;
        rectangle.y = newPos.y;
        popupMenuButtons[0].setPos({rectangle.x+122,rectangle.y+26});
        popupMenuButtons[1].setPos({rectangle.x+122,rectangle.y+202});
        Upper = true;
    } else {
        rectangle.x = newPos.x;
        rectangle.y = newPos.y-rectangle.height;
        popupMenuButtons[0].setPos({rectangle.x+122,rectangle.y+26});
        popupMenuButtons[1].setPos({rectangle.x+122,rectangle.y+202});
        Upper = false;
    }
}

void popUpMenu::update()
{
    Vector2 mousePos = GetMousePosition();
    if (mousePos.y < 450) {
        isHovered = CheckCollisionPointRec(mousePos, Rectangle{rectangle.x-50,rectangle.y-50,rectangle.width+100,rectangle.height+100});
    } else {
        isHovered = CheckCollisionPointRec(mousePos, Rectangle{rectangle.x-50,rectangle.y+50,rectangle.width+100,rectangle.height+50});
    }
    for (int i = 0; i < popupMenuButtons.size(); i++)
    {
        popupMenuButtons[i].update();
    }
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (popupMenuButtons[0].isHover())
        {
            // Play map chosen
        }
        else if (popupMenuButtons[1].isHover())
        {
            // Edit map chosen
        }
    }
}