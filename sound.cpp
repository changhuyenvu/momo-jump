#include "Sound.h"
#include <iostream>
using namespace std;

Sound::Sound(const string& filepath) {
    sound = Mix_LoadWAV(filepath.c_str());
    if (!sound) {
        cerr << "Failed to load sound file '" << filepath << "': " << Mix_GetError() << std::endl;
    } else {
        cout << "Sound file '" << filepath << "' loaded successfully!" << endl;
    }
}

Sound::~Sound() {
    if (sound) {
        Mix_FreeChunk(sound);
        sound = nullptr;
    }
}

void Sound::playSound() {
    if (sound) {
        int channel = Mix_PlayChannel(-1, sound, 0); // -1 để chọn kênh tự do, 0 để không lặp
        if (channel == -1) {
            cerr << "Failed to play sound: " << Mix_GetError() << endl;
        }
    } else {
        cerr << "No sound loaded to play!" << endl;
    }
}

void Sound::stopSound() {

    Mix_HaltChannel(-1); // Dừng tất cả kênh (-1)
    
}