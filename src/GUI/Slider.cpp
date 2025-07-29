#include "GUI/Slider.h"
#include "raylib.h"
#include "Common/ResourceManager.h"
#include <algorithm>

Slider::Slider(std::string sliderName, Vector2 position, float width, float minValue, float maxValue, float initialValue, int fontSize)
    : sliderName(sliderName), minValue(minValue), maxValue(maxValue), width(width), dragging(false), fontSize(fontSize)
{
    float clampedValue = std::clamp(initialValue, minValue, maxValue);
    ratio = (maxValue != minValue) ? (clampedValue - minValue) / (maxValue - minValue) : 0.0f;

    track = { position.x, position.y, width, 6 };

    float handleX = position.x + ratio * width - 10;
    float handleY = position.y - 7;
    handle = { handleX, handleY, 20, 20 };
}

void Slider::update() {
    Vector2 mouse = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, handle)) {
        dragging = true;
    }
    if (dragging && !IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        dragging = false;
    }
    if (dragging) {
        float x = std::clamp(mouse.x, track.x, track.x + width);
        ratio = (x - track.x) / width;
        handle.x = x - handle.width / 2;
    }
    else {
        handle.x = track.x + ratio * width - handle.width / 2;
    }
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, track)) {
        ratio = (mouse.x - track.x) / width;
        if(ratio < 0.0f) {
            ratio = 0.0f;
        } else if(ratio > 1.0f) {
            ratio = 1.0f;
        }
        handle.x = track.x + ratio * width - handle.width / 2;
    }
}

void Slider::draw() {
    ResourceManager::drawBigString(sliderName, (int)(track.x - 10 - ResourceManager::getDrawBigStringWidth(sliderName, fontSize)), (int)(track.y) - ResourceManager::getDrawBigStringHeight(fontSize) / 2, fontSize);
    ResourceManager::drawBigNumber((int)(minValue + ratio * (maxValue - minValue) * 100), (int)(track.x + width + 10), (int)(track.y) - ResourceManager::getBigNumberHeight() / 2);
    DrawRectangleRec(track, DARKGRAY);

    Vector2 mouse = GetMousePosition();
    bool hovered = CheckCollisionPointRec(mouse, handle);
    Color handleColor = dragging ? ORANGE : (hovered ? LIGHTGRAY : GRAY);
    DrawRectangleRec(handle, handleColor);

    DrawRectangleLinesEx(handle, 2, BLACK);
}

float Slider::getValue() const {
    return minValue + ratio * (maxValue - minValue);
}

float Slider::getRatio() const {
    return ratio;
}

void Slider::setRatio(float ratio) {
    this->ratio = ratio;
}

Rectangle Slider::getTrackRect() const {
    return track;
}