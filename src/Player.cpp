#include <iostream>
#include <string>
#include "Parameters.h"
#include "Player.h"

using namespace std;

Player::Player(Texture2D _texture, Texture2D _state_texture) {
    texture = _texture;
    state_texture = _state_texture;
    position.first = PLAYER_POSITION_X_MIDDLE_LANE;
    position.second = PLAYER_POSITION_Y;
    sound_hit =LoadSound("sfx/PlayerHit.wav");
    sound_dead = LoadSound("sfx/PlayerDead.wav");
    sound_slide = LoadSound("sfx/PlayerSlide.wav");
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
    if (distance%(2*STEP_WIDTH) == 0) {
        --sprite_index;
    }
}

void Player::stepRight() {
    setPosition(position.first+STEP_WIDTH, position.second);
    distance += STEP_WIDTH;
    if (distance%(2*STEP_WIDTH) == 0) {
        ++sprite_index;
    }
}

bool Player::moveComplete() {
    return (distance >= LANE_WIDTH+SEPARATOR_WIDTH);
}

void Player::update(ObstacleLine obstacle_line, int &game_state, int socket_lane) {
    score += 0.25;

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
                    if (!hit) { // not already hit
                        PlaySound(sound_hit);
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
        PlaySound(sound_dead);
        game_state = 4;
        lives = MAX_LIVES;
        score = 0.0;
        position.first = PLAYER_POSITION_X_MIDDLE_LANE;
        sprite_index = 5;
    }

    if (position.first == PLAYER_POSITION_X_LEFT_LANE) {
        lane = 0;
    }
    else if (position.first == PLAYER_POSITION_X_MIDDLE_LANE) {
        lane = 1;
    }
    else if (position.first == PLAYER_POSITION_X_RIGHT_LANE) {
        lane = 2;
    }

    if (!getIsMoving()) {
        if (IsKeyDown(KEY_LEFT) || (socket_lane < lane && socket_lane != -1)) {
            if (lane != 0) { // Not play sound when trying to move left and already in left lane
                PlaySound(sound_slide);
            }
            setMovingLeft(true);
        }
        else if (IsKeyDown(KEY_RIGHT) || (socket_lane > lane && socket_lane != -1)) {
            if (lane != 2) { // Not play sound when trying to move right and already in right lane
                PlaySound(sound_slide);
            }
            setMovingRight(true);
        }
        else if (IsKeyDown(KEY_SPACE)) {
            setJumping(true);
        }
    }
}

void Player::draw() {
    DrawTexturePro(texture, (Rectangle){(float)sprite_index*texture.width/11, 0, (float)texture.width/11, (float)texture.height}, (Rectangle){position.first+PLAYER_WIDTH/2-(texture.width/11)/2, position.second, texture.width/11, texture.height}, (Vector2){0, 0}, 0, White);
    DrawTexturePro(state_texture, (Rectangle){(MAX_LIVES-lives)*state_texture.width/3, 0, (float)state_texture.width/3, (float)state_texture.height}, (Rectangle){0, SCREEN_HEIGHT/20, state_texture.width/3, state_texture.height}, (Vector2){0, 0}, 0, White);
    DrawText(TextFormat("%d", int(score)), SCREEN_WIDTH*8/9-MeasureTextEx(GetFontDefault(), TextFormat("%d", int(score)), 70, 0).x, SCREEN_HEIGHT/70, 70, White);
}