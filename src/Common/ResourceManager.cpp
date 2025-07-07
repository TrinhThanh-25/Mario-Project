#include "Common/ResourceManager.h"
#include "raylib.h"
#include <string>
#include <sstream>
#include <iostream>
#include <vector>


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
    texture["Logo"] = LoadTexture("../resources/logo.png");

    // Tags
    texture["SmallMarioTag"] = LoadTexture("../resources/Mario/SmallMario.png");
    texture["SuperMarioTag"] = LoadTexture("../resources/Mario/SuperMario.png");
    texture["FlowerMarioTag"] = LoadTexture("../resources/Mario/FlowerMario.png");
    texture["SmallLuigiTag"] = LoadTexture("../resources/Luigi/SmallLuigi.png");
    texture["SuperLuigiTag"] = LoadTexture("../resources/Luigi/SuperLuigi.png");
    texture["FlowerLuigiTag"] = LoadTexture("../resources/Luigi/FlowerLuigi.png");
    texture["MarioNameTag"] = LoadTexture("../resources/Mario/MarioNameTag.png");
    texture["LuigiNameTag"] = LoadTexture("../resources/Luigi/LuigiNameTag.png");

    // Small Mario
    texture["SmallMario0Right"] = LoadTexture("../resources/Mario/SmallMario_0.png");
    texture["SmallMario0Left"] = texture2DFlipHorizontal(texture["SmallMario0Right"]);
    texture["SmallMario1Right"] = LoadTexture("../resources/Mario/SmallMario_1.png");
    texture["SmallMario1Left"] = texture2DFlipHorizontal(texture["SmallMario1Right"]);
    texture["SmallMarioDucking0Right"] = LoadTexture("../resources/Mario/SmallMarioDucking_0.png");
    texture["SmallMarioDucking0Left"] = texture2DFlipHorizontal(texture["SmallMarioDucking0Right"]);
    texture["SmallMarioFalling0Right"] = LoadTexture("../resources/Mario/SmallMarioFalling_0.png");
    texture["SmallMarioFalling0Left"] = texture2DFlipHorizontal(texture["SmallMarioFalling0Right"]);
    texture["SmallMarioJumping0Right"] = LoadTexture("../resources/Mario/SmallMarioJumping_0.png");
    texture["SmallMarioJumping0Left"] = texture2DFlipHorizontal(texture["SmallMarioJumping0Right"]);
    texture["SmallMarioJumpingAndRunning0Right"] = LoadTexture("../resources/Mario/SmallMarioJumpingAndRunning_0.png");
    texture["SmallMarioJumpingAndRunning0Left"] = texture2DFlipHorizontal(texture["SmallMarioJumpingAndRunning0Right"]);
    texture["SmallMarioLookingUp0Right"] = LoadTexture("../resources/Mario/SmallMarioLookingUp_0.png");
    texture["SmallMarioLookingUp0Left"] = texture2DFlipHorizontal(texture["SmallMarioLookingUp0Right"]);
    texture["SmallMarioRunning0Right"] = LoadTexture("../resources/Mario/SmallMarioRunning_0.png");
    texture["SmallMarioRunning0Left"] = texture2DFlipHorizontal(texture["SmallMarioRunning0Right"]);
    texture["SmallMarioRunning1Right"] = LoadTexture("../resources/Mario/SmallMarioRunning_1.png");
    texture["SmallMarioRunning1Left"] = texture2DFlipHorizontal(texture["SmallMarioRunning1Right"]);
    texture["SmallMarioDying0"] = LoadTexture("../resources/Mario/SmallMarioDying_0.png");
    texture["SmallMarioDying1"] = texture2DFlipHorizontal(texture["SmallMarioDying0"]);
    texture["SmallMarioVictory0"] = LoadTexture("../resources/Mario/SmallMarioVictory_0.png");

    //Super Mario
    texture["SuperMario0Right"] = LoadTexture("../resources/Mario/SuperMario_0.png");
    texture["SuperMario0Left"] = texture2DFlipHorizontal(texture["SuperMario0Right"]);
    texture["SuperMario1Right"] = LoadTexture("../resources/Mario/SuperMario_1.png");
    texture["SuperMario1Left"] = texture2DFlipHorizontal(texture["SuperMario1Right"]);
    texture["SuperMario2Right"] = LoadTexture("../resources/Mario/SuperMario_2.png");
    texture["SuperMario2Left"] = texture2DFlipHorizontal(texture["SuperMario2Right"]);
    texture["SuperMarioDucking0Right"] = LoadTexture("../resources/Mario/SuperMarioDucking_0.png");
    texture["SuperMarioDucking0Left"] = texture2DFlipHorizontal(texture["SuperMarioDucking0Right"]);
    texture["SuperMarioFalling0Right"] = LoadTexture("../resources/Mario/SuperMarioFalling_0.png");
    texture["SuperMarioFalling0Left"] = texture2DFlipHorizontal(texture["SuperMarioFalling0Right"]);
    texture["SuperMarioJumping0Right"] = LoadTexture("../resources/Mario/SuperMarioJumping_0.png");
    texture["SuperMarioJumping0Left"] = texture2DFlipHorizontal(texture["SuperMarioJumping0Right"]);
    texture["SuperMarioJumpingAndRunning0Right"] = LoadTexture("../resources/Mario/SuperMarioJumpingAndRunning_0.png");
    texture["SuperMarioJumpingAndRunning0Left"] = texture2DFlipHorizontal(texture["SuperMarioJumpingAndRunning0Right"]);
    texture["SuperMarioLookingUp0Right"] = LoadTexture("../resources/Mario/SuperMarioLookingUp_0.png");
    texture["SuperMarioLookingUp0Left"] = texture2DFlipHorizontal(texture["SuperMarioLookingUp0Right"]);
    texture["SuperMarioRunning0Right"] = LoadTexture("../resources/Mario/SuperMarioRunning_0.png");
    texture["SuperMarioRunning0Left"] = texture2DFlipHorizontal(texture["SuperMarioRunning0Right"]);
    texture["SuperMarioRunning1Right"] = LoadTexture("../resources/Mario/SuperMarioRunning_1.png");
    texture["SuperMarioRunning1Left"] = texture2DFlipHorizontal(texture["SuperMarioRunning1Right"]);
    texture["SuperMarioRunning2Right"] = LoadTexture("../resources/Mario/SuperMarioRunning_2.png");
    texture["SuperMarioRunning2Left"] = texture2DFlipHorizontal(texture["SuperMarioRunning2Right"]);
    texture["SuperMarioThrowingFireball0Right"] = LoadTexture("../resources/Mario/SuperMarioThrowingFireball_0.png");
    texture["SuperMarioThrowingFireball0Left"] = texture2DFlipHorizontal(texture["SuperMarioThrowingFireball0Right"]);
    texture["SuperMarioVictory0"] = LoadTexture("../resources/Mario/SuperMarioVictory_0.png");
    texture["TransitioningSuperMario0Right"] = LoadTexture("../resources/Mario/TransitioningSuperMario_0.png");
    texture["TransitioningSuperMario0Left"] = texture2DFlipHorizontal(texture["TransitioningSuperMario0Right"]);
    texture["TransitioningSuperMario1Right"] = LoadTexture("../resources/Mario/TransitioningSuperMario_1.png");
    texture["TransitioningSuperMario1Left"] = texture2DFlipHorizontal(texture["TransitioningSuperMario1Right"]);
    texture["TransitioningSuperMario2Right"] = LoadTexture("../resources/Mario/TransitioningSuperMario_2.png");
    texture["TransitioningSuperMario2Left"] = texture2DFlipHorizontal(texture["TransitioningSuperMario2Right"]);

    //Flower Mario
    texture["FlowerMario0Right"] = LoadTexture("../resources/Mario/FlowerMario_0.png");
    texture["FlowerMario0Left"] = texture2DFlipHorizontal(texture["FlowerMario0Right"]);
    texture["FlowerMario1Right"] = LoadTexture("../resources/Mario/FlowerMario_1.png");
    texture["FlowerMario1Left"] = texture2DFlipHorizontal(texture["FlowerMario1Right"]);
    texture["FlowerMario2Right"] = LoadTexture("../resources/Mario/FlowerMario_2.png");
    texture["FlowerMario2Left"] = texture2DFlipHorizontal(texture["FlowerMario2Right"]);
    texture["FlowerMarioDucking0Right"] = LoadTexture("../resources/Mario/FlowerMarioDucking_0.png");
    texture["FlowerMarioDucking0Left"] = texture2DFlipHorizontal(texture["FlowerMarioDucking0Right"]);
    texture["FlowerMarioFalling0Right"] = LoadTexture("../resources/Mario/FlowerMarioFalling_0.png");
    texture["FlowerMarioFalling0Left"] = texture2DFlipHorizontal(texture["FlowerMarioFalling0Right"]);
    texture["FlowerMarioJumping0Right"] = LoadTexture("../resources/Mario/FlowerMarioJumping_0.png");
    texture["FlowerMarioJumping0Left"] = texture2DFlipHorizontal(texture["FlowerMarioJumping0Right"]);
    texture["FlowerMarioJumpingAndRunning0Right"] = LoadTexture("../resources/Mario/FlowerMarioJumpingAndRunning_0.png");
    texture["FlowerMarioJumpingAndRunning0Left"] = texture2DFlipHorizontal(texture["FlowerMarioJumpingAndRunning0Right"]);
    texture["FlowerMarioLookingUp0Right"] = LoadTexture("../resources/Mario/FlowerMarioLookingUp_0.png");
    texture["FlowerMarioLookingUp0Left"] = texture2DFlipHorizontal(texture["FlowerMarioLookingUp0Right"]);
    texture["FlowerMarioRunning0Right"] = LoadTexture("../resources/Mario/FlowerMarioRunning_0.png");
    texture["FlowerMarioRunning0Left"] = texture2DFlipHorizontal(texture["FlowerMarioRunning0Right"]);
    texture["FlowerMarioRunning1Right"] = LoadTexture("../resources/Mario/FlowerMarioRunning_1.png");
    texture["FlowerMarioRunning1Left"] = texture2DFlipHorizontal(texture["FlowerMarioRunning1Right"]);
    texture["FlowerMarioRunning2Right"] = LoadTexture("../resources/Mario/FlowerMarioRunning_2.png");
    texture["FlowerMarioRunning2Left"] = texture2DFlipHorizontal(texture["FlowerMarioRunning2Right"]);
    texture["FlowerMarioThrowingFireball0Right"] = LoadTexture("../resources/Mario/FlowerMarioThrowingFireball_0.png");
    texture["FlowerMarioThrowingFireball0Left"] = texture2DFlipHorizontal(texture["FlowerMarioThrowingFireball0Right"]);
    texture["FlowerMarioVictory0"] = LoadTexture("../resources/Mario/FlowerMarioVictory_0.png");
    texture["TransitioningFlowerMario0Right"] = LoadTexture("../resources/Mario/TransitioningFlowerMario_0.png");
    texture["TransitioningFlowerMario0Left"] = texture2DFlipHorizontal(texture["TransitioningFlowerMario0Right"]);
    texture["TransitioningFlowerMario1Right"] = LoadTexture("../resources/Mario/TransitioningFlowerMario_1.png");
    texture["TransitioningFlowerMario1Left"] = texture2DFlipHorizontal(texture["TransitioningFlowerMario1Right"]);
    texture["TransitioningFlowerMario2Right"] = LoadTexture("../resources/Mario/TransitioningFlowerMario_2.png");
    texture["TransitioningFlowerMario2Left"] = texture2DFlipHorizontal(texture["TransitioningFlowerMario2Right"]);

    //Small Luigi
    texture["SmallLuigi0Right"] = LoadTexture("../resources/Luigi/SmallLuigi_0.png");
    texture["SmallLuigi0Left"] = texture2DFlipHorizontal(texture["SmallLuigi0Right"]);
    texture["SmallLuigi1Right"] = LoadTexture("../resources/Luigi/SmallLuigi_1.png");
    texture["SmallLuigi1Left"] = texture2DFlipHorizontal(texture["SmallLuigi1Right"]);
    texture["SmallLuigiDucking0Right"] = LoadTexture("../resources/Luigi/SmallLuigiDucking_0.png");
    texture["SmallLuigiDucking0Left"] = texture2DFlipHorizontal(texture["SmallLuigiDucking0Right"]);
    texture["SmallLuigiFalling0Right"] = LoadTexture("../resources/Luigi/SmallLuigiFalling_0.png");
    texture["SmallLuigiFalling0Left"] = texture2DFlipHorizontal(texture["SmallLuigiFalling0Right"]);
    texture["SmallLuigiJumping0Right"] = LoadTexture("../resources/Luigi/SmallLuigiJumping_0.png");
    texture["SmallLuigiJumping0Left"] = texture2DFlipHorizontal(texture["SmallLuigiJumping0Right"]);
    texture["SmallLuigiJumpingAndRunning0Right"] = LoadTexture("../resources/Luigi/SmallLuigiJumpingAndRunning_0.png");
    texture["SmallLuigiJumpingAndRunning0Left"] = texture2DFlipHorizontal(texture["SmallLuigiJumpingAndRunning0Right"]);
    texture["SmallLuigiLookingUp0Right"] = LoadTexture("../resources/Luigi/SmallLuigiLookingUp_0.png");
    texture["SmallLuigiLookingUp0Left"] = texture2DFlipHorizontal(texture["SmallLuigiLookingUp0Right"]);
    texture["SmallLuigiRunning0Right"] = LoadTexture("../resources/Luigi/SmallLuigiRunning_0.png");
    texture["SmallLuigiRunning0Left"] = texture2DFlipHorizontal(texture["SmallLuigiRunning0Right"]);
    texture["SmallLuigiRunning1Right"] = LoadTexture("../resources/Luigi/SmallLuigiRunning_1.png");
    texture["SmallLuigiRunning1Left"] = texture2DFlipHorizontal(texture["SmallLuigiRunning1Right"]);
    texture["SmallLuigiDying0"] = LoadTexture("../resources/Luigi/SmallLuigiDying_0.png");
    texture["SmallLuigiDying1"] = texture2DFlipHorizontal(texture["SmallLuigiDying0"]);
    texture["SmallLuigiVictory0"] = LoadTexture("../resources/Luigi/SmallLuigiVictory_0.png");

    //Super Luigi
    texture["SuperLuigi0Right"] = LoadTexture("../resources/Luigi/SuperLuigi_0.png");
    texture["SuperLuigi0Left"] = texture2DFlipHorizontal(texture["SuperLuigi0Right"]);
    texture["SuperLuigi1Right"] = LoadTexture("../resources/Luigi/SuperLuigi_1.png");
    texture["SuperLuigi1Left"] = texture2DFlipHorizontal(texture["SuperLuigi1Right"]);
    texture["SuperLuigi2Right"] = LoadTexture("../resources/Luigi/SuperLuigi_2.png");
    texture["SuperLuigi2Left"] = texture2DFlipHorizontal(texture["SuperLuigi2Right"]);
    texture["SuperLuigiDucking0Right"] = LoadTexture("../resources/Luigi/SuperLuigiDucking_0.png");
    texture["SuperLuigiDucking0Left"] = texture2DFlipHorizontal(texture["SuperLuigiDucking0Right"]);
    texture["SuperLuigiFalling0Right"] = LoadTexture("../resources/Luigi/SuperLuigiFalling_0.png");
    texture["SuperLuigiFalling0Left"] = texture2DFlipHorizontal(texture["SuperLuigiFalling0Right"]);
    texture["SuperLuigiJumping0Right"] = LoadTexture("../resources/Luigi/SuperLuigiJumping_0.png");
    texture["SuperLuigiJumping0Left"] = texture2DFlipHorizontal(texture["SuperLuigiJumping0Right"]);
    texture["SuperLuigiJumpingAndRunning0Right"] = LoadTexture("../resources/Luigi/SuperLuigiJumpingAndRunning_0.png");
    texture["SuperLuigiJumpingAndRunning0Left"] = texture2DFlipHorizontal(texture["SuperLuigiJumpingAndRunning0Right"]);
    texture["SuperLuigiLookingUp0Right"] = LoadTexture("../resources/Luigi/SuperLuigiLookingUp_0.png");
    texture["SuperLuigiLookingUp0Left"] = texture2DFlipHorizontal(texture["SuperLuigiLookingUp0Right"]);
    texture["SuperLuigiRunning0Right"] = LoadTexture("../resources/Luigi/SuperLuigiRunning_0.png");
    texture["SuperLuigiRunning0Left"] = texture2DFlipHorizontal(texture["SuperLuigiRunning0Right"]);
    texture["SuperLuigiRunning1Right"] = LoadTexture("../resources/Luigi/SuperLuigiRunning_1.png");
    texture["SuperLuigiRunning1Left"] = texture2DFlipHorizontal(texture["SuperLuigiRunning1Right"]);
    texture["SuperLuigiRunning2Right"] = LoadTexture("../resources/Luigi/SuperLuigiRunning_2.png");
    texture["SuperLuigiRunning2Left"] = texture2DFlipHorizontal(texture["SuperLuigiRunning2Right"]);
    texture["SuperLuigiThrowingFireball0Right"] = LoadTexture("../resources/Luigi/SuperLuigiThrowingFireball_0.png");
    texture["SuperLuigiThrowingFireball0Left"] = texture2DFlipHorizontal(texture["SuperLuigiThrowingFireball0Right"]);
    texture["SuperLuigiVictory0"] = LoadTexture("../resources/Luigi/SuperLuigiVictory_0.png");
    texture["TransitioningSuperLuigi0Right"] = LoadTexture("../resources/Luigi/TransitioningSuperLuigi_0.png");
    texture["TransitioningSuperLuigi0Left"] = texture2DFlipHorizontal(texture["TransitioningSuperLuigi0Right"]);
    texture["TransitioningSuperLuigi1Right"] = LoadTexture("../resources/Luigi/TransitioningSuperLuigi_1.png");
    texture["TransitioningSuperLuigi1Left"] = texture2DFlipHorizontal(texture["TransitioningSuperLuigi1Right"]);
    texture["TransitioningSuperLuigi2Right"] = LoadTexture("../resources/Luigi/TransitioningSuperLuigi_2.png");
    texture["TransitioningSuperLuigi2Left"] = texture2DFlipHorizontal(texture["TransitioningSuperLuigi2Right"]);

    //Flower Luigi
    texture["FlowerLuigi0Right"] = LoadTexture("../resources/Luigi/FlowerLuigi_0.png");
    texture["FlowerLuigi0Left"] = texture2DFlipHorizontal(texture["FlowerLuigi0Right"]);
    texture["FlowerLuigi1Right"] = LoadTexture("../resources/Luigi/FlowerLuigi_1.png");
    texture["FlowerLuigi1Left"] = texture2DFlipHorizontal(texture["FlowerLuigi1Right"]);
    texture["FlowerLuigi2Right"] = LoadTexture("../resources/Luigi/FlowerLuigi_2.png");
    texture["FlowerLuigi2Left"] = texture2DFlipHorizontal(texture["FlowerLuigi2Right"]);
    texture["FlowerLuigiDucking0Right"] = LoadTexture("../resources/Luigi/FlowerLuigiDucking_0.png");
    texture["FlowerLuigiDucking0Left"] = texture2DFlipHorizontal(texture["FlowerLuigiDucking0Right"]);
    texture["FlowerLuigiFalling0Right"] = LoadTexture("../resources/Luigi/FlowerLuigiFalling_0.png");
    texture["FlowerLuigiFalling0Left"] = texture2DFlipHorizontal(texture["FlowerLuigiFalling0Right"]);
    texture["FlowerLuigiJumping0Right"] = LoadTexture("../resources/Luigi/FlowerLuigiJumping_0.png");
    texture["FlowerLuigiJumping0Left"] = texture2DFlipHorizontal(texture["FlowerLuigiJumping_0Right"]);
    texture["FlowerLuigiJumpingAndRunning0Right"] = LoadTexture("../resources/Luigi/FlowerLuigiJumpingAndRunning_0.png");
    texture["FlowerLuigiJumpingAndRunning0Left"] = texture2DFlipHorizontal(texture["FlowerLuigiJumpingAndRunning_0Right"]);
    texture["FlowerLuigiLookingUp0Right"] = LoadTexture("../resources/Luigi/FlowerLuigiLookingUp_0.png");
    texture["FlowerLuigiLookingUp0Left"] = texture2DFlipHorizontal(texture["FlowerLuigiLookingUp_0Right"]);
    texture["FlowerLuigiRunning0Right"] = LoadTexture("../resources/Luigi/FlowerLuigiRunning_0.png");
    texture["FlowerLuigiRunning0Left"] = texture2DFlipHorizontal(texture["FlowerLuigiRunning_0Right"]);
    texture["FlowerLuigiRunning1Right"] = LoadTexture("../resources/Luigi/FlowerLuigiRunning_1.png");
    texture["FlowerLuigiRunning1Left"] = texture2DFlipHorizontal(texture["FlowerLuigiRunning_1Right"]);
    texture["FlowerLuigiRunning2Right"] = LoadTexture("../resources/Luigi/FlowerLuigiRunning_2.png");
    texture["FlowerLuigiRunning2Left"] = texture2DFlipHorizontal(texture["FlowerLuigiRunning_2Right"]);
    texture["FlowerLuigiThrowingFireball0Right"] = LoadTexture("../resources/Luigi/FlowerLuigiThrowingFireball_0.png");
    texture["FlowerLuigiThrowingFireball0Left"] = texture2DFlipHorizontal(texture["FlowerLuigiThrowingFireball_0Right"]);
    texture["FlowerLuigiVictory0"] = LoadTexture("../resources/Luigi/FlowerLuigiVictory_0.png");
    texture["TransitioningFlowerLuigi0Right"] = LoadTexture("../resources/Luigi/TransitioningFlowerLuigi_0.png");
    texture["TransitioningFlowerLuigi0Left"] = texture2DFlipHorizontal(texture["TransitioningFlowerLuigi0Right"]);
    texture["TransitioningFlowerLuigi1Right"] = LoadTexture("../resources/Luigi/TransitioningFlowerLuigi_1.png");
    texture["TransitioningFlowerLuigi1Left"] = texture2DFlipHorizontal(texture["TransitioningFlowerLuigi1Right"]);
    texture["TransitioningFlowerLuigi2Right"] = LoadTexture("../resources/Luigi/TransitioningFlowerLuigi_2.png");
    texture["TransitioningFlowerLuigi2Left"] = texture2DFlipHorizontal(texture["TransitioningFlowerLuigi2Right"]);

    //Fireball
    texture["Fireball0Right"] = LoadTexture("../resources/Fireball/Fireball_0.png");
    texture["Fireball0Left"] = texture2DFlipHorizontal(texture["Fireball0Right"]);
    texture["Fireball1Right"] = LoadTexture("../resources/Fireball/Fireball_1.png");
    texture["Fireball1Left"] = texture2DFlipHorizontal(texture["Fireball1Right"]);
    texture["Fireball2Right"] = LoadTexture("../resources/Fireball/Fireball_2.png");
    texture["Fireball2Left"] = texture2DFlipHorizontal(texture["Fireball2Right"]);
    texture["Fireball3Right"] = LoadTexture("../resources/Fireball/Fireball_3.png");
    texture["Fireball3Left"] = texture2DFlipHorizontal(texture["Fireball3Right"]);

    //Backgound
    texture["Background1"] = LoadTexture("../resources/Background/Background_1.png");
    texture["Background2"] = LoadTexture("../resources/Background/Background_2.png");
    texture["Background3"] = LoadTexture("../resources/Background/Background_3.png");
    texture["Background4"] = LoadTexture("../resources/Background/Background_4.png");
    texture["Background5"] = LoadTexture("../resources/Background/Background_5.png");
    texture["Background6"] = LoadTexture("../resources/Background/Background_6.png");
    texture["Background7"] = LoadTexture("../resources/Background/Background_7.png");
    texture["Background8"] = LoadTexture("../resources/Background/Background_8.png");
    texture["Background9"] = LoadTexture("../resources/Background/Background_9.png");
    texture["Background10"] = LoadTexture("../resources/Background/Background_10.png");

    //Enemy
    texture["BanzaiBill0Right"] = LoadTexture("../resources/Enemy/BanzaiBill_0.png");
    texture["BanzaiBill0Left"] = texture2DFlipHorizontal(texture["BanzaiBill0Right"]);
    texture["BlueKoopaTroopa0Right"] = LoadTexture("../resources/Enemy/BlueKoopaTroopa_0.png");
    texture["BlueKoopaTroopa0Left"] = texture2DFlipHorizontal(texture["BlueKoopaTroopa0Right"]);
    texture["BlueKoopaTroopa1Right"] = LoadTexture("../resources/Enemy/BlueKoopaTroopa_1.png");
    texture["BlueKoopaTroopa1Left"] = texture2DFlipHorizontal(texture["BlueKoopaTroopa1Right"]);
    texture["BobOmb0Right"] = LoadTexture("../resources/Enemy/BobOmb_0.png");
    texture["BobOmb0Left"] = texture2DFlipHorizontal(texture["BobOmb0Right"]);
    texture["BobOmb1Right"] = LoadTexture("../resources/Enemy/BobOmb_1.png");
    texture["BobOmb1Left"] = texture2DFlipHorizontal(texture["BobOmb1Right"]);
    texture["BulletBill0Right"] = LoadTexture("../resources/Enemy/BulletBill_0.png");
    texture["BulletBill0Left"] = texture2DFlipHorizontal(texture["BulletBill0Right"]);
    texture["BuzzyBeetle0Right"] = LoadTexture("../resources/Enemy/BuzzyBeetle_0.png");
    texture["BuzzyBeetle0Left"] = texture2DFlipHorizontal(texture["BuzzyBeetle0Right"]);
    texture["BuzzyBeetle1Right"] = LoadTexture("../resources/Enemy/BuzzyBeetle_1.png");
    texture["BuzzyBeetle1Left"] = texture2DFlipHorizontal(texture["BuzzyBeetle1Right"]);
    texture["FlyingGoomba0Right"] = LoadTexture("../resources/Enemy/FlyingGoomba_0.png");
    texture["FlyingGoomba0Left"] = texture2DFlipHorizontal(texture["FlyingGoomba0Right"]);
    texture["FlyingGoomba1Right"] = LoadTexture("../resources/Enemy/FlyingGoomba_1.png");
    texture["FlyingGoomba1Left"] = texture2DFlipHorizontal(texture["FlyingGoomba1Right"]);
    texture["FlyingGoomba2Right"] = LoadTexture("../resources/Enemy/FlyingGoomba_2.png");
    texture["FlyingGoomba2Left"] = texture2DFlipHorizontal(texture["FlyingGoomba2Right"]);
    texture["FlyingGoomba3Right"] = LoadTexture("../resources/Enemy/FlyingGoomba_3.png");
    texture["FlyingGoomba3Left"] = texture2DFlipHorizontal(texture["FlyingGoomba3Right"]);
    texture["Goomba0Right"] = LoadTexture("../resources/Enemy/Goomba_0.png");
    texture["Goomba0Left"] = texture2DFlipHorizontal(texture["Goomba0Right"]);
    texture["Goomba1Right"] = LoadTexture("../resources/Enemy/Goomba_1.png");
    texture["Goomba1Left"] = texture2DFlipHorizontal(texture["Goomba1Right"]);
    texture["GreenKoopaTroopa0Right"] = LoadTexture("../resources/Enemy/GreenKoopaTroopa_0.png");
    texture["GreenKoopaTroopa0Left"] = texture2DFlipHorizontal(texture["GreenKoopaTroopa0Right"]);
    texture["GreenKoopaTroopa1Right"] = LoadTexture("../resources/Enemy/GreenKoopaTroopa_1.png");
    texture["GreenKoopaTroopa1Left"] = texture2DFlipHorizontal(texture["GreenKoopaTroopa1Right"]);
    texture["JumpingPiranhaPlant0"] = LoadTexture("../resources/Enemy/JumpingPiranhaPlant_0.png");
    texture["JumpingPiranhaPlant1"] = LoadTexture("../resources/Enemy/JumpingPiranhaPlant_1.png");
    texture["JumpingPiranhaPlant2"] = LoadTexture("../resources/Enemy/JumpingPiranhaPlant_2.png");
    texture["JumpingPiranhaPlant3"] = LoadTexture("../resources/Enemy/JumpingPiranhaPlant_3.png");
    texture["MontyMole0Right"] = LoadTexture("../resources/Enemy/MontyMole_0.png");
    texture["MontyMole0Left"] = texture2DFlipHorizontal(texture["MontyMole0Right"]);
    texture["MontyMole1Right"] = LoadTexture("../resources/Enemy/MontyMole_1.png");
    texture["MontyMole1Left"] = texture2DFlipHorizontal(texture["MontyMole1Right"]);
    texture["MummyBeetle0Right"] = LoadTexture("../resources/Enemy/MummyBeetle_0.png");
    texture["MummyBeetle0Left"] = texture2DFlipHorizontal(texture["MummyBeetle0Right"]);
    texture["MummyBeetle1Right"] = LoadTexture("../resources/Enemy/MummyBeetle_1.png");
    texture["MummyBeetle1Left"] = texture2DFlipHorizontal(texture["MummyBeetle1Right"]);
    texture["Muncher0"] = LoadTexture("../resources/Enemy/Muncher_0.png");
    texture["Muncher1"] = LoadTexture("../resources/Enemy/Muncher_1.png");
    texture["PiranhaPlant0"] = LoadTexture("../resources/Enemy/PiranhaPlant_0.png");
    texture["PiranhaPlant1"] = LoadTexture("../resources/Enemy/PiranhaPlant_1.png");
    texture["RedKoopaTroopa0Right"] = LoadTexture("../resources/Enemy/RedKoopaTroopa_0.png");
    texture["RedKoopaTroopa0Left"] = texture2DFlipHorizontal(texture["RedKoopaTroopa0Right"]);
    texture["RedKoopaTroopa1Right"] = LoadTexture("../resources/Enemy/RedKoopaTroopa_1.png");
    texture["RedKoopaTroopa1Left"] = texture2DFlipHorizontal(texture["RedKoopaTroopa1Right"]);
    texture["Rex10Right"] = LoadTexture("../resources/Enemy/Rex_1_0.png");
    texture["Rex10Left"] = texture2DFlipHorizontal(texture["Rex10Right"]);
    texture["Rex11Right"] = LoadTexture("../resources/Enemy/Rex_1_1.png");
    texture["Rex11Left"] = texture2DFlipHorizontal(texture["Rex11Right"]);
    texture["Rex20Right"] = LoadTexture("../resources/Enemy/Rex_2_0.png");
    texture["Rex20Left"] = texture2DFlipHorizontal(texture["Rex20Right"]);
    texture["Rex21Right"] = LoadTexture("../resources/Enemy/Rex_2_1.png");
    texture["Rex21Left"] = texture2DFlipHorizontal(texture["Rex21Right"]);
    texture["Swooper0Right"] = LoadTexture("../resources/Enemy/Swooper_0.png");
    texture["Swooper0Left"] = texture2DFlipHorizontal(texture["Swooper0Right"]);
    texture["Swooper1Right"] = LoadTexture("../resources/Enemy/Swooper_1.png");
    texture["Swooper1Left"] = texture2DFlipHorizontal(texture["Swooper1Right"]);
    texture["Swooper2Right"] = LoadTexture("../resources/Enemy/Swooper_2.png");
    texture["Swooper2Left"] = texture2DFlipHorizontal(texture["Swooper2Right"]);
    texture["YellowKoopaTroopa0Right"] = LoadTexture("../resources/Enemy/YellowKoopaTroopa_0.png");
    texture["YellowKoopaTroopa0Left"] = texture2DFlipHorizontal(texture["YellowKoopaTroopa0Right"]);
    texture["YellowKoopaTroopa1Right"] = LoadTexture("../resources/Enemy/YellowKoopaTroopa_1.png");
    texture["YellowKoopaTroopa1Left"] = texture2DFlipHorizontal(texture["YellowKoopaTroopa1Right"]);

    //Block
    texture["Cloud0"] = LoadTexture("../resources/Block/Cloud_0.png");
    texture["Exclamation0"] = LoadTexture("../resources/Block/Exclamation_0.png");
    texture["EyesClosed0"] = LoadTexture("../resources/Block/EyesClosed_0.png");
    texture["EyesOpened0"] = LoadTexture("../resources/Block/EyesOpened_0.png");
    texture["EyesOpened1"] = LoadTexture("../resources/Block/EyesOpened_1.png");
    texture["EyesOpened2"] = LoadTexture("../resources/Block/EyesOpened_2.png");
    texture["EyesOpened3"] = LoadTexture("../resources/Block/EyesOpened_3.png");
    texture["Glass0"] = LoadTexture("../resources/Block/Glass_0.png");
    texture["Message0"] = LoadTexture("../resources/Block/Message_0.png");
    texture["Question0"] = LoadTexture("../resources/Block/Question_0.png");
    texture["Question1"] = LoadTexture("../resources/Block/Question_1.png");
    texture["Question2"] = LoadTexture("../resources/Block/Question_2.png");
    texture["Question3"] = LoadTexture("../resources/Block/Question_3.png");
    texture["Stone0"] = LoadTexture("../resources/Block/Stone_0.png");
    texture["Wood0"] = LoadTexture("../resources/Block/Wood_0.png");

    //Item
    texture["1UpMushroom"] = LoadTexture("../resources/Item/1UpMushroom.png");
    texture["3UpMoon"] = LoadTexture("../resources/Item/3UpMoon.png");
    texture["BeanstalkHead0"] = LoadTexture("../resources/Item/BeanstalkHead_0.png");
    texture["BeanstalkHead1"] = LoadTexture("../resources/Item/BeanstalkHead_1.png");
    texture["BeanstalkHeadLoop"] = LoadTexture("../resources/Item/BeanstalkHeadLoop.png");
    texture["Coin0"] = LoadTexture("../resources/Item/Coin_0.png");
    texture["Coin1"] = LoadTexture("../resources/Item/Coin_1.png");
    texture["Coin2"] = LoadTexture("../resources/Item/Coin_2.png");
    texture["Coin3"] = LoadTexture("../resources/Item/Coin_3.png");
    texture["CourseClearToken"] = LoadTexture("../resources/Item/CourseClearToken.png");
    texture["FireFlower0"] = LoadTexture("../resources/Item/FireFlower_0.png");
    texture["FireFlower1"] = LoadTexture("../resources/Item/FireFlower_1.png");
    texture["Mushroom"] = LoadTexture("../resources/Item/Mushroom.png");
    texture["Star"] = LoadTexture("../resources/Item/Star.png");
    texture["YoshiCoin0"] = LoadTexture("../resources/Item/YoshiCoin_0.png");
    texture["YoshiCoin1"] = LoadTexture("../resources/Item/YoshiCoin_1.png");
    texture["YoshiCoin2"] = LoadTexture("../resources/Item/YoshiCoin_2.png");
    texture["YoshiCoin3"] = LoadTexture("../resources/Item/YoshiCoin_3.png");

    //Tile
    for (int i=1;i<=4;i++){
        for (char j='A'; j<='Z'; j++){
            std::string key = std::to_string(i) + j;
            texture[key] = LoadTexture(("../resources/Tile/" + std::to_string(i) + "/tile_" + j + ".png").c_str());
        }
    }

    //Effect
    texture["Puft0"] = LoadTexture("../resources/Effect/Puft_0.png");
    texture["Puft1"] = LoadTexture("../resources/Effect/Puft_1.png");
    texture["Puft2"] = LoadTexture("../resources/Effect/Puft_2.png");
    texture["Puft3"] = LoadTexture("../resources/Effect/Puft_3.png");
    texture["Start0"] = LoadTexture("../resources/Effect/Start_0.png");
    texture["Start1"] = LoadTexture("../resources/Effect/Start_1.png");
    texture["Start2"] = LoadTexture("../resources/Effect/Start_2.png");
    texture["Start3"] = LoadTexture("../resources/Effect/Start_3.png");

    //GUI
    texture["Gui1Up"] = LoadTexture("../resources/gui/gui1Up.png");
    texture["Gui2Up"] = LoadTexture("../resources/gui/gui2Up.png");
    texture["Gui3Up"] = LoadTexture("../resources/gui/gui3Up.png");
    texture["GuiAlfa"] = LoadTexture("../resources/gui/guiAlfa.png");
    texture["GuiAlfaLowerUpper"] = LoadTexture("../resources/gui/guiAlfaLowerUpper.png");
    texture["GuiClock"] = LoadTexture("../resources/gui/guiClock.png");
    texture["GuiCoin"] = LoadTexture("../resources/gui/guiCoin.png");
    texture["GuiGameOver"] = LoadTexture("../resources/gui/guiGameOver.png");
    texture["Gui100"] = LoadTexture("../resources/gui/guiHundredsPoints_100.png");
    texture["Gui200"] = LoadTexture("../resources/gui/guiHundredsPoints_200.png");
    texture["Gui400"] = LoadTexture("../resources/gui/guiHundredsPoints_400.png");
    texture["Gui800"] = LoadTexture("../resources/gui/guiHundredsPoints_800.png");
    texture["GuiLetters"] = LoadTexture("../resources/gui/guiLetters.png");
    texture["GuiMario"] = LoadTexture("../resources/gui/guiMario.png");
    texture["GuiLuigi"] = LoadTexture("../resources/gui/guiLuigi.png");
    texture["GuiMarioStart"] = LoadTexture("../resources/gui/guiMarioStart.png");
    texture["GuiNextItem"] = LoadTexture("../resources/gui/guiNextItem.png");
    texture["GuiNumbersBig"] = LoadTexture("../resources/gui/guiNumbersBig.png");
    texture["GuiNumbersWhite"] = LoadTexture("../resources/gui/guiNumbersWhite.png");
    texture["GuiNumbersYellow"] = LoadTexture("../resources/gui/guiNumbersYellow.png");
    texture["GuiPunctuation"] = LoadTexture("../resources/gui/guiPunctuation.png");
    texture["GuiRayMarioLogo"] = LoadTexture("../resources/gui/guiRayMarioLogo.png");
    texture["Gui10"] = LoadTexture("../resources/gui/guiTensPoints_10.png");
    texture["Gui20"] = LoadTexture("../resources/gui/guiTensPoints_20.png");
    texture["Gui40"] = LoadTexture("../resources/gui/guiTensPoints_40.png");
    texture["Gui80"] = LoadTexture("../resources/gui/guiTensPoints_80.png");
    texture["Gui1000"] = LoadTexture("../resources/gui/guiThousandsPoints_1000.png");
    texture["Gui2000"] = LoadTexture("../resources/gui/guiThousandsPoints_2000.png");
    texture["Gui4000"] = LoadTexture("../resources/gui/guiThousandsPoints_4000.png");
    texture["Gui8000"] = LoadTexture("../resources/gui/guiThousandsPoints_8000.png");
    texture["GuiTime"] = LoadTexture("../resources/gui/guiTime.png");
    texture["GuiTimeUp"] = LoadTexture("../resources/gui/guiTimeUp.png");
    texture["GuiX"] = LoadTexture("../resources/gui/guiX.png");
};

