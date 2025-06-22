#include "Common/ResourceManager.h"
#include <string>

std::unordered_map<std::string, Texture2D> ResourceManager::texture;
std::unordered_map<std::string, Sound> ResourceManager::sound;
std::unordered_map<std::string, Music> ResourceManager::music;

std::unordered_map<std::string, Texture2D>& ResourceManager::getTexture() {
    return texture;
}

std::unordered_map<std::string, Sound>& ResourceManager::getSound() {
    return sound;
}

std::unordered_map<std::string, Music>& ResourceManager::getMusic() {
    return music;
}

void ResourceManager::loadTexture() {
    // Normal Mario
    texture["SmallMario0Right"] = LoadTexture("resources/Mario/SmallMario_0.png");
    texture["SmallMario0Left"] = texture2DFlipHorizontal(texture["SmallMario0Right"]);
    texture["SmallMario1Right"] = LoadTexture("resources/Mario/SmallMario_1.png");
    texture["SmallMario1Left"] = texture2DFlipHorizontal(texture["SmallMario1Right"]);
    texture["SmallMarioDucking0Right"] = LoadTexture("resources/Mario/SmallMarioDucking_0.png");
    texture["SmallMarioDucking0Left"] = texture2DFlipHorizontal(texture["SmallMarioDucking0Right"]);
    texture["SmallMarioFalling0Right"] = LoadTexture("resources/Mario/SmallMarioFalling_0.png");
    texture["SmallMarioFalling0Left"] = texture2DFlipHorizontal(texture["SmallMarioFalling0Right"]);
    texture["SmallMarioJumping0Right"] = LoadTexture("resources/Mario/SmallMarioJumping_0.png");
    texture["SmallMarioJumping0Left"] = texture2DFlipHorizontal(texture["SmallMarioJumping0Right"]);
    texture["SmallMarioJumpingAndRunning0Right"] = LoadTexture("resources/Mario/SmallMarioJumpingAndRunning_0.png");
    texture["SmallMarioJumpingAndRunning0Left"] = texture2DFlipHorizontal(texture["SmallMarioJumpingAndRunning0Right"]);
    texture["SmallMarioLookingUp0Right"] = LoadTexture("resources/Mario/SmallMarioLookingUp_0.png");
    texture["SmallMarioLookingUp0Left"] = texture2DFlipHorizontal(texture["SmallMarioLookingUp0Right"]);
    texture["SmallMarioRunning0Right"] = LoadTexture("resources/Mario/SmallMarioRunning_0.png");
    texture["SmallMarioRunning0Left"] = texture2DFlipHorizontal(texture["SmallMarioRunning0Right"]);
    texture["SmallMarioRunning1Right"] = LoadTexture("resources/Mario/SmallMarioRunning_1.png");
    texture["SmallMarioRunning1Left"] = texture2DFlipHorizontal(texture["SmallMarioRunning1Right"]);
    texture["SmallMarioDying0"] = LoadTexture("resources/Mario/SmallMarioDying_0.png");
    texture["SmallMarioDying1"] = texture2DFlipHorizontal(texture["SmallMarioDying0"]);
    texture["SmallMarioVictory0"] = LoadTexture("resources/Mario/SmallMarioVictory_0.png");

    //Super Mario
    texture["SuperMario0Right"] = LoadTexture("resources/Mario/SuperMario_0.png");
    texture["SuperMario0Left"] = texture2DFlipHorizontal(texture["SuperMario0Right"]);
    texture["SuperMario1Right"] = LoadTexture("resources/Mario/SuperMario_1.png");
    texture["SuperMario1Left"] = texture2DFlipHorizontal(texture["SuperMario1Right"]);
    texture["SuperMario2Right"] = LoadTexture("resources/Mario/SuperMario_2.png");
    texture["SuperMario2Left"] = texture2DFlipHorizontal(texture["SuperMario2Right"]);
    texture["SuperMarioDucking0Right"] = LoadTexture("resources/Mario/SuperMarioDucking_0.png");
    texture["SuperMarioDucking0Left"] = texture2DFlipHorizontal(texture["SuperMarioDucking0Right"]);
    texture["SuperMarioFalling0Right"] = LoadTexture("resources/Mario/SuperMarioFalling_0.png");
    texture["SuperMarioFalling0Left"] = texture2DFlipHorizontal(texture["SuperMarioFalling0Right"]);
    texture["SuperMarioJumping0Right"] = LoadTexture("resources/Mario/SuperMarioJumping_0.png");
    texture["SuperMarioJumping0Left"] = texture2DFlipHorizontal(texture["SuperMarioJumping0Right"]);
    texture["SuperMarioJumpingAndRunning0Right"] = LoadTexture("resources/Mario/SuperMarioJumpingAndRunning_0.png");
    texture["SuperMarioJumpingAndRunning0Left"] = texture2DFlipHorizontal(texture["SuperMarioJumpingAndRunning0Right"]);
    texture["SuperMarioLookingUp0Right"] = LoadTexture("resources/Mario/SuperMarioLookingUp_0.png");
    texture["SuperMarioLookingUp0Left"] = texture2DFlipHorizontal(texture["SuperMarioLookingUp0Right"]);
    texture["SuperMarioRunning0Right"] = LoadTexture("resources/Mario/SuperMarioRunning_0.png");
    texture["SuperMarioRunning0Left"] = texture2DFlipHorizontal(texture["SuperMarioRunning0Right"]);
    texture["SuperMarioRunning1Right"] = LoadTexture("resources/Mario/SuperMarioRunning_1.png");
    texture["SuperMarioRunning1Left"] = texture2DFlipHorizontal(texture["SuperMarioRunning1Right"]);
    texture["SuperMarioRunning2Right"] = LoadTexture("resources/Mario/SuperMarioRunning_2.png");
    texture["SuperMarioRunning2Left"] = texture2DFlipHorizontal(texture["SuperMarioRunning2Right"]);
    texture["SuperMarioThrowingFireball0Right"] = LoadTexture("resources/Mario/SuperMarioThrowingFireball_0.png");
    texture["SuperMarioThrowingFireball0Left"] = texture2DFlipHorizontal(texture["SuperMarioThrowingFireball0Right"]);
    texture["SuperMarioVictory0"] = LoadTexture("resources/Mario/SuperMarioVictory_0.png");
    texture["TransitioningSuperMario0Right"] = LoadTexture("resources/Mario/TransitioningSuperMario_0.png");
    texture["TransitioningSuperMario0Left"] = texture2DFlipHorizontal(texture["TransitioningSuperMario0Right"]);
    texture["TransitioningSuperMario1Right"] = LoadTexture("resources/Mario/TransitioningSuperMario_1.png");
    texture["TransitioningSuperMario1Left"] = texture2DFlipHorizontal(texture["TransitioningSuperMario1Right"]);
    texture["TransitioningSuperMario2Right"] = LoadTexture("resources/Mario/TransitioningSuperMario_2.png");
    texture["TransitioningSuperMario2Left"] = texture2DFlipHorizontal(texture["TransitioningSuperMario2Right"]);

    //Flower Mario
    texture["FlowerMario0Right"] = LoadTexture("resources/Mario/FlowerMario_0.png");
    texture["FlowerMario0Left"] = texture2DFlipHorizontal(texture["FlowerMario0Right"]);
    texture["FlowerMario1Right"] = LoadTexture("resources/Mario/FlowerMario_1.png");
    texture["FlowerMario1Left"] = texture2DFlipHorizontal(texture["FlowerMario1Right"]);
    texture["FlowerMario2Right"] = LoadTexture("resources/Mario/FlowerMario_2.png");
    texture["FlowerMario2Left"] = texture2DFlipHorizontal(texture["FlowerMario2Right"]);
    texture["FlowerMarioDucking0Right"] = LoadTexture("resources/Mario/FlowerMarioDucking_0.png");
    texture["FlowerMarioDucking0Left"] = texture2DFlipHorizontal(texture["FlowerMarioDucking0Right"]);
    texture["FlowerMarioFalling0Right"] = LoadTexture("resources/Mario/FlowerMarioFalling_0.png");
    texture["FlowerMarioFalling0Left"] = texture2DFlipHorizontal(texture["FlowerMarioFalling0Right"]);
    texture["FlowerMarioJumping0Right"] = LoadTexture("resources/Mario/FlowerMarioJumping_0.png");
    texture["FlowerMarioJumping0Left"] = texture2DFlipHorizontal(texture["FlowerMarioJumping0Right"]);
    texture["FlowerMarioJumpingAndRunning0Right"] = LoadTexture("resources/Mario/FlowerMarioJumpingAndRunning_0.png");
    texture["FlowerMarioJumpingAndRunning0Left"] = texture2DFlipHorizontal(texture["FlowerMarioJumpingAndRunning0Right"]);
    texture["FlowerMarioLookingUp0Right"] = LoadTexture("resources/Mario/FlowerMarioLookingUp_0.png");
    texture["FlowerMarioLookingUp0Left"] = texture2DFlipHorizontal(texture["FlowerMarioLookingUp0Right"]);
    texture["FlowerMarioRunning0Right"] = LoadTexture("resources/Mario/FlowerMarioRunning_0.png");
    texture["FlowerMarioRunning0Left"] = texture2DFlipHorizontal(texture["FlowerMarioRunning0Right"]);
    texture["FlowerMarioRunning1Right"] = LoadTexture("resources/Mario/FlowerMarioRunning_1.png");
    texture["FlowerMarioRunning1Left"] = texture2DFlipHorizontal(texture["FlowerMarioRunning1Right"]);
    texture["FlowerMarioRunning2Right"] = LoadTexture("resources/Mario/FlowerMarioRunning_2.png");
    texture["FlowerMarioRunning2Left"] = texture2DFlipHorizontal(texture["FlowerMarioRunning2Right"]);
    texture["FlowerMarioThrowingFireball0Right"] = LoadTexture("resources/Mario/FlowerMarioThrowingFireball_0.png");
    texture["FlowerMarioThrowingFireball0Left"] = texture2DFlipHorizontal(texture["FlowerMarioThrowingFireball0Right"]);
    texture["FlowerMarioVictory0"] = LoadTexture("resources/Mario/FlowerMarioVictory_0.png");
    texture["TransitioningFlowerMario0Right"] = LoadTexture("resources/Mario/TransitioningFlowerMario_0.png");
    texture["TransitioningFlowerMario0Left"] = texture2DFlipHorizontal(texture["TransitioningFlowerMario0Right"]);
    texture["TransitioningFlowerMario1Right"] = LoadTexture("resources/Mario/TransitioningFlowerMario_1.png");
    texture["TransitioningFlowerMario1Left"] = texture2DFlipHorizontal(texture["TransitioningFlowerMario1Right"]);
    texture["TransitioningFlowerMario2Right"] = LoadTexture("resources/Mario/TransitioningFlowerMario_2.png");
    texture["TransitioningFlowerMario2Left"] = texture2DFlipHorizontal(texture["TransitioningFlowerMario2Right"]);

    //Fireball
    texture["Fireball0Right"] = LoadTexture("resources/Fireball/Fireball_0.png");
    texture["Fireball0Left"] = texture2DFlipHorizontal(texture["Fireball0Right"]);
    texture["Fireball1Right"] = LoadTexture("resources/Fireball/Fireball_1.png");
    texture["Fireball1Left"] = texture2DFlipHorizontal(texture["Fireball1Right"]);
    texture["Fireball2Right"] = LoadTexture("resources/Fireball/Fireball_2.png");
    texture["Fireball2Left"] = texture2DFlipHorizontal(texture["Fireball2Right"]);
    texture["Fireball3Right"] = LoadTexture("resources/Fireball/Fireball_3.png");
    texture["Fireball3Left"] = texture2DFlipHorizontal(texture["Fireball3Right"]);

    //Backgound
    texture["Background1"] = LoadTexture("resources/Background/Background_1.png");
    texture["Background2"] = LoadTexture("resources/Background/Background_2.png");
    texture["Background3"] = LoadTexture("resources/Background/Background_3.png");
    texture["Background4"] = LoadTexture("resources/Background/Background_4.png");
    texture["Background5"] = LoadTexture("resources/Background/Background_5.png");
    texture["Background6"] = LoadTexture("resources/Background/Background_6.png");
    texture["Background7"] = LoadTexture("resources/Background/Background_7.png");
    texture["Background8"] = LoadTexture("resources/Background/Background_8.png");
    texture["Background9"] = LoadTexture("resources/Background/Background_9.png");
    texture["Background10"] = LoadTexture("resources/Background/Background_10.png");

    //Baddie

    //Block
    texture["Cloud0"] = LoadTexture("resources/Block/Cloud_0.png");
    texture["Exclamation0"] = LoadTexture("resources/Block/Exclamation_0.png");
    texture["EyesClosed0"] = LoadTexture("resources/Block/EyesClosed_0.png");
    texture["EyesOpened0"] = LoadTexture("resources/Block/EyesOpened_0.png");
    texture["EyesOpened1"] = LoadTexture("resources/Block/EyesOpened_1.png");
    texture["EyesOpened2"] = LoadTexture("resources/Block/EyesOpened_2.png");
    texture["EyesOpened3"] = LoadTexture("resources/Block/EyesOpened_3.png");
    texture["Glass0"] = LoadTexture("resources/Block/Glass_0.png");
    texture["Message0"] = LoadTexture("resources/Block/Message_0.png");
    texture["Question0"] = LoadTexture("resources/Block/Question_0.png");
    texture["Question1"] = LoadTexture("resources/Block/Question_1.png");
    texture["Question2"] = LoadTexture("resources/Block/Question_2.png");
    texture["Question3"] = LoadTexture("resources/Block/Question_3.png");
    texture["Stone0"] = LoadTexture("resources/Block/Stone_0.png");
    texture["Wood0"] = LoadTexture("resources/Block/Wood_0.png");

    //Effect
    texture["Puft0"] = LoadTexture("resources/Effect/Puft_0.png");
    texture["Puft1"] = LoadTexture("resources/Effect/Puft_1.png");
    texture["Puft2"] = LoadTexture("resources/Effect/Puft_2.png");
    texture["Puft3"] = LoadTexture("resources/Effect/Puft_3.png");
    texture["Start0"] = LoadTexture("resources/Effect/Start_0.png");
    texture["Start1"] = LoadTexture("resources/Effect/Start_1.png");
    texture["Start2"] = LoadTexture("resources/Effect/Start_2.png");
    texture["Start3"] = LoadTexture("resources/Effect/Start_3.png");

    //Item
    texture["Coin0"] = LoadTexture("resources/Item/Coin_0.png");
    texture["Coin1"] = LoadTexture("resources/Item/Coin_1.png");
    texture["Coin2"] = LoadTexture("resources/Item/Coin_2.png");
    texture["Coin3"] = LoadTexture("resources/Item/Coin_3.png");
    

    //Tile
    for (int i=1;i<=4;i++){
        for (char j='A'; j<='Z'; j++){
            std::string key = std::to_string(i) + j;
            texture[key] = LoadTexture(("resources/Tile/" + std::to_string(i) + "/tile_" + j + ".png").c_str());
        }
    }

};

