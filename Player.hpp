#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "raymath.h"

struct Player {
    Vector2 position;
    Vector2 velocity;
    bool isOnGround;
    bool isFacingRight;
    float jumpHoldTime;
    bool isChargingJump;
};

#define PLAYER_SIZE (Vector2){ 16, 32 }
#define PLAYER_GRAVITY 800.0f
#define PLAYER_SPEED 200.0f
#define PLAYER_JUMP_STRENGTH 600.0f // Adjusted jump strength
#define MAX_JUMP_HOLD_TIME 1.5f // Maximum time for jump charging

// Function prototype
void UpdatePlayer(Player& player, Rectangle platforms[], const int MAX_PLATFORMS, Rectangle powerBarRect, Color& powerBarColor, const int screenWidth, const int screenHeight);

#endif // PLAYER_H
