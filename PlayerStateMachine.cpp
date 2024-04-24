#include "Player.hpp"

// Function prototypes
void ProcessJump(Player& player, Rectangle& powerBarRect, Color& powerBarColor, const int screenWidth);
void HandleCollisions(Player& player, Rectangle platforms[], const int MAX_PLATFORMS, const int screenWidth, const int screenHeight);
void HandleMovement(Player& player, const int screenWidth);
void HandleIdle(Player& player);
void ApplyGravity(Player& player);

void UpdatePlayer(Player& player, Rectangle platforms[], const int MAX_PLATFORMS, Rectangle powerBarRect, Color& powerBarColor, const int screenWidth, const int screenHeight) {
    // Process jump
    ProcessJump(player, powerBarRect, powerBarColor, screenWidth);

    // Handle collisions
    HandleCollisions(player, platforms, MAX_PLATFORMS, screenWidth, screenHeight);

    // Handle player movement
    HandleMovement(player, screenWidth);

    // Handle player idle state
    HandleIdle(player);
    ApplyGravity(player);
}

void ProcessJump(Player& player, Rectangle& powerBarRect, Color& powerBarColor, const int screenWidth) {
    // Jump logic
    if (IsKeyDown(KEY_SPACE)) {
        player.isChargingJump = true;
        player.jumpHoldTime += GetFrameTime();

        // Prevent horizontal movement while charging jump
        if (player.isChargingJump) {
            player.velocity.x = 0;
        }

        // Adjust power bar width based on jump hold time
        powerBarRect.width = screenWidth / (MAX_JUMP_HOLD_TIME / player.jumpHoldTime);
        if (powerBarRect.width > screenWidth) powerBarRect.width = screenWidth;

        // Change power bar color based on charge level
        if (player.jumpHoldTime < 0.5f) powerBarColor = GREEN;
        else if (player.jumpHoldTime < 1.0f) powerBarColor = YELLOW;
        else powerBarColor = RED;
    } else if (player.isOnGround && player.isChargingJump) {
        // Handle jump execution
        const float jumpStrength = Clamp(player.jumpHoldTime * 2.6f, 1.1f, 2.0f) / 2.0f;

        Vector2 dir = { 0.0f, -1.0f };
        const float xMoveStrength = 0.75f - (jumpStrength * 0.5f);

        if (!player.isChargingJump) {
            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) dir.x += xMoveStrength;
            if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) dir.x -= xMoveStrength;
        }

        dir = Vector2Normalize(dir);
        dir = Vector2Scale(dir, jumpStrength * PLAYER_JUMP_STRENGTH);
        player.velocity = dir;

        player.isOnGround = false;
        player.isChargingJump = false;
        player.jumpHoldTime = 0.0f;

        // Reset power bar
        powerBarRect.width = 0;
        powerBarColor = GREEN;
    }
}

void HandleCollisions(Player& player, Rectangle platforms[], const int MAX_PLATFORMS, const int screenWidth, const int screenHeight) {
    // Collision detection with platforms
    Rectangle playerRect = { player.position.x, player.position.y, PLAYER_SIZE.x, PLAYER_SIZE.y };
    
    for (int i = 0; i < MAX_PLATFORMS; i++) {
        if (CheckCollisionRecs(playerRect, platforms[i])) {
            // Vertical collision detection
            if (player.velocity.y > 0 && player.position.y < platforms[i].y) {
                // Collision from the bottom, adjust player's position and velocity
                player.position.y = platforms[i].y - PLAYER_SIZE.y;
                player.velocity.y = 0;
                player.isOnGround = true;
            } else if (player.velocity.y < 0 && player.position.y > platforms[i].y) {
                // Collision from the top, adjust player's position and velocity
                player.position.y = platforms[i].y + platforms[i].height;
                player.velocity.y = 0;
            }

            // Horizontal collision detection
            if (player.velocity.x > 0 && player.position.x < platforms[i].x) {
                // Collision from the left, adjust player's position
                player.position.x = platforms[i].x - PLAYER_SIZE.x;
            } else if (player.velocity.x < 0 && player.position.x > platforms[i].x) {
                // Collision from the right, adjust player's position
                player.position.x = platforms[i].x + platforms[i].width;
            }

            // Break loop, no need to check other platforms
            break;
        } else {
            player.isOnGround = false;
        }
    }

    // Check collisions with screen bounds
    if (player.position.x < 0) player.position.x = 0;
    if (player.position.x > screenWidth - PLAYER_SIZE.x) player.position.x = screenWidth - PLAYER_SIZE.x;
    if (player.position.y < 0) player.position.y = 0;
    if (player.position.y > screenHeight - PLAYER_SIZE.y) player.position.y = screenHeight - PLAYER_SIZE.y;
}

void HandleMovement(Player& player, const int screenWidth) {
    // Player movement logic
    if (!player.isChargingJump) {
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
            player.position.x += PLAYER_SPEED * GetFrameTime();
            player.isFacingRight = true;
        }
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
            player.position.x -= PLAYER_SPEED * GetFrameTime();
            player.isFacingRight = false;
        }
    }
}

void HandleIdle(Player& player) {
    // Idle logic for the player
}

void ApplyGravity(Player& player) {
    // Apply gravity only if the player is not on the ground
    if (!player.isOnGround) {
        player.velocity.y += PLAYER_GRAVITY * GetFrameTime();
    }

    // Update player position based on velocity
    player.position.y += player.velocity.y * GetFrameTime();
}