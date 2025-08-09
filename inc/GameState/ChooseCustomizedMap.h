#ifndef CHOOSE_CUSTOMIZED_MAP_H
#define CHOOSE_CUSTOMIZED_MAP_H

#include "GameState/GameState.h"
#include "GameState/GameStateType.h"
#include "raylib.h"
#include <vector>
#include <unordered_set>

class MyButton {
private:
    Rectangle rectangle;
    bool isHovered = false;
    bool wasHovered = false;
    std::string text;
    int fontSize = 0;
    bool isHandled = false;
    bool isDrawn = false;
    Color color;
    std::string textureName = "";
public:
    MyButton(Rectangle rectangle, std::string text, int fontSize, Color color, bool isHandled = true, bool isDrawn = true);
    void draw();
    void update();
    void toggleHandle();
    bool isHandle() const;
    bool isHover() const;
    bool isDraw() const;
    void toggleDraw();
    void setTextureName(std::string name);
    void setColor(Color new_color);
    void setText(const std::string &new_text);
    void setPos(Vector2 newPos);
    std::string getText();
    bool isClicked();
};

class ChooseCustomizedMapState : public GameState
{
private:
    int numberOfFile;
    std::vector<std::string> listFileName;
    std::unordered_set<std::string> mapNameSet;
    std::vector<MyButton> mapButtons;
    std::vector<MyButton> OptionButtons;
    bool OptionActive = false;
    int MapChosen = 0;

    float MouseWhellAcum = 0;
    float MouseWhellTime = 1.0f;
    int currentIndex;
public:
    ChooseCustomizedMapState(World *world);
    ~ChooseCustomizedMapState() override;

    void update() override;
    void draw() override;

    void setMap(bool Up);
    void load(std::string fileName = "../resources/Map/ListMap.json");
    void save(std::string fileName = "../resources/Map/ListMap.json");
    void createDefaultMapFile(const std::string& mapName);
};


#endif 
