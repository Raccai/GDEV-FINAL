#include "raylib.h"
#include "raymath.h"
#include "Player.hpp"
#include "Enemy.hpp"

#define MAX_PLATFORMS 5
#define MAX_ENEMIES 3

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

    Color powerBarColor = GREEN;
    Rectangle powerBarRect = { 10, screenHeight - 20, 0, 10 };

    Enemy enemies[MAX_ENEMIES] = {
        { (Vector2){ 100, screenHeight - 440 }, (Vector2){ ENEMY_PATROL_SPEED, 0 }, false, 0 },
        { (Vector2){ 200, screenHeight - 280 }, (Vector2){ ENEMY_PATROL_SPEED, 0 }, false, 1 },
        { (Vector2){ 700, screenHeight - 230 }, (Vector2){ ENEMY_PATROL_SPEED, 0 }, false, 2 }
    };

    while (!WindowShouldClose()) {
        UpdatePlayer(player, platforms, MAX_PLATFORMS, powerBarRect, powerBarColor, screenWidth, screenHeight);

        for (int i = 0; i < MAX_ENEMIES; i++) {
            UpdateEnemy(enemies[i], player, platforms, MAX_PLATFORMS, screenWidth, screenHeight);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int i = 0; i < MAX_PLATFORMS; i++) {
            DrawRectangleRec(platforms[i], GRAY);
        }

        DrawRectangleRec(powerBarRect, powerBarColor);

        DrawRectangleV(player.position, PLAYER_SIZE, DARKBLUE);

        for (int i = 0; i < MAX_ENEMIES; i++) {
            DrawRectangleV(enemies[i].position, ENEMY_SIZE, RED);
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}