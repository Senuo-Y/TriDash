#ifndef _PLAYER_
#define _PLAYER_

#include <iostream>
#include <utility>
#include <string>
#include "../include/ObstacleLine.h"

using namespace std;

class Player {
    private:
        pair<int, int> position;
        bool moving_left = false;
        bool moving_right = false;
        int distance = 0;
        bool jumping = false;
        bool falling = false;
        int lives = MAX_LIVES;
        bool hit = false;
        float hit_timer = HIT_TIME;
        int score = 0;
        Texture2D texture;
        Texture2D state_texture;
    
    public:
        Player(Texture2D _texture, Texture2D _state_texture);

        void setPosition(int x, int y);

        pair<int, int> getPosition() const;

        void setMovingLeft(bool left);

        void setMovingRight(bool right);

        bool getIsMoving();

        void setJumping(bool jump);

        void setFalling(bool fall);

        bool getIsJumping();
        
        bool inLeftLane();

        bool inMiddleLane();

        bool inRightLane();

        int getLane();

        void stepLeft();

        void stepRight();

        bool moveComplete();

        void update(ObstacleLine obstacle_line, int &game_state);

        void draw();
};

#endif