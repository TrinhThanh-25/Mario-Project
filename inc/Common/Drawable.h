#ifndef DRAWABLE_H
#define DRAWABLE_H

class Drawable {
    protected:
        virtual ~Drawable() = default;
        virtual void draw() = 0;
};

#endif