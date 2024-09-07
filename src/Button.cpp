#include <iostream>
#include "../include/Parameters.h"
#include "../include/Button.h"
#include "raylib.h"

using namespace std;

Button::Button() {}

Button::Button(string _name, bool _hovered, Texture2D _texture, Vector2 _position) {
    name = _name;
    hovered = _hovered;
    texture = _texture;
    position = _position;
}

string Button::getName() {
    return name;
}

bool Button::getHovered() {
    return hovered;
}

bool Button::getClicked() {
    return clicked;
}

void Button::setHovered(bool _hovered) {
    hovered = _hovered;
}

void Button::setClicked(bool _clicked) {
    clicked = _clicked;
}

void Button::update(int &game_state) {
    Vector2 mouse_pos = GetMousePosition();
    if (position.x-texture.width/8 < mouse_pos.x && mouse_pos.x < position.x+texture.width/8 && position.y-texture.height/4 < mouse_pos.y && mouse_pos.y < position.y+texture.height/4) {
        hovered = true;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (name == "Play" || name == "Try Again") {
                game_state = 1;
            }
            else if (name == "Quit") {
                game_state = -1;
            }
        }
    }
    else {
        hovered = false;
    }
}

void Button::draw() {
    if (!hovered) {
        DrawTexturePro(texture, (Rectangle){0, 0, texture.width/2, texture.height}, (Rectangle){position.x-texture.width/4, position.y-texture.height/2, texture.width/2, texture.height}, (Vector2){0, 0}, 0, White);
    }
    else {
        DrawTexturePro(texture, (Rectangle){texture.width/2, 0, texture.width/2, texture.height}, (Rectangle){position.x-texture.width/4, position.y-texture.height/2, texture.width/2, texture.height}, (Vector2){0, 0}, 0, White);
    }
}

Button Button::operator==(Button &_button) const {

}