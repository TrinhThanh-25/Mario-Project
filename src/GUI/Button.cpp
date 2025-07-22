#include "Common/ResourceManager.h"
#include <GUI/Button.h>

Button::Button(){}

Button::Button(Rectangle rectangle, const char* text, int fontSize) :
    rectangle(rectangle), text(text), fontSize(fontSize) {
}

void Button::setPosition(Vector2 position){
    this->rectangle.x = position.x;
    this->rectangle.y = position.y;
}

void Button::setSize(Vector2 size){
    this->rectangle.width = size.x;
    this->rectangle.height = size.y;
}

void Button::setText(const char* text, int fontSize){
    this->text = text;
    this->fontSize=fontSize;
}

void Button::update(){
    //
}

void Button::draw(){
    std::unordered_map<std::string, Texture2D>& texture = ResourceManager::getTexture();
    if (this->text) {
        ResourceManager::drawBigString(this->text, this->rectangle.x + (this->rectangle.width - ResourceManager::getDrawBigStringWidth(this->text, fontSize)) / 2, this->rectangle.y + (this->rectangle.height - ResourceManager::getDrawBigStringHeight(fontSize)) / 2, fontSize);
    }
    if(selected) {
        DrawTexture(texture["GuiArrowLeft"], this->rectangle.x -texture["GuiArrowLeft"].width, this->rectangle.y + (this->rectangle.height - texture["GuiArrowLeft"].height) / 2, WHITE);
        DrawTexture(texture["GuiArrowRight"], this->rectangle.x + this->rectangle.width, this->rectangle.y + (this->rectangle.height - texture["GuiArrowRight"].height) / 2, WHITE);
    }
}

Vector2 Button::getSize(){
    return {this->rectangle.width, this->rectangle.height};
}

Vector2 Button::getPosition(){
return {this->rectangle.x, this->rectangle.y};
}

int Button::getfontSize(){
    return this->fontSize;
}

bool Button::isPressed(){
    return IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), rectangle);
}

bool Button::isHovered(){
    return CheckCollisionPointRec(GetMousePosition(), rectangle);
}

bool Button::isSelected(){
    return this->selected;
}

void Button::Selected(){
    this->selected = true;
}

void Button::deSelected(){
    this->selected = false;
}