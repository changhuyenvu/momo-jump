#include "challenge.h"
#include "utils.h"
#include <iostream>

Challenge::Challenge() : renderer(nullptr), rockTexture(nullptr), riverTexture(nullptr), 
    raftTexture(nullptr), arrowRoadTexture(nullptr), arrowTexture(nullptr), 
    lastRaftRect{0, 0, 0, 0}, obstacleSpawnRate(OBSTACLE_SPAWN_RATE_1) {}


bool Challenge::init(SDL_Renderer* renderer) {
    this->renderer = renderer;
    rockTexture = loadTexture("asset/Chopped_Tree.png", renderer);
    riverTexture = loadTexture("asset/water.jpg", renderer);
    raftTexture = loadTexture("asset/raft.png", renderer);
    arrowRoadTexture = loadTexture("asset/arrow_road.jpg", renderer);
    arrowTexture = loadTexture("asset/arrow.png", renderer);

    if (!rockTexture || !riverTexture || !raftTexture || !arrowRoadTexture || !arrowTexture) {
        std::cerr << "Failed to load one or more assets!" << std::endl;
        return false;
    }

    loadArrowSpriteSheet();
    reset();
    return true;
}

// Hàm update chính
void Challenge::update(int frameCount, int mapScrollSpeed) {
    spawnRiver(frameCount);
    spawnArrowRoad(frameCount);
    spawnObstacle(frameCount);
    updateArrows(mapScrollSpeed);
    updatePositions(mapScrollSpeed);
    removeOffScreen();
    // sau khoảng 4000 frames ~ 60s thì tăng mật độ vật cản lên
    if (frameCount == 4000) {
        std::cout << "Obstacle spawn rate has increased..." << std::endl;
        obstacleSpawnRate = OBSTACLE_SPAWN_RATE_2;
    }
}

void Challenge::render() {
    for (int i = 0; i < rivers.size(); ++i) {
        SDL_Rect srcRect = {0, 0, RIVER_WIDTH, rivers[i].height};
        SDL_Rect dstRect1 = {rivers[i].offsetX, rivers[i].rect.y, RIVER_WIDTH, rivers[i].height};
        SDL_Rect dstRect2 = {rivers[i].offsetX + RIVER_WIDTH, rivers[i].rect.y, RIVER_WIDTH, rivers[i].height};
        SDL_RenderCopy(renderer, riverTexture, &srcRect, &dstRect1);
        SDL_RenderCopy(renderer, riverTexture, &srcRect, &dstRect2);
        for (const auto& raft : riverRafts[i]) {
            SDL_RenderCopy(renderer, raftTexture, nullptr, &raft.rect);
        }
    }

    for (int i = 0; i < arrowRoads.size(); ++i) {
        SDL_RenderCopy(renderer, arrowRoadTexture, nullptr, &arrowRoads[i].rect);
        for (const auto& arrow : roadArrows[i]) {
            SDL_Rect frame = arrowFrames[arrow.direction == 1 ? 0 : 1];
            SDL_RenderCopy(renderer, arrowTexture, &frame, &arrow.rect);
        }
    }

    for (const auto& obstacle : obstacles) {
        SDL_RenderCopy(renderer, rockTexture, nullptr, &obstacle.rect);
    }
}

void Challenge::destroy() {
    SDL_DestroyTexture(rockTexture);
    SDL_DestroyTexture(riverTexture);
    SDL_DestroyTexture(raftTexture);
    SDL_DestroyTexture(arrowRoadTexture);
    SDL_DestroyTexture(arrowTexture);
}

