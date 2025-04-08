#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <vector>
#include "constants.h"
#include "entity.h"
#include"player.h"
#include"challenge.h"
#include"sound.h"
#include"music.h"

class Game {
    public:
        Game();
        bool init(SDL_Renderer* renderer);
        void run();
    
    private:
        Music backgroundMusic;
        Sound jumpSound;
        Sound waterSplashSound;
        Sound arrowHitSound;
        Player player;
        Challenge challenge;
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Texture* background;
        SDL_Texture* waterSplashTexture;
        SDL_Rect waterSplashFrames[WATER_SPLASH_FRAMES];
        int frameCount;
        int mapOffset;
        int mapScrollSpeed;
        int waterSplashFrameIndex;
        int waterSplashFrameCounter;
        SDL_Rect waterSplashPos;
        SDL_Rect currentRaftRect;
        bool running;
        bool waterSplashSoundPlayed;
    
        void handleEvents(SDL_Event& event);
        void update();
        void render();
        void close();
        void reset();
        void loadWaterSplashSpriteSheet();
        bool canMoveTo(const SDL_Rect& newPos);
        void checkRiverAndRaftCollision(SDL_Rect& tempPos, SDL_Rect& newRaftRect);
        void checkRaftCollision(SDL_Rect& newPos, const Player& player, const Challenge& challenge);
        void updateWaterSplash(const SDL_Rect& playerWaterCollisionRect);
        void updateMapScrolling();
        bool checkArrowCollision();
};

#endif