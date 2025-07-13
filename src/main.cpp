#include "Game/World.h"

int main() {
    World world(1600, 900, "My Game World", 60);
    world.run();
}