void ResourceManager::loadSound() {
    sound["1Up"] = LoadSound("resources/Sound/1_up.wav");
    sound["BreakBlock"] = LoadSound("resources/Sound/break_block.wav");
    sound["ChuckWhistle"] = LoadSound("resources/Sound/chuck_whistle.wav");
    sound["Coin"] = LoadSound("resources/Sound/coin.wav");
    sound["DragonCoin"] = LoadSound("resources/Sound/dragon_coin.wav");
    sound["Fireball"] = LoadSound("resources/Sound/fireball.wav");
    sound["GoalIrisOut"] = LoadSound("resources/Sound/goal_iris_out.wav");
    sound["Jump"] = LoadSound("resources/Sound/jump.wav");
    sound["Kick"] = LoadSound("resources/Sound/kick.wav");
    sound["MessageBlock"] = LoadSound("resources/Sound/message_block.wav");
    sound["Pause"] = LoadSound("resources/Sound/pause.wav");
    sound["Pipe"] = LoadSound("resources/Sound/pipe.wav");
    sound["PowerUpAppear"] = LoadSound("resources/Sound/power_up_appear.wav");
    sound["PowerUp"] = LoadSound("resources/Sound/power_up.wav");
    sound["ReserveItemRelease"] = LoadSound("resources/Sound/reserve_item_release.wav");
    sound["ReserveItemStore"] = LoadSound("resources/Sound/reserve_item_store.wav");
    sound["RidingYoshi"] = LoadSound("resources/Sound/riding_yoshi.wav");
    sound["ShellRicochet"] = LoadSound("resources/Sound/shell_ricochet.wav");
    sound["StompNoDamage"] = LoadSound("resources/Sound/stomp_no_damage.wav");
    sound["Stomp"] = LoadSound("resources/Sound/stomp.wav");
}

void ResourceManager::loadMusic() {
    // Load music here
}

void ResourceManager::unloadTexture() {
    for (auto& pair : texture) {
        UnloadTexture(pair.second);
    }
    texture.clear();
}

void ResourceManager::unloadSound() {
    for (auto& pair : sound) {
        UnloadSound(pair.second);
    }
    sound.clear();
}

void ResourceManager::unloadMusic() {
    for (auto& pair : music) {
        UnloadMusicStream(pair.second);
    }
    music.clear();
}

Texture2D ResourceManager::texture2DFlipHorizontal(Texture2D texture) {
    Image image = LoadImageFromTexture(texture);
    ImageFlipHorizontal(&image);
    Texture2D flippedTexture = LoadTextureFromImage(image);
    UnloadImage(image);
    return flippedTexture;
}

void ResourceManager::loadResources() {
    loadTexture();
    loadSound();
    loadMusic();
}

void ResourceManager::unloadResources() {
    unloadTexture();
    unloadSound();
    unloadMusic();
}