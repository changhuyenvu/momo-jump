#ifndef CHALLENGE_H
#define CHALLENGE_H

#include <SDL2/SDL.h>
#include <vector>
#include "constants.h"
#include "entity.h"
using namespace std;

struct River {
    SDL_Rect rect;
    int height;
    int offsetX;
    River(int y, int h) : rect{0, y, RIVER_WIDTH, h}, height(h), offsetX(0) {}
};

struct Raft {
    SDL_Rect rect;
    int direction;
    Raft(int x, int y, int dir) : rect{x, y, RAFT_WIDTH, RAFT_HEIGHT}, direction(dir) {}
};

struct ArrowRoad {
    SDL_Rect rect;
    int arrowSpawnCounter;
    ArrowRoad(int y) : rect{0, y, ARROW_ROAD_WIDTH, ARROW_ROAD_HEIGHT}, arrowSpawnCounter(0) {}
};

struct Arrow {
    SDL_Rect rect;
    int direction;
    Arrow(int x, int y, int dir) : rect{x, y, ARROW_WIDTH, ARROW_HEIGHT}, direction(dir) {}
};

class Challenge {

public:
    Challenge();
    bool init(SDL_Renderer* renderer);

    int obstacleSpawnRate;
    void update(int frameCount, int mapScrollSpeed);
    void render();
    void destroy();
    void reset();
    bool getValidObstaclePosition(SDL_Rect& obstacleRect);
    bool canSpawnChallenge();
    void loadArrowSpriteSheet();
    void spawnRiver(int newRiverTop);
    void spawnArrowRoad(int newRoadTop);
    void updateArrows(int mapScrollSpeed);
    void spawnObstacle(int frameCount);
    void updateRivers(int mapScrollSpeed);
    void updatePositions(int mapScrollSpeed);
    void removeOffScreen();

    SDL_Renderer* renderer;
    SDL_Texture* rockTexture;
    SDL_Texture* riverTexture;
    SDL_Texture* raftTexture;
    SDL_Texture* arrowRoadTexture;
    SDL_Texture* arrowTexture;

    vector<Entity> obstacles;
    vector<River> rivers;
    vector<vector<Raft>> riverRafts;
    vector<ArrowRoad> arrowRoads;
    vector<vector<Arrow>> roadArrows;

    SDL_Rect arrowFrames[2];

    SDL_Rect lastRaftRect;
};

#endif
