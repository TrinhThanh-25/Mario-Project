#ifndef CHARACTERTAG_H
#define CHARACTERTAG_H

#include "raylib.h"
#include "string"
#include "Character/CharacterType.h"
#include "Character/CharacterName.h"
#include "Game/ModeWorld.h"

enum class TagState {
    FIRSTPLAYERSELECTING,
    SECONDPLAYERSELECTING,
    BOTHPLAYERSELECTING,
    FIRSTPLAYERSELECTED,
    SECONDPLAYERSELECTED,
    FIRSTPLAYERSELECTED_SECONDPLAYERSELECTING,
    SECONDPLAYERSECLECTED_FIRSTPLAYERSELECTING,
    BOTHPLAYERSELECTED,
    NOTSELECTED
};

class CharacterTag {
    private:
        Rectangle rec;
        CharacterName name;
        CharacterType type;

        TagState state;
        ModeWorld modeWorld;
    public:
        CharacterTag(CharacterName name, Rectangle rec);
        ~CharacterTag();

        void update();
        void draw();
        void addSelect(TagState state);
        void removeSelect(TagState state);
        void updateState(int isUp);

        CharacterName getName() const;

        void setModeWorld(ModeWorld mode);
        TagState getState() const;
        void selected(TagState state);
};

#endif