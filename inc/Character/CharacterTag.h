#ifndef CHARACTERTAG_H
#define CHARACTERTAG_H

#include "raylib.h"
#include "string"
#include "Character/CharacterType.h"

enum class CharacterName {
    MARIO,
    LUIGI
};

class CharacterTag {
    private:
        CharacterName name;
        CharacterType type;
        Rectangle rec;
        KeyboardKey left;
        KeyboardKey right;
        KeyboardKey up;
        KeyboardKey down;
    public:
        CharacterTag(CharacterName name, Rectangle rec);
        ~CharacterTag();

        void update();
        void draw();

        void setKeys(KeyboardKey left, KeyboardKey right, KeyboardKey up, KeyboardKey down);
        CharacterName getName() const;
};

#endif