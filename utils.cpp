#include "utils.h"
#include "constants.h"
#include <iostream>
using namespace std;

SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, path);
    if (!texture) {
        cerr << "Failed to load texture: " << path << " - " << SDL_GetError() << endl;
    }
    return texture;
}

bool checkCollision(const SDL_Rect& a, const SDL_Rect& b) {
    return SDL_HasIntersection(&a, &b);
}
// kiểm tra ngưỡng va chạm với sông
bool checkWaterCollision(const SDL_Rect& playerRect, const SDL_Rect& riverRect) {
    if (!SDL_HasIntersection(&playerRect, &riverRect)) {
        return false;
    }
    int intersectionTop = max(playerRect.y, riverRect.y);
    int intersectionBottom = min(playerRect.y + playerRect.h, riverRect.y + riverRect.h);
    int intersectionHeight = intersectionBottom - intersectionTop;
    float intersectionRatio = static_cast<float>(intersectionHeight) / playerRect.h;
    return intersectionRatio >= WATER_COLLISION_THRESHOLD;
}