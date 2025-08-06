#ifndef CHOOSE_CUSTOMIZED_MAP_H
#define CHOOSE_CUSTOMIZED_MAP_H

#include "Game/World.h"
#include "Common/ResourceManager.h"
#include "GameState/ChooseMap.h"
#include <vector>

class MyButton;

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
    ChooseCustomizedMapState(World *world, GameStateType type);
    ~ChooseCustomizedMapState() override;

    void update() override;
    void draw() override;

    void loadFromJson(const json& j) override; // Placeholder for loading state from JSON
};


#endif // CHOOSE_CUSTOMIZED_MAP_H
