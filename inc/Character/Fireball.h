#ifndef FIREBALL_H
#define FIREBALL_H

#include "Common/Sprite.h"
#include "Common/Direction.h"
#include "raylib.h"

class Fireball : virtual public Sprite {
    private:
        float fireAcum;
        float fireTime;
    public:
        Fireball(Vector2 pos, Direction Direction, float fireTime);
        ~Fireball() override;
        void update() override;
        void draw() override;
    private:
        void updateFrame();
        void updatePosition();
        void updateVelocity();
        void updateState();

        json saveToJson() const override;
        void loadFromJson(const json& j) override;
};

#endif