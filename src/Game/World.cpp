#include "Game/World.h"
#include "Common/ResourceManager.h"

float World::gravity = 800.0f;

World::World(int width, int height, const std::string& title, int FPS)
    : width(width), height(height), title(title), FPS(FPS) {
}

World::~World() {
}

void World::init() {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
        // SetConfigFlags( FLAG_FULLSCREEN_MODE );
        // SetConfigFlags( FLAG_WINDOW_UNDECORATED );
    SetConfigFlags(FLAG_WINDOW_ALWAYS_RUN);
    InitWindow(width, height, title.c_str());
    SetWindowIcon(LoadImage("resources/icon.png"));
    SetExitKey(0);
    InitAudioDevice();
    SetTargetFPS(FPS);
    ResourceManager::loadResources();
    
    camera.target = {0,0};
    camera.offset = {(float)GetScreenWidth()/2.0f, (float)GetScreenHeight() - 104};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    setCamera(&camera);

    while (!WindowShouldClose()) {
        update();
        draw();
    }

    ResourceManager::unloadResources();
    CloseAudioDevice();
    CloseWindow();

}

void World::update() {}

void World::draw() {}

void World::setCamera(Camera2D *camera) {
    this->camera = *camera;
}

const Camera2D* World::getCamera() const {
    return &camera;
}