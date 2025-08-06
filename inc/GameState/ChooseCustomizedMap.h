#ifndef CHOOSE_CUSTOMIZED_MAP_H
#define CHOOSE_CUSTOMIZED_MAP_H

#include "GameState/GameState.h"
#include "GameState/GameStateType.h"
#include "raylib.h"
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
class popUpMenu {
private:
    std::vector<MyButton> popupMenuButtons;
    Rectangle rectangle;
    bool isHandled = false;
    bool isHovered = false;
    bool Upper = false;
    int mapChosen = 0;
public:
    popUpMenu();
    ~popUpMenu();

    void update();
    void draw();

    void setPos(Vector2 newPos);
    bool isHandle() const {
        return isHandled;
    }
    bool isHover() const {
        return isHovered;
    }
    void setMapChosen(int i) {
        mapChosen = i;
    }
};

class ChooseCustomizedMapState : public GameState
{
private:
    popUpMenu popUp;
    std::vector<MyButton> mapButtons;
    std::vector<MyButton> ESCMenuButtons;
    bool ESCMenuActive = false;
    bool PopUpActive = false;
public:
    ChooseCustomizedMapState(World *world);
    ~ChooseCustomizedMapState() override;

    void update() override;
    void draw() override;

    void loadFromJson(const json& j) override; // Placeholder for loading state from JSON
};


#endif 
