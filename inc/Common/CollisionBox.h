#ifndef COLLISIONBOX_H
#define COLLISIONBOX_H

#include "Common/Drawable.h"
#include "raylib.h"

class CollisionBox : public virtual Drawable {
    protected:
        Vector2 position;
        Vector2 size;
        Rectangle rect;
        Color color;

    public:
        CollisionBox();
        CollisionBox(Vector2 position, Vector2 size, Color color = WHITE);

        bool checkCollision(const Rectangle& other) const;
        void draw() override;

        void setPosition(Vector2 position);
        void setPosition(float x, float y);
        void setX(float x);
        void setY(float y);

        void setSize(Vector2 size);
        void setSize(float width, float height);
        void setWidth(float width);
        void setHeight(float height);

        void setColor(Color color);

        Vector2 getPosition() const;
        float getX() const;
        float getY() const;

        Vector2 getSize() const;
        float getWidth() const;
        float getHeight() const;

        Color getColor() const;
        
        Rectangle getRect() const;
};

#endif