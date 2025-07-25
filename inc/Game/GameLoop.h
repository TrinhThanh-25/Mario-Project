#ifndef GAMELOOP_H
#define GAMELOOP_H

#include "Common/Drawable.h"

class GameLoop : virtual public Drawable {
    protected:
        bool isClosed;
        GameLoop() : isClosed(false) {}
    public:
        void run();
        virtual void init() = 0;
        virtual void update() = 0;
        virtual void draw() = 0;
        virtual void updateCamera() = 0;
        virtual ~GameLoop() = default;
};

#endif