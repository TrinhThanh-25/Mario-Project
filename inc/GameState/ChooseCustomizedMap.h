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
    std::string getText() {
        return text;
    }
};

class ChooseCustomizedMapState : public GameState
{
private:
    int numberOfFile;
    std::vector<std::string> fileName;
    std::vector<MyButton> mapButtons;
    std::vector<MyButton> ESCMenuButtons;
    std::vector<MyButton> OptionButtons;
    bool ESCMenuActive = false;
    bool OptionActive = false;
    int MapChosen = 0;

    // Mouse Whell handling
    float MouseWhellAcum = 0;
    float MouseWhellTime = 1.0f;
    int currentIndex;
public:
    ChooseCustomizedMapState(World *world);
    ~ChooseCustomizedMapState() override;

    void update() override;
    void draw() override;

    void setMap(bool Up);
    void loadFromJson(std::string fileName); // Placeholder for loading state from JSON
    void save(std::string fileName);
};


#endif 
