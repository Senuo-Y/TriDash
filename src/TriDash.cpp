#include "raylib.h"
#include "Parameters.h"
#include "Player.h"
#include "Obstacle.h"
#include "ObstacleLine.h"
#include "Button.h"
#include "Menu.h"
#include <cstdlib>
#include <cmath>
#include <time.h>
#include <utility>

// Function to convert hex color to Raylib's Color struct
Color HexToColor(int hexValue) {
    Color color;
    color.r = (hexValue >> 16) & 0xFF;  // Extract Red
    color.g = (hexValue >> 8) & 0xFF;   // Extract Green
    color.b = (hexValue) & 0xFF;        // Extract Blue
    color.a = 255;                      // Full opacity
    return color;
}

int main(int argc, char* argv[]){
    srand(time(0));
    
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OperationRUN");

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////// FILE LOADING //////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Load Logo
    Texture2D logo_tex = LoadTexture("images/LogoSS.png");
    if (logo_tex.id == 0) {
        cout << "Failed to load Logo\n";
        return -1;
    }
    // Load Play Button
    Texture2D play_button_tex = LoadTexture("images/PlayButtonSS.png");
    if (play_button_tex.id == 0) {
        cout << "Failed to load Play Button\n";
        return -1;
    }
    // Load Options Button
    /*Texture2D options_button_tex = LoadTexture("images/OptionsButtonSpriteSheet.png");
    if (options_button_tex.id == 0) {
        cout << "Failed to load Options Button\n";
        return -1;
    }*/
    // Load Quit Button
    Texture2D quit_button_tex = LoadTexture("images/QuitButtonSS.png");
    if (quit_button_tex.id == 0) {
        cout << "Failed to load Quit Button\n";
        return -1;
    }
    // Load Try Again Button
    Texture2D tryagain_button_tex = LoadTexture("images/TryAgainButtonSpriteSheet.png");
    if (tryagain_button_tex.id == 0) {
        cout << "Failed to load Try Again Button\n";
        return -1;
    }
    // Load Game Over Logo
    Texture2D gameover_button_tex = LoadTexture("images/GameOverSpriteSheet.png");
    if (gameover_button_tex.id == 0) {
        cout << "Failed to load Game Over Button\n";
        return -1;
    }
    // Load Arena
    Texture2D arena_tex = LoadTexture("images/ArenaIM.png");
    if (arena_tex.id == 0) {
        cout << "Failed to load Arena\n";
        return -1;
    }
    // Load Player
    Texture2D player_tex = LoadTexture("images/PlayerSS.png");
    if (player_tex.id == 0) {
        cout << "Failed to load Player\n";
        return -1;
    }
    // Load Health Bar
    Texture2D player_state_tex = LoadTexture("images/HealthBarSS.png");
    if (player_state_tex.id == 0) {
        cout << "Failed to load Player State\n";
        return -1;
    }
    // Load Score
    Texture2D score_tex = LoadTexture("images/ScoreIM.png");
    if (score_tex.id == 0) {
        cout << "Failed to load Player State\n";
        return -1;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    Button play_button("Play", false, play_button_tex, (Vector2){SCREEN_WIDTH/2, SCREEN_HEIGHT*5/9});
    //Button options_button("Options", false, options_button_tex, (Vector2){SCREEN_WIDTH/2, SCREEN_HEIGHT*6/8});
    Button quit1_button("Quit", false, quit_button_tex, (Vector2){SCREEN_WIDTH/2, SCREEN_HEIGHT*7/9});
    vector<Button> menu_buttons;
    menu_buttons.push_back(play_button);
    //buttons.push_back(options_button);
    menu_buttons.push_back(quit1_button);
    Menu menu(logo_tex, menu_buttons);

    Button tryagain_button("Try Again", true, tryagain_button_tex, (Vector2){SCREEN_WIDTH/2, SCREEN_HEIGHT*6/10});
    Button quit2_button("Quit", false, quit_button_tex, (Vector2){SCREEN_WIDTH/2, SCREEN_HEIGHT*8/10});
    vector<Button> gameover_buttons;
    gameover_buttons.push_back(tryagain_button);
    gameover_buttons.push_back(quit2_button);
    Menu gameover_menu(gameover_button_tex, gameover_buttons);

    float timer = TIME;
    int index = 0;
    Rectangle arena_source, score_source;
    //Rectangle player_source = (Rectangle){0, 0, (float)player_tex.width, (float)player_tex.height};
    Rectangle arena_dest = (Rectangle){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    Rectangle score_dest = (Rectangle){SCREEN_WIDTH-score_tex.width, SCREEN_HEIGHT/20, score_tex.width, score_tex.height};
    Rectangle player_dest;

    Player player(player_tex, player_state_tex);

    //int obstacle_num = rand() % 3 + 1;
    int obstacle_num = 3;
    ObstacleLine obstacleLine;

    int game_state = 0; // menu

    SetTargetFPS(FPS);
    ToggleFullscreen();
    
    while (!WindowShouldClose() && game_state != -1) {
        timer -= GetFrameTime();
        if (timer < 0) {
            timer = TIME;
            index = (index + 1) % 2;
        }

        if (game_state == 0) { // Title Menu
            BeginDrawing();
                ClearBackground(HexToColor(0x1E2329));
                
                menu.draw(index);
                menu.update(game_state);
            EndDrawing();
        }
        else if (game_state == 1) { // Game
            arena_source = (Rectangle){0, 0, (float)arena_tex.width, (float)arena_tex.height};
            score_source = (Rectangle){0, 0, (float)score_tex.width, (float)score_tex.height};
            //player_dest = (Rectangle){player.getPosition().first+PLAYER_WIDTH/2-player_tex.width/2, player.getPosition().second, player_tex.width, player_tex.height};

            BeginDrawing();
                ClearBackground(White);
                
                DrawTexturePro(arena_tex, arena_source, arena_dest, (Vector2){0, 0}, 0, White);
                DrawTexturePro(score_tex, score_source, score_dest, (Vector2){0, 0}, 0, White);
                obstacleLine.DrawObstacleLine();
                player.draw();
            EndDrawing();

            player.update(obstacleLine, game_state);
            obstacleLine.update();
        }
        else if (game_state == 2) { // Game Over
            BeginDrawing();
                ClearBackground(Black);
                //DrawText("YEAH YOU LOST", SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 20, White);
                gameover_menu.draw(index);
                gameover_menu.update(game_state);
            EndDrawing();
        }
    }

    UnloadTexture(logo_tex);
    UnloadTexture(play_button_tex);
    //UnloadTexture(options_button_tex);
    UnloadTexture(quit_button_tex);
    UnloadTexture(arena_tex);
    UnloadTexture(player_tex);
    CloseWindow();

}