void Challenge::reset() {
    obstacles.clear();
    rivers.clear();
    riverRafts.clear();
    arrowRoads.clear();
    roadArrows.clear();
    lastRaftRect = {0, 0, 0, 0};
    obstacleSpawnRate = OBSTACLE_SPAWN_RATE_1;
}
// Hàm lấy vị trí ngẫu nhiên của vật cản
bool Challenge::getValidObstaclePosition(SDL_Rect& obstacleRect) {
    std::vector<int> validXPositions;
    for (int x = 0; x <= SCREEN_WIDTH - OBSTACLE_SIZE; x += OBSTACLE_SIZE) {
        SDL_Rect tempRect = {x, -OBSTACLE_SIZE, OBSTACLE_SIZE, OBSTACLE_SIZE};
        bool valid = true;
        for (const auto& river : rivers) {
            if (checkCollision(tempRect, river.rect)) {
                valid = false;
                break;
            }
        }
        for (const auto& road : arrowRoads) {
            if (checkCollision(tempRect, road.rect)) {
                valid = false;
                break;
            }
        }
        if (valid) validXPositions.push_back(x);
    }

    if (validXPositions.empty()) return false;

    obstacleRect = {validXPositions[rand() % validXPositions.size()], -OBSTACLE_SIZE, OBSTACLE_SIZE, OBSTACLE_SIZE};
    return true;
}

// hàm kiểm tra khoảng cách tối thiểu giữa các thử thách sông/đường mũi tên trước khi sinh ra
bool Challenge::canSpawnChallenge() {
    if (rivers.empty() && arrowRoads.empty()) return true;

    int lastBottom = -MIN_CHALLENGE_GAP - 1;
    if (!rivers.empty()) lastBottom = rivers.back().rect.y;
    if (!arrowRoads.empty() && (rivers.empty() || arrowRoads.back().rect.y < rivers.back().rect.y)) {
        lastBottom = arrowRoads.back().rect.y;
    }
    return lastBottom - MIN_CHALLENGE_GAP >= 0;
}

void Challenge::loadArrowSpriteSheet() {
    arrowFrames[0] = {0, 0, ARROW_WIDTH, ARROW_HEIGHT};
    arrowFrames[1] = {ARROW_WIDTH, 0, ARROW_WIDTH, ARROW_HEIGHT};
}

// Hàm spwan sông
void Challenge::spawnRiver(int frameCount) {
    if (frameCount % RIVER_SPAWN_RATE != 0 || !canSpawnChallenge()) return;

    int numRows = (rand() % 3) + 1; // Lấy độ rộng ngẫu nhiên của sông
    int riverHeight = RAFT_HEIGHT * numRows;
    int riverTop = -riverHeight;

    std::cout << "Spawning river at y = " << riverTop << std::endl;
    rivers.push_back(River(riverTop, riverHeight));

    std::vector<Raft> rafts;
    int lastDirection = (rand() % 2 == 0) ? -1 : 1;

    for (int row = 0; row < numRows; ++row) {
        int numRafts = (rand() % 4) + 1;
        std::vector<int> positions;
        for (int i = 0; i < SCREEN_WIDTH / RAFT_WIDTH; ++i) {
            positions.push_back(i * RAFT_WIDTH);
        }

        int direction = -lastDirection;
        lastDirection = direction;

        for (int i = 0; i < numRafts && !positions.empty(); ++i) {
            int posIndex = rand() % positions.size();
            int x = positions[posIndex];
            int y = riverTop + (row * RAFT_HEIGHT);
            rafts.push_back(Raft(x, y, direction));
            positions.erase(positions.begin() + posIndex);
        }
    }
    riverRafts.push_back(rafts);
}

// hàm spwan đường để chứa mũi teen
void Challenge::spawnArrowRoad(int frameCount) {
    if (frameCount % ARROW_ROAD_SPAWN_RATE != 0 || !canSpawnChallenge()) return;

    int roadTop = -ARROW_ROAD_HEIGHT;
    std::cout << "Spawning arrow road at y = " << roadTop << std::endl;
    arrowRoads.push_back(ArrowRoad(roadTop));

    std::vector<Arrow> initialArrows;
    initialArrows.push_back(Arrow(190, roadTop + (ARROW_ROAD_HEIGHT - ARROW_HEIGHT) / 2, 1));
    initialArrows.push_back(Arrow(250, roadTop + (ARROW_ROAD_HEIGHT - ARROW_HEIGHT) / 2, -1));
    roadArrows.push_back(initialArrows);
}