void ResourceManager::loadSound() {
    sound["1Up"] = LoadSound("../resources/Sound/1_up.wav");
    sound["BreakBlock"] = LoadSound("../resources/Sound/break_block.wav");
    sound["ChuckWhistle"] = LoadSound("../resources/Sound/chuck_whistle.wav");
    sound["Coin"] = LoadSound("../resources/Sound/coin.wav");
    sound["DragonCoin"] = LoadSound("../resources/Sound/dragon_coin.wav");
    sound["Fireball"] = LoadSound("../resources/Sound/fireball.wav");
    sound["GoalIrisOut"] = LoadSound("../resources/Sound/goal_iris_out.wav");
    sound["Jump"] = LoadSound("../resources/Sound/jump.wav");
    sound["Kick"] = LoadSound("../resources/Sound/kick.wav");
    sound["MessageBlock"] = LoadSound("../resources/Sound/message_block.wav");
    sound["Pause"] = LoadSound("../resources/Sound/pause.wav");
    sound["Pipe"] = LoadSound("../resources/Sound/pipe.wav");
    sound["PowerUpAppear"] = LoadSound("../resources/Sound/power_up_appear.wav");
    sound["PowerUp"] = LoadSound("../resources/Sound/power_up.wav");
    sound["ReleasePowerUpItem"] = LoadSound("../resources/Sound/release_power_up_item.wav");
    sound["StorePowerUpItem"] = LoadSound("../resources/Sound/store_power_up_item.wav");
    sound["RidingYoshi"] = LoadSound("../resources/Sound/riding_yoshi.wav");
    sound["ShellRicochet"] = LoadSound("../resources/Sound/shell_ricochet.wav");
    sound["StompNoDamage"] = LoadSound("../resources/Sound/stomp_no_damage.wav");
    sound["Stomp"] = LoadSound("../resources/Sound/stomp.wav");
}

