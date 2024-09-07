#ifndef _PARAMETERS_
#define _PARAMETERS_

#include <cmath>
#include "raylib.h"

const Color Green1 = {31, 234, 0, 255};
const Color Green2 = {39, 161, 2, 255};
const Color Green3 = {28, 96, 0, 255};
const Color Green4 = {0, 47, 6, 255};
const Color Black = {0, 0, 0, 255};
const Color White = {255, 255, 255, 255};
const Color BlueGreen = {28, 87, 93, 255};

const int SCREEN_HEIGHT = 1080;
const int SCREEN_WIDTH = 1920;
const int FPS = 60;
const float TIME = 0.5;

const int BACKGROUND_FRAMES = 5;
const int BG_WIDTH = 1920;
const int BG_HEIGHT = 1080;

const int PLAYER_HEIGHT = SCREEN_HEIGHT/5;
const int PLAYER_WIDTH = SCREEN_WIDTH/10;

const int LANE_WIDTH = SCREEN_WIDTH/4;
const int SEPARATOR_WIDTH = (SCREEN_WIDTH-3*LANE_WIDTH)/4;

const int PLAYER_POSITION_X_LEFT_LANE = SEPARATOR_WIDTH + (LANE_WIDTH/2-PLAYER_WIDTH/2);
const int PLAYER_POSITION_X_MIDDLE_LANE = 2*SEPARATOR_WIDTH + LANE_WIDTH + (LANE_WIDTH/2-PLAYER_WIDTH/2);
const int PLAYER_POSITION_X_RIGHT_LANE = 3*SEPARATOR_WIDTH + 2*LANE_WIDTH + (LANE_WIDTH/2-PLAYER_WIDTH/2);
const int PLAYER_POSITION_Y = SCREEN_HEIGHT*4/5;
const int MAX_LIVES = 3;
const int HIT_TIME = 1; // 1 second

const int STEP_WIDTH = (LANE_WIDTH+SEPARATOR_WIDTH)/10;
const int JUMP_STEPS = 12;

const int OBSTACLE_NUM = 3;
const int ACTIVE_OBSTACLE_NUM = 2;
const int OBSTACLE_MAX_RADIUS = SCREEN_WIDTH/20;
const int OBSTACLE_INITIAL_POSITION_X_LEFT = 2*SEPARATOR_WIDTH+LANE_WIDTH+SEPARATOR_WIDTH/2;
const int OBSTACLE_INITIAL_POSITION_X_MIDDLE = SCREEN_WIDTH/2;
const int OBSTACLE_INITIAL_POSITION_X_RIGHT = SCREEN_WIDTH-OBSTACLE_INITIAL_POSITION_X_LEFT;
const int OBSTACLE_INITIAL_POSITION_Y = SCREEN_HEIGHT*12/25;
const int OBSTACLE_STEPS = 500;
const int OBSTACLE_STEP_WIDTH = (SCREEN_HEIGHT*5/6)/OBSTACLE_STEPS;
const int OBSTACLE_STEP_LATERAL_WIDTH = (OBSTACLE_INITIAL_POSITION_X_LEFT - PLAYER_POSITION_X_LEFT_LANE+PLAYER_WIDTH/2)/(OBSTACLE_STEPS/8);

const int DIAGONAL_X = OBSTACLE_INITIAL_POSITION_X_LEFT - PLAYER_POSITION_X_LEFT_LANE;
const int DIAGONAL_Y = PLAYER_POSITION_Y-OBSTACLE_INITIAL_POSITION_Y;
const int DIAGONAL = sqrt(pow(DIAGONAL_X, 2)+pow(DIAGONAL_Y, 2));

#endif