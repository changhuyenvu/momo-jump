#ifndef SOUND_H
#define SOUND_H

#include <SDL2/SDL_mixer.h>
using namespace std;
#include <string>

class Sound {
public:
    Sound(const string& filepath); 
    ~Sound();                        
    void playSound();               
    void stopSound();                 

private:
    Mix_Chunk* sound;             
};
#endif