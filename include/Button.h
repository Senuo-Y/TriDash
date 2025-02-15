#ifndef _BUTTON_
#define _BUTTON_

#include <iostream>
#include <vector>
#include <raylib.h>

using namespace std;

class Button {
    private:
        string name;
        bool hovered;
        bool clicked = false;
        Texture2D texture;
        Vector2 position;
    
    public:
        Button();

        Button(string _name, bool _hovered, Texture2D _texture, Vector2 _position);

        string getName();

        bool getHovered();

        bool getClicked();

        void setHovered(bool _hovered);

        void setClicked(bool _clicked);

        void update(int &game_state);

        void draw();

        Button operator==(Button &_button) const;
};

#endif