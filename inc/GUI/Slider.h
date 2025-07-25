#ifndef SLIDER_H
#define SLIDER_H

#include "raylib.h"

class Slider {
public:
    Slider(Vector2 position, float width, float minValue, float maxValue, float initialValue);

    void update();
    void draw();

    float getValue() const;
    float getRatio() const;
    void setRatio(float ratio);
    Rectangle getTrackRect() const;

private:
    float minValue, maxValue;
    float ratio;
    float width;
    bool dragging = false;
    Rectangle handle;
    Rectangle track;
};

#endif