#include "GameState/ChooseMap.h"
#include "GameState/TitleScreenState.h"

void MyButton::draw()
{
    if (!isDrawn) return;
        DrawTexture(ResourceManager::getTexture()[textureName+"0"], rectangle.x, rectangle.y, color);
    if (isHovered && isHandled)
    {
        DrawTexture(ResourceManager::getTexture()[textureName+"1"], rectangle.x, rectangle.y, color);
    }
    // Draw the button text
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

ChooseMapState::ChooseMapState(World *world, GameStateType type) : GameState(world, type)
{
    mapButtons.emplace_back(Rectangle{222, 301, 355, 67}, "Map 1", 20, WHITE, true, true);
    mapButtons.emplace_back(Rectangle{1022, 301, 355, 67}, "Map 2", 20, WHITE, true, true);
    mapButtons.emplace_back(Rectangle{222, 751, 355, 67}, "Map 3", 20, WHITE, true, true);
    mapButtons.emplace_back(Rectangle{1022, 751, 355, 67}, "Customized maps", 20, WHITE, true, true);
    for (int i=0;i<=3;i++) {
        mapButtons[i].setTextureName("ChooseMapButton");
    }
    // ESC menu button
    mapButtons.emplace_back(Rectangle{577, 288, 442, 94}, "Resume", 20, WHITE, false, false);
    mapButtons.emplace_back(Rectangle{577, 432, 442, 94}, "Exit to Main Menu", 20, WHITE, false, false);
    mapButtons.emplace_back(Rectangle{577, 576, 442, 94}, "Exit to Desktop", 20, WHITE, false, false);
    for (int i=4;i<=6;i++) {
        mapButtons[i].setTextureName("ESCMenuButton");
    }
}

ChooseMapState::~ChooseMapState()
{
    mapButtons.clear();
}

void ChooseMapState::update()
{
    for (auto &button : mapButtons)
    {
        button.update();
    }
    if (ECSMenuActive)
    {
        if (IsKeyPressed(KEY_ESCAPE))
        {
            ECSMenuActive = !ECSMenuActive;
            for (int i = 0; i <= 3; i++)
            {
                mapButtons[i].toggleHandle();
            }
            for (int i = 4; i <= 6; i++)
            {
                mapButtons[i].toggleDraw();
                mapButtons[i].toggleHandle();
            }
        }
        else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (mapButtons[4].isHover())
            {
                ECSMenuActive = !ECSMenuActive;
            for (int i = 0; i <= 3; i++)
            {
                mapButtons[i].toggleHandle();
            }
            for (int i = 4; i <= 6; i++)
            {
                mapButtons[i].toggleDraw();
                mapButtons[i].toggleHandle();
            }
            }
            else if (mapButtons[5].isHover())
            {
                world->setGameState(new TitleScreenState(world));
            }
            else if (mapButtons[6].isHover())
            {
                world->setIsClosed(true);
            }
        }
    }
    else
    {
        if (IsKeyPressed(KEY_ESCAPE))
        {
            ECSMenuActive = !ECSMenuActive;

            for (int i = 0; i <= 3; i++)
            {
                mapButtons[i].toggleHandle();
            }
            for (int i = 4; i <= 6; i++)
            {
                mapButtons[i].toggleDraw();
                mapButtons[i].toggleHandle();
            }
        }
        else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (mapButtons[0].isHover())
            {
                // Map 1
            }
            else if (mapButtons[1].isHover())
            {
                // Map 2
            }
            else if (mapButtons[2].isHover())
            {
                // Map 3
            }
            else if (mapButtons[3].isHover())
            {
                GameState* newState = new ChooseCustomizedMapState(world,GameStateType::CHOOSE_CUSTOMIZED_MAP);
                world->setGameState(newState);
            }
        }
    }
}

void ChooseMapState::draw()
{
    ClearBackground(WHITE);
    for (int i = 0; i <= 3; i++)
    {
        mapButtons[i].draw();
    }
    if (ECSMenuActive)
    {
        DrawRectangle(0, 0, 1600, 900, Fade({159, 139, 121, 255}, 0.5f));
        DrawTexture(ResourceManager::getTexture()["ESCMenuPad"],444,170,WHITE);
        for (int i = 4; i <= 6; i++)
        {
            mapButtons[i].draw();
        }
    }
}
