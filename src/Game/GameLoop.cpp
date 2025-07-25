#include "Game/GameLoop.h"
#include "Common/ResourceManager.h"

void GameLoop::run() {
    init();
    ResourceManager::loadResources();
    updateCamera();
    while (!WindowShouldClose()) {
        update();
        draw();
        if(isClosed) {
            break;
        }
    }
    ResourceManager::unloadResources();
    CloseAudioDevice();
    CloseWindow();
}