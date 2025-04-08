#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include"game.h"
#include"utils.h"
using namespace std;

Game::Game() : window(nullptr), renderer(nullptr), background(nullptr), waterSplashTexture(nullptr),
    frameCount(0), mapOffset(0), mapScrollSpeed(BASE_MAP_SCROLL_SPEED), waterSplashFrameIndex(0),
    waterSplashFrameCounter(0), waterSplashPos{0, 0, WATER_SPLASH_WIDTH, WATER_SPLASH_HEIGHT},
    running(false), jumpSound("asset/water-drop-fx.wav"), backgroundMusic("asset/Summer_JoeHisaishi.wav"),
    waterSplashSound("asset/recording-hit-water.wav"), waterSplashSoundPlayed(false),
    arrowHitSound("asset/flamenco-clap-box-hit.wav")
{}

bool Game::init(SDL_Renderer* renderer) {
    this->renderer = renderer;  
    player.init(renderer);
    challenge.init(renderer);
    background = loadTexture("asset/grass_bg.jpg", renderer);
    waterSplashTexture = loadTexture("asset/smoke.png", renderer);

    if (!background || !waterSplashTexture) {
        cerr << "Failed to load one or more assets!" << endl;
        return false;
    }

    loadWaterSplashSpriteSheet();
    reset();
    running = true;
    return true;
}

void Game::run() {
    SDL_Event event;
    backgroundMusic.PlayMusic(-1);

    while (running) {
        while (SDL_PollEvent(&event)) {
            handleEvents(event);
        }
        update();
        render();
        SDL_Delay(16);
    }
    close();
}

void Game::handleEvents(SDL_Event& event) {
    if (event.type == SDL_QUIT) {
        running = false;
        return;
    }
    if (event.type == SDL_KEYDOWN) {
        jumpSound.playSound();
        SDL_Rect newPos = player.rect;
        player.isMoving = true;
        switch (event.key.keysym.sym) {
            case SDLK_DOWN:  newPos.y += PLAYER_VERTICAL_MOVE_DISTANCE; player.playerDirection = 0; player.isMovingHorizontally = false; break;
            case SDLK_LEFT:  newPos.x -= PLAYER_HORIZONTAL_MOVE_DISTANCE; player.playerDirection = 1; player.isMovingHorizontally = true; break;
            case SDLK_UP:    newPos.y -= PLAYER_VERTICAL_MOVE_DISTANCE; player.playerDirection = 2; player.isMovingHorizontally = false; break;
            case SDLK_RIGHT: newPos.x += PLAYER_HORIZONTAL_MOVE_DISTANCE; player.playerDirection = 3; player.isMovingHorizontally = true; break;
        }
        if (canMoveTo(newPos)) {
            player.rect = newPos;
        }
    }
    else if (event.type == SDL_KEYUP) {
        player.isMoving = false;
        if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_RIGHT) {
            player.isMovingHorizontally = false;
        }
    }
}

void Game::update() {
    SDL_Rect tempPos = player.rect;
    if (!player.isMoving && !player.inWater) {
        tempPos.y += mapScrollSpeed;
    }

    SDL_Rect newRaftRect = {0, 0, 0, 0};
    checkRiverAndRaftCollision(tempPos, newRaftRect);

    if (player.onRaft && player.isMovingHorizontally) {
        checkRaftCollision(tempPos, player, challenge);
    }
    player.rect = tempPos;
    currentRaftRect = player.onRaft ? newRaftRect : SDL_Rect{0, 0, 0, 0};

    if (player.onRaft && (player.rect.x <= -PLAYER_WIDTH || player.rect.x >= SCREEN_WIDTH)) {
        cout << "Game Over! Player on raft moved completely out of screen bounds!" << endl;
        reset();
        return;
    }
    if (player.rect.y >= SCREEN_HEIGHT) {
        cout << "Game Over! Player reached bottom!" << endl;
        reset();
        return;
    }
    if (checkArrowCollision()) {
        reset();
        return;
    }

    SDL_Rect playerWaterCollisionRect = {player.rect.x, player.rect.y + (PLAYER_HEIGHT - PLAYER_WATER_COLLISION_HEIGHT), PLAYER_WIDTH, PLAYER_WATER_COLLISION_HEIGHT};
    updateWaterSplash(playerWaterCollisionRect);

    updateMapScrolling();

    mapOffset = (mapOffset + mapScrollSpeed) % SCREEN_HEIGHT;
    player.update(frameCount, challenge);
    challenge.update(frameCount, mapScrollSpeed);
    frameCount++;
}

void Game::render() {
    SDL_RenderClear(renderer);
    for (int i = -1; i <= 0; i++) {
        SDL_Rect bgRect = {0, i * SCREEN_HEIGHT + mapOffset, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, background, nullptr, &bgRect);
    }
    challenge.render();
    player.render();
    if (player.inWater) {
        SDL_RenderCopy(renderer, waterSplashTexture, &waterSplashFrames[waterSplashFrameIndex], &waterSplashPos);
    }
    SDL_RenderPresent(renderer);
}

