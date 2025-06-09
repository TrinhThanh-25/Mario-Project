#ifndef SPRITE_H
#define SPRITE_H

#include "raylib.h"
#include "Common/Direction.h"
#include "Common/Drawable.h"
#include "Common/SpriteState.h"
#include "Common/CollisionType.h"
#include "Common/CollisionBox.h"

class Sprite : public virtual Drawable {
    protected:
        Vector2 position;
        Vector2 size;
        Vector2 velocity;
        Color color;
        SpriteState state;
        Direction direction;

        float frameTime;
        float frameAcum;
        int curFrame;
        int maxFrame;

        CollisionBox north;
        CollisionBox south;
        CollisionBox west;
        CollisionBox east;
    public:
        Sprite();
        Sprite(Vector2 positon, Vector2 size, Vector2 vel, Color color, float frameTime, int maxFrames, Direction facingDirection);
        Sprite(Vector2 positon, Vector2 size, Color color);
        virtual ~Sprite() override;

        virtual void update() = 0;
        void draw() override = 0;
        virtual CollisionType checkCollision(Sprite* sprite) const;
        virtual void updateCollisionBoxes();

        void setPosition(Vector2 position);
        void setPosition(float x, float y);
        void setX(float x);
        void setY(float y);

        void setSize(Vector2 size);
        void setSize(float width, float height);
        void setWidth(float width);
        void setHeight(float height);

        void setVelocity(Vector2 velocity);
        void setVelocity(float x, float y);
        void setVelocityX(float x);
        void setVelocityY(float y);

        void setColor(Color color);
        void setState(SpriteState state);
        void setDirection(Direction direction);

        Vector2 getPosition() const;
        float getX() const;
        float getY() const;

        Vector2 getSize() const;
        float getWidth() const;
        float getHeight() const;

        Vector2 getVelocity() const;
        float getVelocityX() const;
        float getVelocityY() const;

        Color getColor() const;
        SpriteState getState() const;
        Direction getDirection() const;

        Rectangle getRect() const;
};

#endif