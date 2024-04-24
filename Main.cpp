#include "raylib.h"
#include "raymath.h"
#include "Player.hpp"

#define MAX_PLATFORMS 5

int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Jumping Example");
    SetTargetFPS(60);

    Player player = { (Vector2){ screenWidth / 2 - PLAYER_SIZE.x / 2, screenHeight / 2 - PLAYER_SIZE.y / 2 }, 
                      (Vector2){ 0, 0 }, true, true, 0.0f, false };

    Rectangle platforms[MAX_PLATFORMS] = {
        { 0, screenHeight - 30, screenWidth, 30 },
        { 200, screenHeight - 200, 150, 20 },
        { 400, screenHeight - 300, 150, 20 },
        { 600, screenHeight - 150, 150, 20 },
        { 100, screenHeight - 400, 150, 20 }
    };

    Color powerBarColor = GREEN; // Power bar color
    Rectangle powerBarRect = { 10, screenHeight - 20, 0, 10 }; // Power bar rectangle

    while (!WindowShouldClose()) {
        // Update
        UpdatePlayer(player, platforms, MAX_PLATFORMS, powerBarRect, powerBarColor, screenWidth, screenHeight);

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw platforms
        for (int i = 0; i < MAX_PLATFORMS; i++) {
            DrawRectangleRec(platforms[i], GRAY);
        }

        // Draw power bar
        DrawRectangleRec(powerBarRect, powerBarColor);

        // Draw player
        DrawRectangleV(player.position, PLAYER_SIZE, DARKBLUE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}