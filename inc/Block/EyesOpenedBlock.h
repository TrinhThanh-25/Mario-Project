#ifndef EYESOPENEDBLOCK_H
#define EYESOPENEDBLOCK_H

#include "raylib.h"
#include "Block/Block.h"
#include "Game/Map.h"

class EyesOpenedBlock : public Block {
private:
	float animationTime;
	float animationAccumulated;
public:

    EyesOpenedBlock( Vector2 pos, Vector2 dim, Color color );
    EyesOpenedBlock( Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames );
    ~EyesOpenedBlock() override;

    void update() override;
    void draw() override;
	void doHit(Character& character, Map* map) override;

    json saveToJson() const override;
    void loadFromJson(const json& j) override;
};
#endif