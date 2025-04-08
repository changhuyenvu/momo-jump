#ifndef UTILS_H
#define UTILS_H
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>

SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer);
bool checkCollision(const SDL_Rect& a, const SDL_Rect& b);
bool checkWaterCollision(const SDL_Rect& playerRect, const SDL_Rect& riverRect);


#endif