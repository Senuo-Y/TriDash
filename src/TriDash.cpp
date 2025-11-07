
#include "raylib.h"

#include "Parameters.h"
#include "Player.h"
#include "ObstacleLine.h"
#include "Button.h"
#include "Menu.h"

#include <cstdlib>
#include <cmath>
#include <ctime>
#include <iostream>
#include <vector>
#include <cstdint>

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
    
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "TriDash");
    InitAudioDevice();

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////// FILE LOADING //////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Load Logo
    Texture2D logo_tex = LoadTexture("assets/LogoSS.png");
    if (logo_tex.id == 0) {
        cout << "Failed to load Logo\n";
        return -1;
    }
    // Load Play Button
    Texture2D play_button_tex = LoadTexture("assets/PlayButtonSS.png");
    if (play_button_tex.id == 0) {
        cout << "Failed to load Play Button\n";
        return -1;
    }
    // Load Quit Button
    Texture2D quit_button_tex = LoadTexture("assets/QuitButtonSS.png");
    if (quit_button_tex.id == 0) {
        cout << "Failed to load Quit Button\n";
        return -1;
    }
    // Load Try Again Button
    Texture2D tryagain_button_tex = LoadTexture("assets/TryAgainButtonSS.png");
    if (tryagain_button_tex.id == 0) {
        cout << "Failed to load Try Again Button\n";
        return -1;
    }
    // Load Game Over Logo
    Texture2D gameover_button_tex = LoadTexture("assets/GameOverSS.png");
    if (gameover_button_tex.id == 0) {
        cout << "Failed to load Game Over Button\n";
        return -1;
    }
    // Load Arena
    Texture2D arena_tex = LoadTexture("assets/ArenaIM.png");
    if (arena_tex.id == 0) {
        cout << "Failed to load Arena\n";
        return -1;
    }
    // Load Player
    Texture2D player_tex = LoadTexture("assets/PlayerSS.png");
    if (player_tex.id == 0) {
        cout << "Failed to load Player\n";
        return -1;
    }
    // Load Health Bar
    Texture2D player_state_tex = LoadTexture("assets/HealthBarSS.png");
    if (player_state_tex.id == 0) {
        cout << "Failed to load Player State\n";
        return -1;
    }
    // Load Score
    Texture2D score_tex = LoadTexture("assets/ScoreIM.png");
    if (score_tex.id == 0) {
        cout << "Failed to load Score\n";
        return -1;
    }
    // Load Game Mode Image
    Texture2D game_mode_tex = LoadTexture("assets/GameModeIM.png");
    if (game_mode_tex.id == 0) {
        cout << "Failed to load Game Mode Image\n";
        return -1;
    }
    // Load Back Button
    Texture2D back_tex = LoadTexture("assets/BackButtonSS.png");
    if (back_tex.id == 0) {
        cout << "Failed to load Back Button\n";
        return -1;
    }
    // Load Normal Mode Button
    Texture2D normal_mode_tex = LoadTexture("assets/NormalModeSS.png");
    if (normal_mode_tex.id == 0) {
        cout << "Failed to load Normal Mode Button\n";
        return -1;
    }
    // Load Vision Mode Button
    Texture2D vision_mode_tex = LoadTexture("assets/VisionModeSS.png");
    if (vision_mode_tex.id == 0) {
        cout << "Failed to load Vision Mode Button\n";
        return -1;
    }
    // Score Texture
    Texture2D final_score_tex = LoadTexture("assets/ScoreSprite.png");
    if (final_score_tex.id == 0) {
        cout << "Failed to load Score Texture\n";
        return -1;
    }

    Texture2D frame_texture = {0};

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    Button play_button("Play", false, play_button_tex, (Vector2){SCREEN_WIDTH/2, SCREEN_HEIGHT*5/9});
    Button quit1_button("Quit", false, quit_button_tex, (Vector2){SCREEN_WIDTH/2, SCREEN_HEIGHT*7/9});
    vector<Button> menu_buttons;
    menu_buttons.push_back(play_button);
    menu_buttons.push_back(quit1_button);
    Menu menu(logo_tex, menu_buttons);

    Button back_button("Back", false, back_tex, (Vector2){SCREEN_WIDTH/10, (float)SCREEN_HEIGHT/10});
    Button normal_mode_button("Normal Mode", false, normal_mode_tex, (Vector2){SCREEN_WIDTH/4, (float)SCREEN_HEIGHT*5/9});
    Button vision_mode_button("Vision Mode", false, vision_mode_tex, (Vector2){(float)SCREEN_WIDTH*3/4, (float)SCREEN_HEIGHT*5/9});

    Button tryagain_button("Try Again", true, tryagain_button_tex, (Vector2){SCREEN_WIDTH/2, SCREEN_HEIGHT*6/10});
    Button quit2_button("Quit", false, quit_button_tex, (Vector2){SCREEN_WIDTH/2, SCREEN_HEIGHT*8/10});
    vector<Button> gameover_buttons;
    gameover_buttons.push_back(tryagain_button);
    gameover_buttons.push_back(quit2_button);
    Menu gameover_menu(gameover_button_tex, gameover_buttons);

    float timer = TIME;
    int index = 0;
    Rectangle arena_source, score_source, game_mode_source;
    //Rectangle player_source = (Rectangle){0, 0, (float)player_tex.width, (float)player_tex.height};
    Rectangle arena_dest = (Rectangle){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    Rectangle game_mode_dest = arena_dest;
    Rectangle normal_mode_dest = (Rectangle){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    Rectangle score_dest = (Rectangle){(float)SCREEN_WIDTH-score_tex.width, SCREEN_HEIGHT/20, (float)score_tex.width, (float)score_tex.height};
    Rectangle player_dest;

    Player player(player_tex, player_state_tex);
    int current_score;

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
        else if (game_state == 1) {
            // GAME (Normal Mode)

            arena_source = (Rectangle){0, 0, (float)arena_tex.width, (float)arena_tex.height};
            score_source = (Rectangle){0, 0, (float)score_tex.width, (float)score_tex.height};

            BeginDrawing();
            ClearBackground(HexToColor(0x1E2329));

            DrawTexturePro(arena_tex, arena_source, arena_dest, (Vector2){0, 0}, 0, White);
            DrawTexturePro(score_tex, score_source, score_dest, (Vector2){0, 0}, 0, White);
            obstacleLine.DrawObstacleLine();
            player.draw();
            EndDrawing();

            current_score = int(player.getScore());
            player.update(obstacleLine, game_state, -1);
            obstacleLine.update();
        }
        else if (game_state == 2) { // Game Over
            BeginDrawing();

                ClearBackground(HexToColor(0x1E2329));
                gameover_menu.draw(index);
                std::string text = TextFormat("Your Score: %d", current_score);
                Vector2 textSize = MeasureTextEx(GetFontDefault(), text.c_str(), 70, 6);
                DrawTextEx(GetFontDefault(), text.c_str(), { SCREEN_WIDTH/2 - textSize.x/2, SCREEN_HEIGHT * 4.0f/9.0f }, 70, 6, WHITE);
                gameover_menu.update(game_state);

            EndDrawing();
        }
    }

    UnloadTexture(logo_tex);
    UnloadTexture(play_button_tex);
    UnloadTexture(quit_button_tex);
    UnloadTexture(arena_tex);
    UnloadTexture(player_tex);
    CloseAudioDevice();
    CloseWindow();
}