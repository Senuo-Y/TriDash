#include <iostream>
#include "../include/Parameters.h"
#include "../include/Menu.h"
#include "raylib.h"

using namespace std;

Menu::Menu() {}

Menu::Menu(Texture2D _logo_tex, vector<Button> _buttons) {
    logo_tex = _logo_tex;
    buttons = _buttons;
}

/*void Menu::setHoveredButton(Button &button) {
    for (int i = 0; i < buttons.size(); ++i) {
        if (buttons[i] == button) {
            buttons[i].setHovered(true);
        }
        else {
            buttons[i].setHovered(false);
        }
    }
}*/

void Menu::update(int &game_state) {
    Vector2 mouse_pos = GetMousePosition();
    for (int i = 0; i < buttons.size(); ++i) {
        buttons[i].update(game_state);
    }
}

void Menu::draw(int index) {
    DrawTexturePro(logo_tex, (Rectangle){index*logo_tex.width/2, 0, logo_tex.width/2, logo_tex.height}, (Rectangle){SCREEN_WIDTH/2-logo_tex.width/4, SCREEN_HEIGHT/4-logo_tex.height/2, logo_tex.width/2, logo_tex.height}, (Vector2){0, 0}, 0, White);
    for (int i = 0; i < buttons.size(); ++i) {
        buttons[i].draw();
    }
}