void ResourceManager::loadMusic() {
    music["CourseClear"] = LoadMusicStream("../resources/Music/courseClear.mp3");
    music["Ending"] = LoadMusicStream("../resources/Music/ending.mp3");
    music["GameOver"] = LoadMusicStream("../resources/Music/gameOver.mp3");
    music["Music1"] = LoadMusicStream("../resources/Music/music1.mp3");
    music["Music2"] = LoadMusicStream("../resources/Music/music2.mp3");
    music["Music3"] = LoadMusicStream("../resources/Music/music3.mp3");
    music["Music4"] = LoadMusicStream("../resources/Music/music4.mp3");
    music["Music5"] = LoadMusicStream("../resources/Music/music5.mp3");
    music["Music6"] = LoadMusicStream("../resources/Music/music6.mp3");
    music["Music7"] = LoadMusicStream("../resources/Music/music7.mp3");
    music["Music8"] = LoadMusicStream("../resources/Music/music8.mp3");
    music["Music9"] = LoadMusicStream("../resources/Music/music9.mp3");
    music["PlayerDown"] = LoadMusicStream("../resources/Music/playerDown.mp3");
    music["Title"] = LoadMusicStream("../resources/Music/title.mp3");
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

void ResourceManager::drawWhiteSmallNumber(int number, int x, int y) {
    Texture2D texture = ResourceManager::getTexture()["GuiNumbersWhite"];
    int width = 18;
    int height = 14;
    std::string str = std::to_string( number );
    int px = x;
    for ( char i : str ) {
        DrawTextureRec( texture, { float((i - '0') * width), 0.0f, float(width), float(height)}, {float(px), float(y)}, WHITE );
        px += width - 2;
    }
}

void ResourceManager::drawYellowSmallNumber(int number, int x, int y) {
    Texture2D texture = ResourceManager::getTexture()["GuiNumbersYellow"];
    int width = 18;
    int height = 14;
    std::string str = std::to_string( number );
    int px = x;
    for ( char i : str ) {
        DrawTextureRec( texture, { float((i - '0') * width), 0.0f, float(width), float(height) }, { float(px), float(y) }, WHITE );
        px += width - 2;
    }
}

void ResourceManager::drawBigNumber(int number, int x, int y) {
    Texture2D texture = ResourceManager::getTexture()["GuiNumbersBig"];
    int width = 18;
    int height = 28;
    std::string str = std::to_string( number );
    int px = x;
    for ( char i : str ) {
        DrawTextureRec( texture, { float((i - '0') * width), 0.0f, float(width), float(height) }, { float(px), float(y) }, WHITE );
        px += width - 2;
    }
}

int ResourceManager::getSmallNumberWidth(int number) {
    return 16 * std::to_string( number ).length();
}

int ResourceManager::getSmallNumberHeight() {
    return 14;
}

int ResourceManager::getBigNumberWidth(int number) {
    return 16 * std::to_string( number ).length();
}

int ResourceManager::getBigNumberHeight() {
    return 28;
}

void ResourceManager::drawString(std::string str, int x, int y) {
    Texture2D texture = ResourceManager::getTexture()["GuiAlfa"];
    int width = 18;
    int height = 20;
    int px = x;
    for ( char i : str ) {
        int code = std::toupper( i );
        bool space = false;
        int textureY;
        bool undefined = false;
        if ( code >= '0' && code <= '9' ) {
            code -= '0';
            textureY = 0;
        } else if ( code >= 'A' && code <= 'Z' ) {
            code -= 'A';
            textureY = 20;
        } else {
            switch ( code ) {
                case '.':  code = 0; break;
                case ',':  code = 1; break;
                case '-':  code = 2; break;
                case '!':  code = 3; break;
                case '?':  code = 4; break;
                case '=':  code = 5; break;
                case ':':  code = 6; break;
                case '\'': code = 7; break;
                case '"':  code = 8; break;
                case ' ':  space = true; break;
                default:   undefined = true; break;
            }
            textureY = 60;
        }
        if ( undefined ) {
            code = 4;
            textureY = 60;
        }
        if ( !space ) {
            DrawTextureRec( texture, { float(code * width), float(textureY), float(width), float(height) }, { float(px), float(y) }, WHITE );
        }
        px += width - 2;
    }
}

int ResourceManager::getDrawStringWidth(std::string str) {
    return 16 * str.length(); 
}

int ResourceManager::getDrawStringHeight() {
    return 20;
}

void ResourceManager::drawMessageString(std::string str, int x, int y) {
    const Texture2D texture = ResourceManager::getTexture()["GuiAlfaLowerUpper"];
    int width = 16;
    int height = 16;
    int px = x;
    for ( int code : str ) {
        bool space = false;
        int textureY;
        bool undefined = false;
        if ( code >= '0' && code <= '9' ) {
            code -= '0';
            textureY = 0;
        } else if ( code >= 'A' && code <= 'Z' ) {
            code -= 'A';
            textureY = 16;
        } else if ( code >= 'a' && code <= 'z' ) {
            code -= 'a';
            textureY = 32;
        } else {
            switch ( code ) {
                case '.':  code = 0; break;
                case ',':  code = 1; break;
                case '-':  code = 2; break;
                case '!':  code = 3; break;
                case '?':  code = 4; break;
                case '=':  code = 5; break;
                case ':':  code = 6; break;
                case '\'': code = 7; break;
                case '"':  code = 8; break;
                case '#':  code = 9; break;
                case '(':  code = 10; break;
                case ')':  code = 11; break;
                case ' ':  space = true; break;
                default:   undefined = true; break;
            }
            textureY = 48;
        }
        if ( undefined ) {
            code = 4;
            textureY = 48;
        }
        if ( !space ) {
            DrawTextureRec( texture, { float(code * width), float(textureY), float(width), float(height) }, { float(px), float(y) }, WHITE );
        }
        px += width - 2;
    }
}

int ResourceManager::getMessageStringWidth(std::string str) {
    return 14* str.length();
}

int ResourceManager::getMessageStringHeight() {
    return 16;
}

std::vector<std::string> ResourceManager::split( const std::string& str, char delim ) {
    std::vector<std::string> result;
    std::string line;
    std::istringstream ss( str );
    while( std::getline( ss, line, delim ) ) {
        result.push_back( line );
    }
    return result;
}