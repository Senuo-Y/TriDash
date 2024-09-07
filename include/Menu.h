#ifndef _MENU_
#define _MENU_

#include <iostream>
#include "../include/Button.h"
#include <vector>

using namespace std;

class Menu {
    private:
        bool first_menu = true;
        bool open = true;
        Texture2D logo_tex;
        vector<Button> buttons;
    
    public:
        Menu();

        Menu(Texture2D _logo_tex, vector<Button> _buttons);

        //void setHoveredButton(Button &button);

        void update(int &game_state);

        void draw(int index);
};

#endif