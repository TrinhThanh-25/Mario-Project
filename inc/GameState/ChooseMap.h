#ifndef CHOOSE_MAP_H
#define CHOOSE_MAP_H

#include "GameState/GameState.h"
#include "Game/World.h"
#include "Common/ResourceManager.h"
#include "ChooseCustomizedMap.h"
#include <vector>

class MyButton
{
private:
    Rectangle rectangle;
    bool isHovered = false;
    std::string text;
    int fontSize = 0;
    bool isHandled = false;
    bool isDrawn = false;
    Color color;
    std::string textureName = "";
public:
    MyButton(Rectangle rect, const std::string &txt, int size, Color nColor, bool handle, bool drawn = true)
        : rectangle(rect), text(txt), fontSize(size), isHandled(handle), isDrawn(drawn), color(nColor) {}
    void draw();
    void update();
    void toggleHandle() { isHandled = !isHandled; }
    bool isHandle() const { return isHandled; }
    bool isHover() const { return isHovered; }
    bool isDraw() const { return isDrawn; }
    void toggleDraw() { isDrawn = !isDrawn; }
    void setTextureName(std::string name) {
        textureName = name;
    }
    void setColor(Color new_color)
    {
        color = new_color;
    }
    void setText(const std::string &new_text)
    {
        text = new_text;
    }
    void setPos(Vector2 newPos);
};

class ChooseMapState : public GameState
{
private:
    std::vector<MyButton> mapButtons;
    bool ECSMenuActive = false;
public:
    ChooseMapState(World *world, GameStateType type);
    ~ChooseMapState() override;

    void update() override;
    void draw() override;
};

#endif // CHOOSE_MAP_H
