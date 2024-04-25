#include "Enemy.hpp"
#include "Player.hpp"
#include <cstdlib>
#include <ctime>

void UpdateEnemy(Enemy& enemy, const Player& player, Rectangle platforms[], const int MAX_PLATFORMS, const int screenWidth, const int screenHeight) {
    enemy.velocity.y += PLAYER_GRAVITY * GetFrameTime();

    enemy.position.x += enemy.velocity.x * GetFrameTime();
    enemy.position.y += enemy.velocity.y * GetFrameTime();

    enemy.position.x = Clamp(enemy.position.x, 0, screenWidth - ENEMY_SIZE.x);
    enemy.position.y = Clamp(enemy.position.y, 0, screenHeight - ENEMY_SIZE.y);

    // Ensure enemy stays on its platform
    Rectangle platform = platforms[enemy.platformIndex];

    // Check if the enemy is within the bounds of its platform
    if (enemy.position.x < platform.x) {
        enemy.position.x = platform.x;
        enemy.velocity.x *= -1; // Reverse direction when reaching platform edge
        enemy.changeDirectionTimer = (int)(enemy.changeDirectionInterval * 60); // Reset the timer
    } else if (enemy.position.x + ENEMY_SIZE.x > platform.x + platform.width) {
        enemy.position.x = platform.x + platform.width - ENEMY_SIZE.x;
        enemy.velocity.x *= -1; // Reverse direction when reaching platform edge
        enemy.changeDirectionTimer = (int)(enemy.changeDirectionInterval * 60); // Reset the timer
    }

    if (enemy.position.y > platform.y) {
        enemy.position.y = platform.y;
        enemy.velocity.y = 0; // Stop vertical movement if below the platform
    }

    if (CheckCollisionRecs((Rectangle){ enemy.position.x, enemy.position.y, ENEMY_SIZE.x, ENEMY_SIZE.y }, platform)) {
        enemy.position.y = platform.y - ENEMY_SIZE.y;
        enemy.velocity.y = 0;
    } else {
        // If the enemy is not on the platform, reset its position to be on the platform
        if (enemy.velocity.y > 0) {
            for (int i = 0; i < MAX_PLATFORMS; i++) {
                if (CheckCollisionRecs((Rectangle){ enemy.position.x, enemy.position.y + 1, ENEMY_SIZE.x, ENEMY_SIZE.y }, platforms[i])) {
                    enemy.position.y = platforms[i].y - ENEMY_SIZE.y;
                    enemy.platformIndex = i;
                    break;
                }
            }
        }
    }

    // If the timer reaches 0, change direction and reset the timer with a new random interval
    if (enemy.changeDirectionTimer <= 0) {
        enemy.velocity.x = 0; // Pause
        enemy.changeDirectionInterval = (float)(rand() % 5 + 1); // Random interval between 1 and 5 seconds
        enemy.changeDirectionTimer = (int)(enemy.changeDirectionInterval * 60); // Reset the timer
    } else {
        enemy.changeDirectionTimer -= 1; // Decrease timer
    }

    // Allow the enemy to wander within the length of its platform
    if (enemy.velocity.x == 0) {
        enemy.velocity.x = ENEMY_PATROL_SPEED * ((rand() % 3) - 1); // Randomly choose a direction to move
    }

    if (CheckCollisionCircleRec(enemy.position, enemy.detectionRangeWidth / 2, (Rectangle){ player.position.x, player.position.y, PLAYER_SIZE.x, PLAYER_SIZE.y })) {
        Vector2 playerCenter = { player.position.x + PLAYER_SIZE.x / 2, player.position.y + PLAYER_SIZE.y / 2 };
        Vector2 enemyCenter = { enemy.position.x + ENEMY_SIZE.x / 2, enemy.position.y + ENEMY_SIZE.y / 2 };
        Vector2 direction = Vector2Normalize(Vector2Subtract(playerCenter, enemyCenter));
        enemy.velocity = Vector2Scale(direction, ENEMY_PATROL_SPEED);

        enemy.position.y += enemy.velocity.y * GetFrameTime();
    }
}
