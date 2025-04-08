#include "Music.h"
#include <iostream>

Music::Music(string musicfilepath){

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)==-1) {
        cout << "Audio library not working:" << Mix_GetError() << endl;;
    } 

    m_music= Mix_LoadMUS(musicfilepath.c_str());
}

Music::~Music(){
    Mix_FreeMusic(m_music);
}

// number of times to play the music
// 0 means 0
// -1 means plays forever.
void Music::PlayMusic(int loops){
    if(m_music!=nullptr){
        Mix_PlayMusic(m_music, loops );
    }
}

void Music::PauseMusic(){
    Mix_PauseMusic();
}

// A range of 0 to 128
void Music::SetVolume(int volume){
    Mix_VolumeMusic(volume);
}