#include "GUI/Slider.h"
#include "raylib.h"
#include "Common/ResourceManager.h"
#include <algorithm>

Slider::Slider(std::string sliderName, Vector2 position, float width, float minValue, float maxValue, float initialValue, int fontSize)
    : sliderName(sliderName), minValue(minValue), maxValue(maxValue), width(width), dragging(false), fontSize(fontSize)
{
    float clampedValue = std::clamp(initialValue, minValue, maxValue);
    ratio = (maxValue != minValue) ? (clampedValue - minValue) / (maxValue - minValue) : 0.0f;

    track = { position.x, position.y, width, 10 };

    float handleX = position.x + ratio * width - 10;
    float handleY = position.y - 1;
    handle = { handleX, handleY, 12, 12 };
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
        dragging = true;
    }
}

void Slider::update(Camera2D* camera) {
    Vector2 mouse = GetScreenToWorld2D(GetMousePosition(), *camera);
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
        dragging = true;
    }
}

void Slider::draw() {
    std::unordered_map<std::string, Texture2D>& textures = ResourceManager::getTexture();
    ResourceManager::drawBigString(sliderName, (int)(track.x - 10 - ResourceManager::getDrawBigStringWidth(sliderName, fontSize)), (int)(track.y) - ResourceManager::getDrawBigStringHeight(fontSize) / 2, fontSize);
    ResourceManager::drawBigNumber((int)(minValue + ratio * (maxValue - minValue) * 100), (int)(track.x + width + 10), (int)(track.y) - ResourceManager::getBigNumberHeight() / 2);
    DrawTexture(textures["GuiSlider"], (int)track.x, (int)track.y, WHITE);
    Vector2 mouse = GetMousePosition();
    bool hovered = CheckCollisionPointRec(mouse, handle);
    Texture2D handleTexture = textures["GuiSliderHandle"];
    if (dragging || hovered) {
        handleTexture = textures["GuiSliderHandleSelected"];
    }
    DrawLineEx({track.x + 4, track.y + 3}, {handle.x + handle.width / 2, track.y + 3}, 2.0f, RED);
    DrawLineEx({track.x + 2, track.y + 5}, {handle.x + handle.width / 2, track.y + 5}, 2.0f, RED);
    DrawLineEx({track.x + 4, track.y + 7}, {handle.x + handle.width / 2, track.y + 7}, 2.0f, RED);
    DrawTexture(handleTexture, (int)handle.x, (int)handle.y, WHITE);
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