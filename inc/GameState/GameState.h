#ifndef GAMESTATE_H
#define GAMESTATE_H

class World;

class GameState {
    protected:
        World* world;
        GameState(World* world);
    public:
        virtual void update() = 0;
        virtual void draw() = 0;
        virtual void enter();
        virtual void exit();
        virtual ~GameState() = default;
};

#endif