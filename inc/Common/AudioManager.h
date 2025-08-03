#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

class AudioManager {
private:
    static float musicVolume; 
    static float sfxVolume; 
public:
    static void setMusicVolume(float volume);
    static float getMusicVolume();

    static void setSfxVolume(float volume);
    static float getSfxVolume();
};

#endif