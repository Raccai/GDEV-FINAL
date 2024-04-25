#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include "Player.hpp" // Include Player.hpp for Player struct definition

struct Enemy {
    Vector2 position;
    Vector2 velocity;
    bool isChasingPlayer;
    int changeDirectionTimer;
    int platformIndex; // Index of the platform the enemy is currently on
    float changeDirectionInterval; // Interval for changing direction
    float detectionRangeWidth; // Width of the detection range for chasing the player
    float pauseTimer; // Timer for pausing
    float pauseDuration; // Duration of the pause
};

#define ENEMY_SIZE (Vector2){ 16, 16 }
#define ENEMY_PATROL_SPEED 50.0f

void UpdateEnemy(Enemy& enemy, const Player& player, Rectangle platforms[], const int MAX_PLATFORMS, const int screenWidth, const int screenHeight);

#endif // ENEMY_H
