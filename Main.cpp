#include "iostream"
#include "fstream"
#include "sstream"

#include "raylib.h"
#include "raymath.h"

#include "Player.hpp"
#include "Enemy.hpp"
#include "Cloud.hpp" // include Cloud header

#define MAX_PLATFORMS 5
#define MAX_ENEMIES 3
#define MAX_CLOUDS 3

struct Level {
    std::string image_name;
    std::vector<Rectangle> tiles;
    std::vector<std::vector<int>> grid;
    std::vector<int> impassable; // to edit @whoevernext
    int grid_width, grid_height;
};

Level loadMap(std::string map_config) {
    Level level;
    
    // Parse level file
    std::ifstream configFile("level/" + map_config);
    std::string line;
    while (std::getline(configFile, line)) {
        // sstream from: https://www.geeksforgeeks.org/stringstream-c-applications/
        std::istringstream iss(line);
        std::string key;
        iss >> key;
        if (key == "IMAGE_NAME") {
            iss >> level.image_name;
            level.image_name = "level/" + level.image_name;
        } else if (key == "TILE_COUNT") {
            int tileCount;
            iss >> tileCount;
            level.tiles.resize(tileCount);
            for (int i = 0; i < tileCount; ++i) {
                std::getline(configFile, line);
                std::istringstream tileStream(line);
                int x, y, width, height;
                tileStream >> x >> y >> width >> height;
                level.tiles[i] = { (float)x, (float)y, (float)width, (float)height };
            }
        } else if (key == "IMPASSABLE") {
            int impassableCount;
            iss >> impassableCount;
            level.impassable.resize(impassableCount);
            for (int i = 0; i < impassableCount; ++i) {
                configFile >> level.impassable[i];
            }
        } else if (key == "GRID") {
            iss >> level.grid_width >> level.grid_height;
            level.grid.resize(level.grid_height, std::vector<int>(level.grid_width));
            for (int i = 0; i < level.grid_height; ++i) {
                for (int j = 0; j < level.grid_width; ++j) {
                    configFile >> level.grid[i][j];
                }
            }
        }
    }

    return level;
}

int main() {
    
    /*
    
        INITIALIZE GAME

    */
    
    const int screenWidth = 960;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "Final Project - Layug, Marcelo, Laurel");

    SetTargetFPS(60);
    std::string map_config = "level2.txt";

    // Variables from config
    Level level = loadMap(map_config);

    // Init Tiles
    Texture2D texture = LoadTexture(level.image_name.c_str());
    float tileSize = 60.0f;

    // Init Player
    Vector2 playerPosition = { (float)screenWidth / 2, (float)screenHeight * 7/8 };
    float playerSize = 20.0f;
    float playerSpeed = 200.0f;
    int playerHealth = 5;
    Player player(playerPosition, {playerSize,playerSize}, playerSpeed, playerHealth); // Provide all five arguments

    // Init Power Bar
    Color powerBarColor = GREEN;
    Rectangle powerBarRect = { 10, screenHeight - 20, 0, 10 };

    // Init Platforms
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

    // Init clouds
    Cloud clouds[MAX_CLOUDS] = {
        { (Vector2){ 350, 150 }, { { 300, 50 }, { 400, 0 }, { 500, 70 }, { 600, 100 } }, 0.0f, 0.1f, 20, BLUE, LoadTexture("textures/Cloud2.png") },
        { (Vector2){ 100, 230 }, { { 120, 300 }, { 170, 250 }, { 200, 320 }, { 250, 340 } }, 0.0f, 0.1f, 15, BLUE, LoadTexture("textures/Cloud3.png") },
        { (Vector2){ 550, 410 }, { { 450, 190 }, { 550, 120 }, { 600, 210 }, { 650, 180 } }, 0.0f, 0.1f, 15, BLUE, LoadTexture("textures/Cloud1.png") },
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

        // Update enemies
        for (int i = 0; i < MAX_ENEMIES; i++) {
            UpdateEnemy(enemies[i], player, platforms, MAX_PLATFORMS, screenWidth, screenHeight);
        }

        // Update clouds
        for (int i = 0; i < MAX_CLOUDS; i++) {
            UpdateCloud(clouds[i], GetFrameTime());
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

        // Draw clouds
        for (int i = 0; i < MAX_CLOUDS; i++) {
            DrawCloud(clouds[i]);
        }

        EndDrawing();
    }

    // Unload textures
    for (int i = 0; i < MAX_CLOUDS; i++) {
        UnloadTexture(clouds[i].texture);
    }

    CloseWindow();

    return 0;
}