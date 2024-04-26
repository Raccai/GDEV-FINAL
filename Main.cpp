#include "iostream"
#include "fstream"
#include "sstream"

#include "raylib.h"
#include "raymath.h"

#include "Player.hpp"
#include "Enemy.hpp"
#include "Cloud.hpp" // include Cloud header
#include "GameUI.hpp"

#include <vector>

#define MAX_PLATFORMS 5
#define MAX_ENEMIES 5
#define MAX_CLOUDS 4

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 720

struct Level {
    std::string image_name;
    std::string background_name;
    std::vector<Rectangle> tiles;
    std::vector<std::vector<int>> grid;
    int grid_width, grid_height;
    float chest_x, chest_y, chest_size;
    std::vector<int> impassable;
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
        } else if (key == "BACKGROUND_NAME") {
            iss >> level.background_name;
            level.background_name = "level/" + level.background_name;
        } else if (key == "TILE_COUNT") {
            int tileCount;
            iss >> tileCount;
            level.tiles.resize(tileCount);
            for (int i = 1; i <= tileCount; ++i) {
                std::getline(configFile, line);
                std::istringstream tileStream(line);
                int x, y, width, height;
                tileStream >> x >> y >> width >> height;
                level.tiles[i] = { (float)x, (float)y, (float)width, (float)height };
            }
        } else if (key == "CHEST_DETAILS") {
            iss >> level.chest_x >> level.chest_y >> level.chest_size;
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

// Based on a GDev 41 submission from last semester (coded by one of the group members)
void endGame(bool win, int health) {
    Color defaultColor = GRAY;
    Color hoverColor = LIGHTGRAY;
    Color clickedColor = DARKGRAY;

    UiLibrary uiLibrary;

    std::string header;
    std::string subtext;

    if (win) {
        header = "Congratulations!";
        subtext = "You won with " + std::to_string(health) + " health!";
    } else {
        header = "Sorry,";
        subtext = "You died.";
    }

    bool end = false;
    float rotation = 0.0f;

    while (!WindowShouldClose() && !end) {
        rotation += 0.2f;

        BeginDrawing();
        ClearBackground(WHITE);

        // Polygon shapes and lines
        // from: https://www.raylib.com/examples/shapes/loader.html?name=shapes_basic_shapes
        DrawPoly((Vector2){ WINDOW_WIDTH/2.0f, WINDOW_HEIGHT / 2.6 }, 6, 230, rotation, BROWN);
        DrawPolyLines((Vector2){ WINDOW_WIDTH/2.0f, WINDOW_HEIGHT / 2.6 }, 6, 240, rotation, BROWN);
        DrawPolyLinesEx((Vector2){ WINDOW_WIDTH/2.0f, WINDOW_HEIGHT / 2.6 }, 6, 235, rotation, 6, BEIGE);

        // Draw text
        DrawText(header.c_str(), WINDOW_WIDTH / 2 - MeasureText(header.c_str(), 30) / 2, WINDOW_HEIGHT / 3, 30, WHITE);
        DrawText(TextFormat(subtext.c_str()),
                 WINDOW_WIDTH / 2 - MeasureText(subtext.c_str(), 20) / 2,
                 WINDOW_HEIGHT / 2.5, 20, WHITE);

        // Draw "Done" button
        if (uiLibrary.Button(0, "Done", {WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT - 120, 200, 60}, defaultColor, hoverColor, clickedColor)) {
            end = true;
        }

        EndDrawing();
    }
}

int main() {
    
    /*
    
        INITIALIZE GAME

    */
    
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Final Project - Layug, Marcelo, Laurel");

    SetTargetFPS(60);
    std::string map_config = "level1.txt";

    // Variables from config
    Level level = loadMap(map_config);

    // Init Background & Tiles
    Texture2D background_texture = LoadTexture(level.background_name.c_str());
    Texture2D tile_texture = LoadTexture(level.image_name.c_str());
    Texture2D chest_texture = LoadTexture("textures/chest.gif");
    float tileSize = 60.0f;

    // Init Player
    Vector2 playerPosition = { (float)WINDOW_WIDTH / 2, (float)WINDOW_HEIGHT * 7/8 };
    float playerSize = 20.0f;
    float playerSpeed = 200.0f;
    int playerHealth = 5;
    Player player(playerPosition, {playerSize,playerSize}, playerSpeed, playerHealth, playerHealth);
    float damageTime = 0.0f;

    // Init Power Bar
    Color powerBarColor = GREEN;
    Rectangle powerBarRect = { 10, WINDOW_HEIGHT - 20, 0, 10 };

    // Init Platforms
    Rectangle platforms[MAX_PLATFORMS] = {
        { 0, WINDOW_HEIGHT - 30, WINDOW_WIDTH, 30 },
        { 200, WINDOW_HEIGHT - 200, 150, 20 },
        { 400, WINDOW_HEIGHT - 300, 150, 20 },
        { 600, WINDOW_HEIGHT - 150, 150, 20 },
        { 100, WINDOW_HEIGHT - 400, 150, 20 }
    };

    // Init Enemies
    Enemy enemies[MAX_ENEMIES] = {
        { (Vector2){ 100, WINDOW_HEIGHT - 120 }, (Vector2){ ENEMY_PATROL_SPEED, 0 }, false, 0 },
        { (Vector2){ 200, WINDOW_HEIGHT - 120 }, (Vector2){ ENEMY_PATROL_SPEED, 0 }, false, 1 },
        { (Vector2){ 300, WINDOW_HEIGHT - 120 }, (Vector2){ ENEMY_PATROL_SPEED, 0 }, false, 2 },
        { (Vector2){ 400, WINDOW_HEIGHT - 120 }, (Vector2){ ENEMY_PATROL_SPEED, 0 }, false, 2 },
        { (Vector2){ 500, WINDOW_HEIGHT - 120 }, (Vector2){ ENEMY_PATROL_SPEED, 0 }, false, 2 }
    };

    // Init clouds
    Cloud clouds[MAX_CLOUDS] = {
        { (Vector2){ 350, 150 }, { { 300, 50 }, { 400, 0 }, { 500, 70 }, { 600, 30 } }, 0.0f, 0.1f, 20, BLUE, LoadTexture("textures/Cloud2.png"), 1 },
        { (Vector2){ 100, 230 }, { { 120, 300 }, { 170, 250 }, { 200, 320 }, { 250, 340 } }, 0.0f, 0.1f, 15, BLUE, LoadTexture("textures/Cloud3.png"), 1 },
        { (Vector2){ 550, 410 }, { { 450, 190 }, { 550, 120 }, { 400, 210 }, { 650, 180 } }, 0.0f, 0.1f, 15, BLUE, LoadTexture("textures/Cloud1.png"), 1 },
        { (Vector2){ 430, 615 }, { { 480, 370 }, { 410, 310 }, { 550, 435 }, { 590, 350 } }, 0.0f, 0.1f, 15, BLUE, LoadTexture("textures/Cloud4.png"), 1 }
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
        
        // Check Player collision with enemies
        for (int i = 0; i < MAX_ENEMIES; i++) {
            if (CheckCollisionRecs((Rectangle){ player.position.x, player.position.y, player.size.x, player.size.y },
                                (Rectangle){ enemies[i].position.x, enemies[i].position.y, ENEMY_SIZE.x, ENEMY_SIZE.y })) {
                // Player-enemy collision detected
                std::cout << deltaTime << std::endl;
                damageTime += deltaTime;
                std::cout << damageTime << std::endl;
                if (damageTime >= 0.25) {
                    player.TakeDamage(1);
                    damageTime = 0;
                }
            }
        }

        // Draw Player Health Bar
        float healthBarWidth = 200.0f; // Width of the health bar
        float healthBarHeight = 20.0f; // Height of the health bar
        Vector2 healthBarPos = { 10, 10 }; // Position of the health bar
        float healthPercentage = (float)player.health / (float)player.maxHealth;
        Rectangle healthBarRect = { healthBarPos.x, healthBarPos.y, healthBarWidth * healthPercentage, healthBarHeight };
        Color healthBarColor = (healthPercentage > 0.5f) ? GREEN : (healthPercentage > 0.2f) ? ORANGE : RED;

        // Check Tile Collisions
        for (int i = 0; i < level.grid_height; ++i) {
            for (int j = 0; j < level.grid_width; ++j) {
                if (level.grid[i][j] > 0) {  // Assume non-zero means there is a tile
                    Rectangle playerRect = {player.position.x - player.size.x / 2, player.position.y - player.size.y / 2, player.size.x, player.size.y};
                    Rectangle tileRect = {j * tileSize, i * tileSize, tileSize, tileSize};

                    if (CheckCollisionRecs(playerRect, tileRect)) {
                        std::cout << "Collision at Tile Index [" << i << "][" << j << "]: " << level.grid[i][j] << std::endl;

                        // Check if the bottom of the player is colliding with the top of the tile
                        if (playerRect.y + playerRect.height > tileRect.y && playerRect.y < tileRect.y) {
                            player.position.y = tileRect.y - playerRect.height;  // Adjust player position to the top of the tile
                            player.velocity.y = 0;
                            player.SetState(&player.idle);
                        }
                        // Check if the top of the player is colliding with the bottom of the tile
                        else if (playerRect.y < tileRect.y + tileRect.height && playerRect.y + playerRect.height > tileRect.y + tileRect.height) {
                            player.position.y = tileRect.y + tileRect.height + playerRect.height;  // Adjust player position to the bottom of the tile
                            player.velocity.y = 0;
                        }

                        // Invert x velocity when there is a horizontal collision
                        if ((playerRect.x < tileRect.x + tileRect.width && playerRect.x + playerRect.width > tileRect.x) ||
                            (playerRect.x + playerRect.width > tileRect.x && playerRect.x < tileRect.x + tileRect.width)) {
                            player.velocity.x = -player.velocity.x;  // Invert x velocity
                        }
                    }
                }
            }
        }

        // Update enemies
        for (int i = 0; i < MAX_ENEMIES; i++) {
            UpdateEnemy(enemies[i], player, platforms, MAX_PLATFORMS, WINDOW_WIDTH, WINDOW_HEIGHT);
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

        // Draw Background
        DrawTexture(background_texture, 0, 0, WHITE);

        // Draw Platforms
        for (int i = 0; i < MAX_PLATFORMS; i++)
            DrawRectangleRec(platforms[i], GRAY);

        // Draw Tiles
        for (int i = 0; i < level.grid_height; ++i) {
            for (int j = 0; j < level.grid_width; ++j) {
                int tileIndex = level.grid[i][j];
                if (tileIndex >= 1 && tileIndex <= level.tiles.size()) {
                    Rectangle dest = { j * tileSize, i * tileSize, tileSize, tileSize };
                    DrawTexturePro(tile_texture, level.tiles[tileIndex], dest, Vector2{ float(-WINDOW_WIDTH/2 + level.grid_width * tileSize / 2), float(-WINDOW_HEIGHT/2 + level.grid_height * tileSize / 2) }, 0.0f, WHITE);
                }
            }
        }

        // Draw chest
        Rectangle chest_source = {0.0f, 0.0f, (float) chest_texture.width, (float) chest_texture.height};
        Rectangle chest_dest = {level.chest_x, level.chest_y, level.chest_size * 1.4f, level.chest_size};
        DrawTexturePro(chest_texture, chest_source, chest_dest, {0.0f, 0.0f}, 0.0f, WHITE);

        // Draw Player
        DrawRectangleV(player.position, player.size, player.color);

        // Draw Enemies
        for (int i = 0; i < MAX_ENEMIES; i++)
            DrawRectangleV(enemies[i].position, ENEMY_SIZE, RED);

        // Draw clouds
        for (int i = 0; i < MAX_CLOUDS; i++) {
            DrawCloud(clouds[i]);
        }

        // Draw Health bar
        DrawRectangleRec(healthBarRect, healthBarColor);
        DrawRectangleLinesEx(healthBarRect, 2, BLACK);

        EndDrawing();

        // Close the window if player's health reaches zero
        if (player.health <= 0) {
            endGame(false, 0);
            break;
        }

        // Detect win if player hit the chest
        Rectangle rec_player = {player.position.x, player.position.y, player.size.x, player.size.y};
        if (CheckCollisionRecs(chest_dest, rec_player)) {
            endGame(true, player.health);
            break;
        }
    }

    // Unload textures
    for (int i = 0; i < MAX_CLOUDS; i++) {
        UnloadTexture(clouds[i].texture);
    }
    
    UnloadTexture(background_texture);
    UnloadTexture(tile_texture);
    UnloadTexture(chest_texture);

    CloseWindow();

    return 0;
}