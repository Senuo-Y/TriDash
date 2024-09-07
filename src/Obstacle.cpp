#include <iostream>
#include "../include/Parameters.h"
#include "../include/Obstacle.h"
#include <cmath>

using namespace std;

Obstacle::Obstacle() {}

Obstacle::Obstacle(int lane) {
    if (lane == 0 || lane == 1 || lane == 2) {
        if (lane == 0) {
            position.first = OBSTACLE_INITIAL_POSITION_X_LEFT;
        }
        else if (lane == 1) {
            position.first = OBSTACLE_INITIAL_POSITION_X_MIDDLE;
        }
        else {
            position.first = OBSTACLE_INITIAL_POSITION_X_RIGHT;
        }
        position.second = OBSTACLE_INITIAL_POSITION_Y;
        initial_position = position;
    }
}

void Obstacle::advance() {
    int random_lane;
    /*if (position.second-size >= SCREEN_HEIGHT) {
        steps = 0;
        size = 0;
        random_lane = rand() % 3 + 1;
        if (random_lane == 1) {
            position.first = OBSTACLE_INITIAL_POSITION_X_LEFT;
        }
        else if (random_lane == 2) {
            position.first = OBSTACLE_INITIAL_POSITION_X_MIDDLE;
        }
        else if (random_lane == 3) {
            position.first = OBSTACLE_INITIAL_POSITION_X_RIGHT;
        }
        position.second = OBSTACLE_INITIAL_POSITION_Y;
        initial_position = position;
    }
    else {*/
    ++steps;
    if (size < OBSTACLE_MAX_RADIUS) {
        ++size;
    }
    if (initial_position.first == OBSTACLE_INITIAL_POSITION_X_LEFT) {
        position.first -= OBSTACLE_STEP_LATERAL_WIDTH;
    }
    else if (initial_position.first == OBSTACLE_INITIAL_POSITION_X_RIGHT) {
        position.first += OBSTACLE_STEP_LATERAL_WIDTH;
    }
    position.second += (steps/3)*OBSTACLE_STEP_WIDTH; /////////
    //}
}

void Obstacle::setToInitialPosition() {
    position = initial_position;
    size = 0;
    steps = 0;
}

pair<int, int> Obstacle::getPosition() const {
    return position;
}

pair<int, int> Obstacle::getInitialPosition() const {
    return initial_position;
}

bool Obstacle::inLeftLane() {
    return (position.first <= OBSTACLE_INITIAL_POSITION_X_LEFT);
}

bool Obstacle::inMiddleLane() {
    return (position.first == OBSTACLE_INITIAL_POSITION_X_MIDDLE);
}

bool Obstacle::inRightLane() {
    return (position.first >= OBSTACLE_INITIAL_POSITION_X_RIGHT);
}

int Obstacle::getLane() {
    if (inLeftLane()) {
        return 0;
    }
    else if (inMiddleLane()) {
        return 1;
    }
    else if (inRightLane()) {
        return 2;
    }
    return -1;
}

bool Obstacle::reachedEnd() {
    return (position.second-size >= SCREEN_HEIGHT);
}

int Obstacle::getSize() const {
    return size;
}
