#include "Player.hpp"
#include "Enemy.hpp"

#include <raylib.h>
#include <raymath.h>

// to remove after debugging
#include <iostream>

// Init Player
Player::Player(Vector2 pos, Vector2 siz, float spd, int hp, int maxHp) {
    this->position = pos;
    this->size = siz;
    this->speed = spd;
    this->health = hp;
    this->maxHealth = maxHp;
    this->velocity = Vector2Zero();
    this->jumpChargeTime = 0;
    this->gravity = 2.8;

    SetState(&idle);
}

void Player::Update(float delta_time) {
    current_state->Update(*this, delta_time);
}

void Player::Draw() {
    DrawRectangleV(position, size, color);

    // TODO: Check if we draw the player charging meter
}

void Player::SetState(PlayerState* new_state) {
    current_state = new_state;
    current_state->Enter(*this);
}

void Player::ApplyGravity(float delta_time) {
    // TODO: change and compute tile collisions
    if (position.y <= 394) {
        velocity.y += gravity * speed * delta_time; // should clamp this value
        position.y += velocity.y * delta_time; // should clamp this value
    } else {
        velocity = {0, 0};
        position.y = 394;
    }
}

bool Player::IsCollidingEnemy(Enemy& enemy) {

    return false;// No collision occurred
}

bool Player::IsCollidingTerrain(Tile& tile) {
    // Implement collision detection with terrain
    return false;
}

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
    std::cout << "PlayerCharging: " << player.jumpStrength << std::endl;

    player.jumpChargeTime += delta_time;
    
    player.jumpStrength = Clamp(player.jumpChargeTime + 0.5f, 0.5f, 1.5f);

    if (IsKeyReleased(KEY_SPACE)) {
        // Determine horizontal direction of the jump
        if (IsKeyDown(KEY_A)) player.velocity = {-1, -1};
        else if (IsKeyDown(KEY_D)) player.velocity = {1, -1};
        else player.velocity = {0, -1};

        Vector2Normalize(player.velocity);
        player.velocity = Vector2Scale(player.velocity, player.speed * player.jumpStrength);
        player.SetState(&player.jumping);
    }
}

void PlayerIdle::Update(Player& player, float delta_time) {
    std::cout << "PlayerIdle" << std::endl;
    if ((IsKeyDown(KEY_A) || IsKeyDown(KEY_D)) && !((IsKeyDown(KEY_A) && IsKeyDown(KEY_D)))) {
        player.SetState(&player.moving);
    }
    else if (IsKeyDown(KEY_SPACE)) {    
        player.SetState(&player.charging);
    }
}

void PlayerJumping::Update(Player& player, float delta_time) {
    std::cout << "PlayerJumping: (" << player.velocity.x << ", " << player.velocity.y << ")" << std::endl;
    
    // Update player's position based on velocity
    player.position = Vector2Add(player.position, Vector2Scale(player.velocity, delta_time));
    
    // Apply gravity
    player.ApplyGravity(delta_time);

    // Reset jump charge time for next jump
    player.jumpChargeTime = 0;
    if (Vector2Length(player.velocity) == 0)
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
