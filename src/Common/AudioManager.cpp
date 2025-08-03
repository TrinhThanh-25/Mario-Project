#include "Common/AudioManager.h"
#include "Common/ResourceManager.h"
#include "raylib.h"

float AudioManager::musicVolume = 1.0f;
float AudioManager::sfxVolume = 1.0f;

void AudioManager::setMusicVolume(float volume) {
    musicVolume = volume;
    auto& music = ResourceManager::getMusic();
    for (auto& [name, musicStream] : music) {
        SetMusicVolume(musicStream, musicVolume);
    }
}

float AudioManager::getMusicVolume() {
    return musicVolume;
}

void AudioManager::setSfxVolume(float volume) {
    sfxVolume = volume;
    auto& sound = ResourceManager::getSound();
    for (auto& [name, soundEffect] : sound) {
        SetSoundVolume(soundEffect, sfxVolume);
    }
}

float AudioManager::getSfxVolume() {
    return sfxVolume;
}