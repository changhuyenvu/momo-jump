#include"player.h"
#include "utils.h"
#include"constants.h"
#include<iostream>
using namespace std;

Player::Player() : renderer(nullptr),playerTexture(nullptr), playerDirection(0),
 frameIndex(0),isMoving(false), isMovingHorizontally(false), inWater(false), onRaft(false){}

void Player::loadPlayerSpriteSheet() {

    playerTexture = loadTexture("asset/player.png", renderer);

    for (int row = 0; row < SPRITE_SHEET_ROWS; row++) {
        for (int col = 0; col < SPRITE_SHEET_COLS; col++) {
            playerFrames[row][col] = {col * PLAYER_WIDTH, row * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT};
        }
    }
}

bool Player::init(SDL_Renderer* renderer) {
    this->renderer = renderer;
    loadPlayerSpriteSheet();
    if (!playerTexture) {
        cerr << "Failed to load player image..." << endl;
        return false;
    }
    return true;
}


void Player::update(int frameCount, Challenge& challenge) {

    if (frameCount % 6 == 0) {
        frameIndex = (frameIndex + 1) % SPRITE_SHEET_COLS; 
    }

}

void Player::render() {

    if (!inWater) {
        SDL_Rect currentFrame = playerFrames[playerDirection][frameIndex];
        SDL_RenderCopy(renderer, playerTexture, &currentFrame, &rect);
    }

}
 

void Player::reset() {
    rect = {SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2, SCREEN_HEIGHT / 2, PLAYER_WIDTH, PLAYER_HEIGHT};
    playerDirection = 0;
    isMoving = false;
    isMovingHorizontally =false;
    inWater = false;
    onRaft = false;
    frameIndex = 0;
}

void Player::destroy() {

    if (playerTexture) {
        SDL_DestroyTexture(playerTexture);
        playerTexture = nullptr;
    }
}



