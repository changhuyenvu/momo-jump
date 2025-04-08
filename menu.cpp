#include "menu.h"
#include "utils.h"
#include <iostream>

Menu::Menu(SDL_Renderer* renderer) {
    this->renderer = renderer;

    playRects = {150, 265, 125, 55};
    exitRects = {150, 340, 125, 50};
}

Menu::~Menu() {
    if (menuBackgroundTexture) {
        SDL_DestroyTexture(menuBackgroundTexture);
    }
}

void Menu::loadMenuTexture() {

    menuBackgroundTexture = loadTexture("asset/menu.png", renderer);
   
}

void Menu::renderMenu() {

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, menuBackgroundTexture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

int Menu::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) return 2; // Thoát
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            int x = event.button.x;
            int y = event.button.y;
            if (x >= playRects.x && x <= playRects.x + playRects.w && y >= playRects.y && y <= playRects.y + playRects.h ) return 1; // Chơi
            if (x >= exitRects.x && x <= exitRects.x + exitRects.w && y >= exitRects.y && y <= exitRects.y + exitRects.h) return 2; // Thoát
        }
    }
    return 0; // Không có sự kiện nào
}

int Menu::show() {
    loadMenuTexture();
    while (true) {
        renderMenu();
        int choice = handleEvents();
        if (choice != 0) {
            return choice;
        }
        SDL_Delay(16);
    }
}