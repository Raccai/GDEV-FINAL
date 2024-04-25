#include "raylib.h"
#include "raymath.h"
#include "Player.hpp"
#include "Enemy.hpp"

#define MAX_PLATFORMS 5
#define MAX_ENEMIES 3

void menuInit() {
    // foo
}

int main() {
    
    /*
    
        INITIALIZE GAME

    */
    
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Jumping Example");
    SetTargetFPS(60);

    // Init Player
    Vector2 playerPosition = { (float)screenWidth / 2, (float)screenHeight * 7/8 };
    float playerSize = 20.0f;
    float playerSpeed = 200.0f;
    int playerHealth = 5;
    Player player(playerPosition, {playerSize,playerSize}, playerSpeed, playerHealth); // Provide all five arguments

    Color powerBarColor = GREEN;
    Rectangle powerBarRect = { 10, screenHeight - 20, 0, 10 };

    // Init Platform
    Rectangle platforms[MAX_PLATFORMS] = {
        { 0, screenHeight - 30, screenWidth, 30 },
        { 200, screenHeight - 200, 150, 20 },
        { 400, screenHeight - 300, 150, 20 },
        { 600, screenHeight - 150, 150, 20 },
        { 100, screenHeight - 400, 150, 20 }
    };

    // Init Enemies
    Enemy enemies[MAX_ENEMIES] = {
        { (Vector2){ 100, screenHeight - 440 }, (Vector2){ ENEMY_PATROL_SPEED, 0 }, false, 0 },
        { (Vector2){ 200, screenHeight - 280 }, (Vector2){ ENEMY_PATROL_SPEED, 0 }, false, 1 },
        { (Vector2){ 700, screenHeight - 230 }, (Vector2){ ENEMY_PATROL_SPEED, 0 }, false, 2 }
    };

    /*
    
        GAME LOOP

    */

    while (!WindowShouldClose()) {
        // TODO: Check win conditions here

        // Calculate delta time
        float deltaTime = GetFrameTime();
        
        // Update Player
        player.Update(deltaTime);

        for (int i = 0; i < MAX_ENEMIES; i++) {
            UpdateEnemy(enemies[i], player, platforms, MAX_PLATFORMS, screenWidth, screenHeight);
        }

        /*
        
            DRAWING

        */

        // Draw Background
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw Platforms
        for (int i = 0; i < MAX_PLATFORMS; i++)
            DrawRectangleRec(platforms[i], GRAY);

        // Draw Power Bar
        DrawRectangleRec(powerBarRect, powerBarColor);

        // Draw Player
        DrawRectangleV(player.position, player.size, player.color);

        // Draw Enemies
        for (int i = 0; i < MAX_ENEMIES; i++)
            DrawRectangleV(enemies[i].position, ENEMY_SIZE, RED);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}