#ifndef _OBSTACLELINE_
#define _OBSTACLELINE_

#include <iostream>
#include <utility>
#include <vector>
#include "../include/Obstacle.h"

using namespace std;

class ObstacleLine {
    private:
        //int obstacle_num;
        vector<Obstacle> line;
        vector<int> indicator;
    
    public:
        ObstacleLine();

        Obstacle getObstacle(int obs);

        void update();

        bool isActive(int obs);

        void DrawObstacleLine();
};

#endif