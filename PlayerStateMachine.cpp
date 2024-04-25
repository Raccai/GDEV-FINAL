#include "Player.hpp"
#include "Enemy.hpp"

#include <raylib.h>
#include <raymath.h>

// to remove after debugging
#include <iostream>

// Init Player
Player::Player(Vector2 pos, Vector2 siz, float spd, int hp) {
    this->position = pos;
    this->size = siz;
    this->speed = spd;
    this->health = hp;
    this->velocity = Vector2Zero();

    SetState(&idle);
}

void Player::Update(float delta_time) {
    current_state->Update(*this, delta_time);
}

void Player::Draw() {
    // Draw the player's main body
    DrawRectangleV(position, size, color);
}

void Player::SetState(PlayerState* new_state) {
    current_state = new_state;
    current_state->Enter(*this);
}

// TODO: UPDATE FOR ALL ENEMIES
bool Player::IsCollidingEnemy(Enemy& enemy) {
    /* 
    float distance = Vector2Distance(enemy.position, position);
    if ((distance < enemy.size + 10)) { // 10 would be the range of the attack
        if(dynamic_cast<PlayerAttacking*>(GetCurrentState())) {
            enemy.TakeDamage(1);
            return true;
        }
        
    }
    */
    return false;// No collision occurred
}

// TODO: CREATE COLLIDING FOR ALL TILES IN GAME

// TODO: CREATE COLLIDING FOR FLOOR/GROUND

/*

    Enter Functions

*/

void PlayerCharging::Enter(Player& player) {
    player.color = ORANGE;
}

void PlayerIdle::Enter(Player& player) {
    player.color = BLUE;
}

void PlayerJumping::Enter(Player& player) {
    player.color = RED;
}

void PlayerMoving::Enter(Player& player) {
    player.color = YELLOW;
}

/*

    Update Functions

*/

void PlayerCharging::Update(Player& player, float delta_time) {
    std::cout << "PlayerCharging" << std::endl;
    if (IsKeyReleased(KEY_SPACE)) {
        player.SetState(&player.jumping);
    }
}

void PlayerIdle::Update(Player& player, float delta_time) {
    std::cout << "PlayerIdle" << std::endl;
    if ((IsKeyDown(KEY_A) || IsKeyDown(KEY_D))) {
        player.SetState(&player.moving);
    }
    else if (IsKeyDown(KEY_SPACE)) {    
        player.SetState(&player.charging);
    }
}

void PlayerJumping::Update(Player& player, float delta_time) {
    std::cout << "PlayerJumping" << std::endl;
    player.SetState(&player.idle);
}

void PlayerMoving::Update(Player& player, float delta_time) {
    std::cout << "PlayerMoving" << std::endl;
    player.velocity = Vector2Zero();

    if (IsKeyDown(KEY_A)) player.velocity.x -= 1;
    if (IsKeyDown(KEY_D)) player.velocity.x += 1;

    if (player.velocity.x != 0) {
        player.velocity = Vector2Normalize(player.velocity);
        player.velocity = Vector2Scale(player.velocity, player.speed);
    }

    // TODO: COMPUTE FOR GRAVITY

    player.position = Vector2Add(player.position, Vector2Scale(player.velocity, delta_time));

    if (Vector2Length(player.velocity) == 0) {
        player.SetState(&player.idle);
    } else if (IsKeyDown(KEY_SPACE)) {    
        player.SetState(&player.charging);
    }
}

/*
void ProcessJump(Player& player, Rectangle& powerBarRect, Color& powerBarColor, const int screenWidth);
void HandleCollisions(Player& player, Rectangle platforms[], const int MAX_PLATFORMS, const int screenWidth, const int screenHeight);
void HandleMovement(Player& player, const int screenWidth);
void HandleIdle(Player& player);
void ApplyGravity(Player& player);

void UpdatePlayer(Player& player, Rectangle platforms[], const int MAX_PLATFORMS, Rectangle powerBarRect, Color& powerBarColor, const int screenWidth, const int screenHeight) {
    ProcessJump(player, powerBarRect, powerBarColor, screenWidth);
    HandleCollisions(player, platforms, MAX_PLATFORMS, screenWidth, screenHeight);
    HandleMovement(player, screenWidth);
    HandleIdle(player);
    ApplyGravity(player);
}

void ProcessJump(Player& player, Rectangle& powerBarRect, Color& powerBarColor, const int screenWidth) {
    if (IsKeyDown(KEY_SPACE)) {
        
        // Check how long the player is charging jump
        player.isChargingJump = true;
        player.jumpHoldTime += GetFrameTime();

        if (player.isChargingJump)
            player.velocity.x = 0;

        // Update jump power bar
        powerBarRect.width = screenWidth / (MAX_JUMP_HOLD_TIME / player.jumpHoldTime);
        if (powerBarRect.width > screenWidth)
            powerBarRect.width = screenWidth;

        if (player.jumpHoldTime < 0.5f)
            powerBarColor = GREEN;
        else if (player.jumpHoldTime < 1.0f)
            powerBarColor = YELLOW;
        else
            powerBarColor = RED;

    } else if (player.isOnGround && player.isChargingJump) {
        // Physics calculations for jumps
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

        powerBarRect.width = 0;
        powerBarColor = GREEN;

        
    }
}

void HandleCollisions(Player& player, Rectangle platforms[], const int MAX_PLATFORMS, const int screenWidth, const int screenHeight) {
    Rectangle playerRect = { player.position.x, player.position.y, PLAYER_SIZE.x, PLAYER_SIZE.y };
    
    for (int i = 0; i < MAX_PLATFORMS; i++) {
        if (CheckCollisionRecs(playerRect, platforms[i])) {
            if (player.velocity.y > 0 && player.position.y < platforms[i].y) {
                player.position.y = platforms[i].y - PLAYER_SIZE.y;
                player.velocity.y = 0;
                player.isOnGround = true;
            } else if (player.velocity.y < 0 && player.position.y > platforms[i].y) {
                player.position.y = platforms[i].y + platforms[i].height;
                player.velocity.y = 0;
            }

            if (player.velocity.x > 0 && player.position.x < platforms[i].x) {
                player.position.x = platforms[i].x - PLAYER_SIZE.x;
            } else if (player.velocity.x < 0 && player.position.x > platforms[i].x) {
                player.position.x = platforms[i].x + platforms[i].width;
            }

            break;
        } else {
            player.isOnGround = false;
        }
    }

    if (player.position.x < 0) player.position.x = 0;
    if (player.position.x > screenWidth - PLAYER_SIZE.x) player.position.x = screenWidth - PLAYER_SIZE.x;
    if (player.position.y < 0) player.position.y = 0;
    if (player.position.y > screenHeight - PLAYER_SIZE.y) player.position.y = screenHeight - PLAYER_SIZE.y;
}

void HandleMovement(Player& player, const int screenWidth) {
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
    if (!player.isOnGround) {
        player.velocity.y += PLAYER_GRAVITY * GetFrameTime();
    }

    player.position.y += player.velocity.y * GetFrameTime();
}
*/