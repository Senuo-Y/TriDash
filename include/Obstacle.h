#ifndef _OBSTACLE_
#define _OBSTACLE_

#include <iostream>
#include <utility>

using namespace std;

class Obstacle {
    private:
        pair<int, int> position;
        pair<int, int> initial_position;
        int size = 0;
        int steps = 0;
    
    public:
        Obstacle();

        Obstacle(int lane);

        void advance();

        void setToInitialPosition();

        pair<int, int> getPosition() const;

        pair<int, int> getInitialPosition() const;

        bool inLeftLane();

        bool inMiddleLane();

        bool inRightLane();

        int getLane();

        bool reachedEnd();
        
        int getSize() const;
};

#endif