// hàm spawn mũi tên trên đường
void Challenge::updateArrows(int mapScrollSpeed) {
    for (int i = 0; i < arrowRoads.size(); ++i) {
        auto& road = arrowRoads[i];
        road.arrowSpawnCounter++;

        if (road.arrowSpawnCounter >= ARROW_SPAWN_RATE) {
            road.arrowSpawnCounter = 0;
            int arrowY = road.rect.y + (ARROW_ROAD_HEIGHT - ARROW_HEIGHT) / 2;
            roadArrows[i].clear();
            roadArrows[i].push_back(Arrow(-ARROW_WIDTH, arrowY, 1)); // Mũi tên trái
            roadArrows[i].push_back(Arrow(SCREEN_WIDTH, arrowY, -1)); // Mũi tên phải
            std::cout << "Spawning new arrows at y = " << arrowY << std::endl;
        }

        for (auto it = roadArrows[i].begin(); it != roadArrows[i].end();) {
            it->rect.x += ARROW_SPEED * it->direction;
            it->rect.y += mapScrollSpeed;
            if ((it->direction == 1 && it->rect.x > SCREEN_WIDTH) || 
                (it->direction == -1 && it->rect.x + ARROW_WIDTH < 0)) {
                it = roadArrows[i].erase(it);
            } else {
                ++it;
            }
        }
        road.rect.y += mapScrollSpeed;
    }
}
// hàm spawn vật cản
void Challenge::spawnObstacle(int frameCount) {
    if (frameCount % obstacleSpawnRate != 0) return;

    SDL_Rect newRockRect;
    if (getValidObstaclePosition(newRockRect)) {
        obstacles.push_back(Entity(newRockRect.x, newRockRect.y, OBSTACLE_SIZE, OBSTACLE_SIZE));
    }
}
// hàm cập nhật vị trí các vật thể
void Challenge::updatePositions(int mapScrollSpeed) {
    for (auto& obstacle : obstacles) {
        obstacle.rect.y += mapScrollSpeed;
    }

    for (int i = 0; i < rivers.size(); ++i) {
        rivers[i].rect.y += mapScrollSpeed;
        rivers[i].offsetX -= RIVER_SPEED;
        if (rivers[i].offsetX <= -RIVER_WIDTH) {
            rivers[i].offsetX += RIVER_WIDTH;
        }
        for (auto& raft : riverRafts[i]) {
            raft.rect.y += mapScrollSpeed;
            raft.rect.x += RIVER_SPEED * raft.direction;
            if (raft.direction == -1 && raft.rect.x <= -RAFT_WIDTH) {
                raft.rect.x += RIVER_WIDTH + RAFT_WIDTH;
            } else if (raft.direction == 1 && raft.rect.x > SCREEN_WIDTH) {
                raft.rect.x -= RIVER_WIDTH + RAFT_WIDTH;
            }
        }
    }
}
// Hàm xóa các vật thể khi ra khỏi màn hìnhhình
void Challenge::removeOffScreen() {
    for (auto it = obstacles.begin(); it != obstacles.end();) {
        if (it->rect.y > SCREEN_HEIGHT) {
            it = obstacles.erase(it);
        } else {
            ++it;
        }
    }

    for (int i = 0; i < rivers.size();) {
        if (rivers[i].rect.y > SCREEN_HEIGHT) {
            rivers.erase(rivers.begin() + i);
            riverRafts.erase(riverRafts.begin() + i);
        } else {
            ++i;
        }
    }

    for (int i = 0; i < arrowRoads.size();) {
        if (arrowRoads[i].rect.y > SCREEN_HEIGHT) {
            arrowRoads.erase(arrowRoads.begin() + i);
            roadArrows.erase(roadArrows.begin() + i);
        } else {
            ++i;
        }
    }
}
