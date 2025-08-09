#ifndef BUTTON_H
#define BUTTON_H

#include "raylib.h"

class Button{
    private:
        Rectangle rectangle;

        bool selected = false;
        const char* text = nullptr;
        int fontSize=0;
        bool isPlayHoverSound = false;
    public:
        Button();
        Button(Rectangle rectangle, const char* text, int fontSize);
        void setPosition(Vector2 position);
        void setSize(Vector2 size);
        void setText(const char* text, int fontSize);

        void update();
        void update(Camera2D* camera);
        void draw();

        Vector2 getSize();
        int getfontSize();
        Vector2 getPosition();

        bool isPressed();
        bool isHovered();
        // Overloaded versions for camera transformation
        bool isPressed(Camera2D* camera);
        bool isHovered(Camera2D* camera);
        bool isSelected();
        void Selected();
        void deSelected();
};

#endif