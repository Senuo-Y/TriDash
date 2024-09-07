#include <iostream>
#include "../include/Parameters.h"
#include "../include/ObstacleLine.h"
#include "../include/Obstacle.h"
#include "raylib.h"

using namespace std;

ObstacleLine::ObstacleLine() {
    int random_lane;
    for (int i = 0; i < OBSTACLE_NUM; ++i) {
        line.push_back(Obstacle(i));
        indicator.push_back(0);
    }
    for (int i = 0; i < ACTIVE_OBSTACLE_NUM; ++i) {
        random_lane = rand() % OBSTACLE_NUM;
        indicator[random_lane] = 1; /////////////////////////////
    }



    /*obstacle_num = obstacle_num_;
    line.resize(obstacle_num);
    int random_lane;
    for (int i = 0; i < obstacle_num; ++i) {
        random_lane = rand() % 3 + 1;
        line[i] = Obstacle(random_lane);
    }*/
}

Obstacle ObstacleLine::getObstacle(int obs) {
    if (0 <= obs && obs < OBSTACLE_NUM) {
        return line[obs];
    }
    return -1;
}

void ObstacleLine::update() {
    bool reset = false;
    int random_lane;

    for (int i = 0; i < OBSTACLE_NUM && !reset; ++i) {
        if (line[i].getPosition().second-line[i].getSize() >= SCREEN_HEIGHT) {
            reset = true;
        }
    }
    if (reset) {
        for (int i = 0; i < OBSTACLE_NUM; ++i) {
            line[i].setToInitialPosition();
            indicator[i] = 0;
        }
        for (int i = 0; i < ACTIVE_OBSTACLE_NUM; ++i) {
            random_lane = rand() % OBSTACLE_NUM;
            indicator[random_lane] = 1;
        }
    }
    else {
        for (int i = 0; i < OBSTACLE_NUM && !reset; ++i) {
            if (indicator[i] == 1) {
                line[i].advance();
            }
        }
    }

}

bool ObstacleLine::isActive(int obs) {
    if (0 <= obs && obs < OBSTACLE_NUM) {
        return indicator[obs];
    }
    return -1;
}

void ObstacleLine::DrawObstacleLine() {
    Rectangle obstacle_source;
    Rectangle obstacle_dest;
    for (int i = 0; i < OBSTACLE_NUM; ++i) {
        if (indicator[i] == 1) {
            DrawCircle(line[i].getPosition().first, line[i].getPosition().second, line[i].getSize(), Black);
        }
    }
}