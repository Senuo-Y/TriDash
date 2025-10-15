// First: avoid name clashes with Windows API
#if defined(_WIN32)
    #define WIN32_LEAN_AND_MEAN     // Removes rarely used APIs
    #define NOGDI                   // Prevents definition of Rectangle() macro
    #define NODRAWTEXT              // Optional: avoid text-related macros
    #define NOMINMAX                // Avoids min/max macro conflicts
    #define NOUSER                  // Prevents CloseWindow and ShowCursor
    #include <winsock2.h>
    #include <windows.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
#endif

// Now safely include raylib and alias the Rectangle struct
#include "raylib.h"
using RLRectangle = Rectangle;

// Your other includes
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

// Server settings
const char* SERVER_IP = "127.0.0.1";
const int SERVER_PORT = 65432;

// Establish a TCP connection to the server and return the socket
SOCKET connect_to_server() {
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // Create TCP socket
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT); // Convert port to network byte order
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr); // Convert IP to binary

    connect(sock, (sockaddr*)&server_addr, sizeof(server_addr)); // Connect to server
    return sock;
}

// Send a single character request to the server (e.g., 'I' for check, 'S' for stream)
void send_request(SOCKET sock, char request) {
    send(sock, &request, 1, 0); // Send 1 byte (char)
}

// Receive a 1-byte response from the server
char receive_response(SOCKET sock) {
    char response;
    recv(sock, &response, 1, 0); // Receive 1 byte
    return response;
}

// Read a full buffer from the socket (used for reading full image data)
bool receive_all(SOCKET sock, char* buffer, int size) {
    int total = 0;
    while (total < size) {
        int received = recv(sock, buffer + total, size - total, 0); // Receive remaining bytes
        if (received <= 0) return false; // If failed or connection closed
        total += received;
    }
    return true;
}

int main(int argc, char* argv[]){
    srand(time(0));
    
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "TriDash");
    InitAudioDevice();
    WSADATA wsaData;
    int wsaResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsaResult != 0) {
        cout << "WSAStartup failed: " << wsaResult << endl;
        return -1;
    }
    SOCKET socket; // Create connection
    int sock_state = -1;

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
    RLRectangle arena_source, score_source, game_mode_source;
    //RLRectangle player_source = (RLRectangle){0, 0, (float)player_tex.width, (float)player_tex.height};
    RLRectangle arena_dest = (RLRectangle){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    RLRectangle game_mode_dest = arena_dest;
    RLRectangle normal_mode_dest = (RLRectangle){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    RLRectangle score_dest = (RLRectangle){(float)SCREEN_WIDTH-score_tex.width, SCREEN_HEIGHT/20, (float)score_tex.width, (float)score_tex.height};
    RLRectangle player_dest;

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
        else if (game_state == 1) { // Choose Game Mode
            BeginDrawing();
                ClearBackground(HexToColor(0x1E2329));
                game_mode_source = (RLRectangle){0, 0, (float)game_mode_tex.width, (float)game_mode_tex.height};
                DrawTexturePro(game_mode_tex, game_mode_source, game_mode_dest, (Vector2){0, 0}, 0, White);
                back_button.draw();
                normal_mode_button.draw();
                vision_mode_button.draw();

                back_button.update(game_state);
                normal_mode_button.update(game_state);
                vision_mode_button.update(game_state);
            EndDrawing();
        }
        else if (game_state == 2) { // GAME (Normal Mode)

            arena_source = (RLRectangle){0, 0, (float)arena_tex.width, (float)arena_tex.height};
            score_source = (RLRectangle){0, 0, (float)score_tex.width, (float)score_tex.height};

            BeginDrawing();
                ClearBackground(HexToColor(0x1E2329));
                
                DrawTexturePro(arena_tex, arena_source, arena_dest, (Vector2){0, 0}, 0, White);
                DrawTexturePro(score_tex, score_source, score_dest, (Vector2){0, 0}, 0, White);
                obstacleLine.DrawObstacleLine();
                player.draw();
            EndDrawing();

            player.update(obstacleLine, game_state, -1);
            obstacleLine.update();
        }
        else if (game_state == 3) {
            if (sock_state == -1) {
                socket = connect_to_server();
                sock_state = 0;
            }
            else if (sock_state == 0) {
                send_request(socket, 'I');           // Send 'I' to ask "Is camera connected?"
                char response = receive_response(socket); // Get '0' or '1'

                if (response == '1') {
                    sock_state = 1;
                }

                BeginDrawing();
                    ClearBackground(HexToColor(0x1E2329));
                EndDrawing();
            }
            else if (sock_state == 1) {
                socket = connect_to_server();
                sock_state = 2;
            }
            else if (sock_state == 2) {
                send_request(socket, 'S');           // Send 'S' to start streaming

                uint32_t net_frame_size;
                if (!receive_all(socket, (char*)&net_frame_size, sizeof(net_frame_size))) {
                    break;
                }
                int frame_size = ntohl(net_frame_size); // Convert from network (big-endian) to host order


                // Allocate buffer to receive the JPEG-encoded frame
                std::vector<unsigned char> buffer(frame_size);
                if (!receive_all(socket, (char*)buffer.data(), frame_size))
                    break; // Exit if failed

                Image img = LoadImageFromMemory(".jpg", buffer.data(), buffer.size());
                if (img.data == nullptr) continue; // Skip if decode failed

                // Convert Image to Texture2D
                frame_texture = LoadTextureFromImage(img);
                UnloadImage(img); // Free Image memory

                BeginDrawing();
                    ClearBackground(HexToColor(0x1E2329));
                    DrawTexture(frame_texture, 0, 0, White);
                EndDrawing();
            }



            // float hand_position = -1.0;
            // int hand_lane = -1;
            // int result = recv(sock, reinterpret_cast<char*>(&hand_position), float_size, 0);
            // if (result != SOCKET_ERROR && hand_position >= 0) {
            //     if (hand_position < 0.33) {
            //         hand_lane = 0;
            //     }
            //     else if (hand_position < 0.66) {
            //         hand_lane = 1;
            //     }
            //     else{
            //         hand_lane = 2;
            //     }
            // }
            //
            // arena_source = (RLRectangle){0, 0, (float)arena_tex.width, (float)arena_tex.height};
            // score_source = (RLRectangle){0, 0, (float)score_tex.width, (float)score_tex.height};
            // //player_dest = (Rectangle){player.getPosition().first+PLAYER_WIDTH/2-player_tex.width/2, player.getPosition().second, player_tex.width, player_tex.height};

            // BeginDrawing();
            //     ClearBackground(HexToColor(0x1E2329));
            //
            //     DrawTexturePro(arena_tex, arena_source, arena_dest, (Vector2){0, 0}, 0, White);
            //     DrawTexturePro(score_tex, score_source, score_dest, (Vector2){0, 0}, 0, White);
            //     obstacleLine.DrawObstacleLine();
            //     player.draw();
            //     if (hand_lane >= 0) {
            //         DrawRectangle(hand_position*SCREEN_WIDTH, 100, 100, 100, Black);
            //     }
            // EndDrawing();
            //
            // player.update(obstacleLine, game_state, hand_lane);
            // obstacleLine.update();
        }
        else if (game_state == 4) { // Game Over
            BeginDrawing();
                ClearBackground(HexToColor(0x1E2329));
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
    CloseAudioDevice();
    CloseWindow();

}