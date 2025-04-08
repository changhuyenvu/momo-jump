#ifndef ENTITY_H
#define ENTITY_H

#include <SDL2/SDL.h>

struct Entity {
    SDL_Rect rect;
    Entity(int x, int y, int w, int h) : rect{x, y, w, h} {}
    Entity() = default;
};

#endif