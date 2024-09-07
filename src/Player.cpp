#include <iostream>
#include <string>
#include "../include/Parameters.h"
#include "../include/Player.h"

using namespace std;

Player::Player(Texture2D _texture, Texture2D _state_texture) {
    texture = _texture;
    state_texture = _state_texture;
    position.first = PLAYER_POSITION_X_MIDDLE_LANE;
    position.second = PLAYER_POSITION_Y;
}

void Player::setPosition(int x, int y) {
    position.first = x;
    position.second = y;
}

pair<int, int> Player::getPosition() const {
    return position;
}

void Player::setMovingLeft(bool left) {
    moving_left = left;
}

void Player::setMovingRight(bool right) {
    moving_right = right;
}

bool Player::getIsMoving() {
    return (moving_left || moving_right || jumping || falling);
}

void Player::setJumping(bool jump) {
    jumping = jump;
}

bool Player::getIsJumping() {
    return jumping;
}

void Player::setFalling(bool fall) {
    falling = fall;
}

bool Player::inLeftLane() {
    return (SEPARATOR_WIDTH <= position.first && position.first <= SEPARATOR_WIDTH+LANE_WIDTH);
}

bool Player::inMiddleLane() {
    return (2*SEPARATOR_WIDTH+LANE_WIDTH <= position.first && position.first <= 2*SEPARATOR_WIDTH+2*LANE_WIDTH);
}

bool Player::inRightLane() {
    return (3*SEPARATOR_WIDTH+2*LANE_WIDTH <= position.first && position.first <= SCREEN_WIDTH-SEPARATOR_WIDTH);
}

int Player::getLane() {
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

void Player::stepLeft() {
    setPosition(position.first-STEP_WIDTH, position.second);
    distance += STEP_WIDTH;
}

void Player::stepRight() {
    setPosition(position.first+STEP_WIDTH, position.second);
    distance += STEP_WIDTH;
}

bool Player::moveComplete() {
    return (distance >= LANE_WIDTH+SEPARATOR_WIDTH);
}

void Player::update(ObstacleLine obstacle_line, int &game_state) {
    ++score;

    if (jumping) {
        if (!falling) {
            if (distance < JUMP_STEPS) {
                setPosition(position.first, position.second-((JUMP_STEPS-distance)*2));
                ++distance;
            }
            else if (distance == JUMP_STEPS) {
                distance = 0;
                setFalling(true);
            }
        }
        else {
            if (distance < JUMP_STEPS) {
                setPosition(position.first, position.second+(distance+1)*2);
                ++distance;
            }
            else if (distance == JUMP_STEPS) {
                distance = 0;
                setFalling(false);
                setJumping(false);
            }
        }
    }

    if (moving_left) {
        if (position.first == PLAYER_POSITION_X_LEFT_LANE) {
            setMovingLeft(false);
        }
        else {
            stepLeft();
        }
    }
    else if (moving_right) {
        if (position.first == PLAYER_POSITION_X_RIGHT_LANE) {
            setMovingRight(false);
        }
        else {
            stepRight();
        }
    }

    if (moveComplete()) {
        setMovingLeft(false);
        setMovingRight(false);
        distance = 0;
        if (inLeftLane()) {
            position.first = PLAYER_POSITION_X_LEFT_LANE;
        }
        else if (inMiddleLane()) {
            position.first = PLAYER_POSITION_X_MIDDLE_LANE;
        }
        else if (inRightLane()) {
            position.first = PLAYER_POSITION_X_RIGHT_LANE;
        }
    }

    for (int i = 0; i < OBSTACLE_NUM && !hit; ++i) {
        if ((obstacle_line.isActive(i)) && (getLane() == obstacle_line.getObstacle(i).getLane())) {
            if ((PLAYER_POSITION_Y + PLAYER_HEIGHT/2) <= obstacle_line.getObstacle(i).getPosition().second && obstacle_line.getObstacle(i).getPosition().second <= (PLAYER_POSITION_Y + PLAYER_HEIGHT)) {
                if (!jumping) { // not jumping over obstacle
                    if (!hit) {
                        --lives;
                        hit = true;
                    }
                }
            }
        }
    }

    if (hit) {
        hit_timer -= GetFrameTime();
        if (hit_timer < 0) {
            hit = false;
            hit_timer = HIT_TIME;
        }
    }

    if (lives == 0) {
        game_state = 2;
        lives = MAX_LIVES;
        score = 0;
        position.first = PLAYER_POSITION_X_MIDDLE_LANE;
    }

    if (IsKeyDown(KEY_LEFT) && !getIsMoving()) {
        setMovingLeft(true);
    }
    else if (IsKeyDown(KEY_RIGHT) && !getIsMoving()) {
        setMovingRight(true);
    }
    else if (IsKeyDown(KEY_SPACE) && !getIsMoving()) {
        setJumping(true);
    }
}

void Player::draw() {
    DrawTexturePro(texture, (Rectangle){(MAX_LIVES-lives)*texture.width/3, 0, (float)texture.width/3, (float)texture.height}, (Rectangle){position.first+PLAYER_WIDTH/2-texture.width/6, position.second, texture.width/3, texture.height}, (Vector2){0, 0}, 0, White);
    DrawTexturePro(state_texture, (Rectangle){(MAX_LIVES-lives)*state_texture.width/3, 0, (float)state_texture.width/3, (float)state_texture.height}, (Rectangle){0, SCREEN_HEIGHT/10, state_texture.width/3, state_texture.height}, (Vector2){0, 0}, 0, White);
    DrawText(TextFormat("%d", score), SCREEN_WIDTH/2-MeasureTextEx(GetFontDefault(), TextFormat("%d", score), 100, 0).x/2, SCREEN_HEIGHT/5, 100, White);
}