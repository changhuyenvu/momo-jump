#ifndef CONSTANTS_H
#define CONSTANTS_H

const int SCREEN_WIDTH = 440;
const int SCREEN_HEIGHT = 660;
const int PLAYER_WIDTH = 59;
const int PLAYER_HEIGHT = 37;
const int SPRITE_SHEET_COLS = 5;
const int SPRITE_SHEET_ROWS = 4;
const int PLAYER_VERTICAL_MOVE_DISTANCE = 61; // khaongr cách di chuyển dọc
const int PLAYER_HORIZONTAL_MOVE_DISTANCE = 33; // khoảng cách di chuyển ngang
const int BASE_MAP_SCROLL_SPEED = 1; // tốc độ cuộn map cơ bản
const int MAX_MAP_SCROLL_SPEED = 10; // tốc độ cuộn map tối đa
const int OBSTACLE_SIZE = 35;
const int OBSTACLE_SPAWN_RATE_1 = 60;
const int OBSTACLE_SPAWN_RATE_2 = 50;
const int RIVER_SPAWN_RATE = 200;
const int MIN_CHALLENGE_GAP = 60;
const int ARROW_ROAD_WIDTH = 440;
const int ARROW_ROAD_HEIGHT = 60;
const int ARROW_SPAWN_RATE = 60;
const int ARROW_SPEED = 12;
const int ARROW_WIDTH = 49;
const int ARROW_HEIGHT = 12;
const int ARROW_ROAD_SPAWN_RATE = 100;
const int RIVER_WIDTH = 440;
const int RIVER_FULL_HEIGHT = 220;
const int RIVER_SPEED = 1;
const int RAFT_WIDTH = 68;
const int RAFT_HEIGHT = 59;
const int WATER_SPLASH_WIDTH = 59;
const int WATER_SPLASH_HEIGHT = 37;
const int WATER_SPLASH_FRAMES = 8;
const int WATER_SPLASH_FRAME_DURATION = 5;
const int PLAYER_WATER_COLLISION_HEIGHT = PLAYER_HEIGHT / 2;
const float WATER_COLLISION_THRESHOLD = 0.6f; //ngưỡng va cham tối thiểu với sông

#endif