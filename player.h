#ifndef PLAYER_H
#define  PLAYER_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include"challenge.h"
class Player{
public:
    SDL_Rect playerFrames[SPRITE_SHEET_ROWS][SPRITE_SHEET_COLS];
    Player();
    bool init(SDL_Renderer* renderer);
    SDL_Rect rect;
    Player(int x, int y, int w, int h) : rect{x, y, w, h} {}
    SDL_Texture* playerTexture;
    SDL_Renderer* renderer;
    int playerDirection;
    bool isMoving;
    bool isMovingHorizontally;
    bool inWater;
    bool onRaft;
    int frameIndex;
    void destroy();
    void loadPlayerSpriteSheet ();
    void render();
    void update(int frameCount, Challenge& challenge);
    void reset();
    
};

#endif