#ifndef SLIDER_H
#define SLIDER_H

#include "raylib.h"
#include <string>

class Slider {
    private:
        std::string sliderName;
        float minValue, maxValue;
        float ratio;
        float width;
        bool dragging = false;
        Rectangle handle;
        Rectangle track;
        int fontSize = 20;
    public:
        Slider(std::string sliderName, Vector2 position, float width, float minValue, float maxValue, float initialValue, int fontSize = 20);

        void update();
        void update(Camera2D* camera);
        void draw();

        float getValue() const;
        float getRatio() const;
        void setRatio(float ratio);
        Rectangle getTrackRect() const;

};

#endif