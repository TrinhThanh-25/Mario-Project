#ifndef ITEM_H
#define ITEM_H

#include "Common/Sprite.h"
#include "Character/Character.h"
#include "raylib.h"
#include "Block/Block.h"
#include "Tile/Tile.h"
#include "Common/ResourceManager.h"
#include "GUI/GameHud.h"
#include "../json.hpp"

using json = nlohmann::json;

enum class ItemType {
    COIN,
    MUSHROOM,
    FLOWER,
    STAR,
    YOSHI_COIN,
    THREE_UP_MOON,
    ONE_UP_MUSHROOM,
    COURSE_CLEAR_TOKEN
};

class Item : public Sprite{
protected:
    float beingHitFrameTime;
    float beingHitFrameAcum;
    int maxBeingHitFrame;
    int currentBeingHitFrame;
    float pointFrameAccum;
    float pointFrameTime;
    bool pauseGameWhenHit;
    ItemType type;
public:
    Item(Vector2 position, Vector2 size, Vector2 vel, Color color, float FrameTime, int MaxFrame, Direction direction,float HitFrameTime, int maxHitFrame, bool pause);
    ~Item() override;
    
    void update() override;
        virtual void updateWhenActive(float timeElapsed) = 0;
        virtual void updateWhenHit(float timeElapsed) = 0;
    void draw() override;

    virtual void playCollisionSound() = 0;
    virtual void updateCharacter(Character* character) = 0;
    virtual void collisionSouth(Character* character) = 0;
    bool isPausedGameWhenBeingHit();

    void collisionBlock(Block* block);
    void collisionTile(Tile* tile);
    void collisionCharacter(Character* character);
    ItemType getType();

    json saveToJson() const override;
    void loadFromJson(const json& j) override;
};

#endif