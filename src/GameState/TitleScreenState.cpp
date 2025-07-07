#include "GameState/TitleScreenState.h"
#include "GameState/PlayingState.h"
#include "Common/ResourceManager.h"
#include "raylib.h"

TitleScreenState::TitleScreenState(World* world)
    : GameState(world) {
}

TitleScreenState::~TitleScreenState() {

}

void TitleScreenState::update() {
    if(!IsMusicStreamPlaying(ResourceManager::getMusic()["Title"])) {
        PlayMusicStream(ResourceManager::getMusic()["Title"]);
    }
    else {
        UpdateMusicStream(ResourceManager::getMusic()["Title"]);
    }
    if(IsKeyPressed(KEY_ENTER)) {
        StopMusicStream(ResourceManager::getMusic()["Title"]);
        world->setGameState(new PlayingState(world));
    }
    // if press new game -> ChooseCharacterState 
    // if press continue -> PlayingState (if there is a saved game)
    // if press exit -> CloseWindow()
}

void TitleScreenState::draw() {
    ClearBackground(RAYWHITE);
    std::unordered_map<std::string, Texture2D>& texture = ResourceManager::getTexture();
    DrawTexture(texture["Logo"], GetScreenWidth() / 2 - texture["Logo"].width / 2, GetScreenHeight() / 2 - texture["Logo"].height / 2 - 200, WHITE);
    // draw title screen background
    // draw buttons for new game, continue, exit (normal, hovered, pressed)
}