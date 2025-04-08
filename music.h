#ifndef MUSIC_H
#define MUSIC_H

#include <SDL2/SDL_mixer.h>
using namespace std;
#include <string>
class Music{
    public:
        Music(string musicfilepath);
        ~Music();

		// number of times to play the music
		// 0 means 0
		// -1 means plays forever.
		void PlayMusic(int loops);
        // Pause the music
		void PauseMusic();
		// A range of 0 to 128
		void SetVolume(int volume);
	private:
		Mix_Music* m_music;
};


#endif