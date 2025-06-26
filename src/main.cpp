#include <iostream>
#include "Game/World.h"
#include "raylib.h"

int main() {
    World world(1600, 900, "Super Mario", 60);
    world.run();
    return 0;
}