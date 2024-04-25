#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "raymath.h"

struct Player {
    Vector2 position;
    Vector2 velocity;
    bool isChargingJump;
    bool isOnGround;
    float jumpHoldTime;
    bool isFacingRight;
};

#define PLAYER_SIZE (Vector2){ 24, 24 }
#define PLAYER_SPEED 200.0f
#define PLAYER_GRAVITY 700.0f
#define PLAYER_JUMP_STRENGTH 500.0f
#define MAX_JUMP_HOLD_TIME 1.5f

void UpdatePlayer(Player& player, Rectangle platforms[], const int MAX_PLATFORMS, Rectangle powerBarRect, Color& powerBarColor, const int screenWidth, const int screenHeight);

#endif // PLAYER_H