#include "Game/GameLoop.h"
#include "Common/ResourceManager.h"

void GameLoop::run() {
    init();
    ResourceManager::loadResources();
    updateCamera();
    while (!WindowShouldClose()) {
        update();
        draw();
    }
    ResourceManager::unloadResources();
    CloseAudioDevice();
    CloseWindow();
}