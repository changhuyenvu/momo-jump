#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
 
class Menu {
    public:
        Menu(SDL_Renderer* renderer);
        ~Menu();
        int show();
    private:
        SDL_Renderer* renderer;
        SDL_Texture* menuBackgroundTexture;
        SDL_Rect playRects;
        SDL_Rect exitRects;

        void loadMenuTexture();
        void renderMenu();
        int handleEvents();
};
#endif