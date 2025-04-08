#include <iostream>
#include "menu.h"
#include "game.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
using namespace std;

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        cerr << "Không thể khởi tạo SDL: " << SDL_GetError() << endl;
        return -1;
    }

    if (Mix_Init(MIX_INIT_MP3) != MIX_INIT_MP3) {
        cerr << "SDL_mixer không thể khởi tạo: " << Mix_GetError() << endl;
        SDL_Quit();
        return -1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
        cerr << "SDL_mixer không thể mở audio: " << Mix_GetError() << endl;
        SDL_Quit();
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Momo Jump!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 440, 660, SDL_WINDOW_SHOWN);
    if (!window) {
        cerr << "Không thể tạo cửa sổ: " << SDL_GetError() << endl;
        Mix_Quit();
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cerr << "Không thể tạo renderer: " << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        Mix_Quit();
        SDL_Quit();
        return -1;
    }

    Menu* menu = new Menu(renderer);
    bool quit = false;

    while (!quit) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        int choice = menu->show(); 
        if (choice == 1) {
            Game* game = new Game(); 
            if (game->init(renderer)) {
                game->run();
            } else {
                cerr << "Không thể khởi tạo game." << endl;
            }
            delete game;
        } else if (choice == 2 || choice == 3) {
            quit = true;
        }
    }

    delete menu;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_Quit();
    SDL_Quit();
    return 0;
}