void Game::close() {
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(waterSplashTexture);
    player.destroy();
    challenge.destroy();
}

void Game::reset() {
    cout << "Resetting Game..." << endl;
    mapOffset = 0;
    mapScrollSpeed = BASE_MAP_SCROLL_SPEED;
    waterSplashFrameIndex = 0;
    waterSplashFrameCounter = 0;
    waterSplashSoundPlayed = false;
    player.reset();
    challenge.reset();
}

bool Game::canMoveTo(const SDL_Rect& newPos) {
    for (auto& obstacle : challenge.obstacles) {
        if (checkCollision(newPos, obstacle.rect)) {
            return false;
        }
    }
    if (!player.onRaft) {
        if (newPos.x < -PLAYER_WIDTH || newPos.x > SCREEN_WIDTH || newPos.y < 0) {
            return false;
        }
    }
    return true;
}

void Game::loadWaterSplashSpriteSheet() {
    for (int i = 0; i < WATER_SPLASH_FRAMES; i++) {
        waterSplashFrames[i] = {i * WATER_SPLASH_WIDTH, 0, WATER_SPLASH_WIDTH, WATER_SPLASH_HEIGHT};
    }
}

void Game::checkRiverAndRaftCollision(SDL_Rect& tempPos, SDL_Rect& newRaftRect) {
    player.onRaft = false;
    for (int i = 0; i < challenge.rivers.size(); i++) {
        if (!checkCollision(tempPos, challenge.rivers[i].rect)) continue;
        for (const auto& raft : challenge.riverRafts[i]) {
            if (checkCollision(tempPos, raft.rect)) {
                player.onRaft = true;
                newRaftRect = raft.rect;
                if (!player.isMoving) {
                    tempPos.x = raft.rect.x + (RAFT_WIDTH - PLAYER_WIDTH) / 2;
                    tempPos.y = raft.rect.y + (RAFT_HEIGHT - PLAYER_HEIGHT) / 2;
                }
                return;
            }
        }
        return;
    }
}

void Game::checkRaftCollision(SDL_Rect& newPos, const Player& player, const Challenge& challenge) {
    int moveDistance = (player.playerDirection == 1 ? -PLAYER_HORIZONTAL_MOVE_DISTANCE : PLAYER_HORIZONTAL_MOVE_DISTANCE);
    SDL_Rect nextPos = {newPos.x + moveDistance, newPos.y, PLAYER_WIDTH, PLAYER_HEIGHT};
    for (int i = 0; i < challenge.rivers.size(); i++) {
        for (const auto& raft : challenge.riverRafts[i]) {
            if (checkCollision(nextPos, raft.rect)) {
                newPos.x = raft.rect.x + (RAFT_WIDTH - PLAYER_WIDTH) / 2;
                return;
            }
        }
    }
    newPos.x += moveDistance;
}

void Game::updateWaterSplash(const SDL_Rect& playerWaterCollisionRect) {
    if (!player.inWater && !player.onRaft) {
        for (const auto& river : challenge.rivers) {
            if (checkWaterCollision(playerWaterCollisionRect, river.rect)) {
                player.inWater = true;
                waterSplashFrameIndex = 0;
                waterSplashFrameCounter = 0;
                waterSplashPos = {player.rect.x + (PLAYER_WIDTH - WATER_SPLASH_WIDTH) / 2, player.rect.y + (PLAYER_HEIGHT - WATER_SPLASH_HEIGHT) / 2, WATER_SPLASH_WIDTH, WATER_SPLASH_HEIGHT};
                break;
            }
        }
    }
    if (player.inWater) {
        if (!waterSplashSoundPlayed) {
            waterSplashSound.playSound();
            waterSplashSoundPlayed = true;
        }
        if (++waterSplashFrameCounter >= WATER_SPLASH_FRAME_DURATION) {
            waterSplashFrameCounter = 0;
            if (++waterSplashFrameIndex >= WATER_SPLASH_FRAMES) {
                cout << "Game Over! Player drowned!" << endl;
                reset();
            }
        }
    }
}

void Game::updateMapScrolling() {
    if (player.rect.y < SCREEN_HEIGHT / 2 && !player.inWater) {
        mapScrollSpeed = BASE_MAP_SCROLL_SPEED + (SCREEN_HEIGHT / 2 - player.rect.y) / 10;
        mapScrollSpeed = min(mapScrollSpeed, MAX_MAP_SCROLL_SPEED);
        player.rect.y += (SCREEN_HEIGHT / 2 - player.rect.y) / 5;
    } else {
        mapScrollSpeed = BASE_MAP_SCROLL_SPEED;
    }
}

bool Game::checkArrowCollision() {
    for (const auto& arrowRow : challenge.roadArrows) {
        for (const auto& arrow : arrowRow) {
            if (checkCollision(player.rect, arrow.rect)) {
                arrowHitSound.playSound();
                cout << "Game Over! Hit by arrow!" << endl;
                return true;
            }
        }
    }
    return false;
}