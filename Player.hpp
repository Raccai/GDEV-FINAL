#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "raymath.h"

#define PLAYER_SIZE (Vector2){ 24, 24 }
#define PLAYER_SPEED 200.0f
#define PLAYER_GRAVITY 700.0f
#define PLAYER_JUMP_STRENGTH 500.0f
#define MAX_JUMP_HOLD_TIME 1.5f

class Enemy;
class Player;

/*
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
*/

class PlayerState {
public:
    virtual ~PlayerState() {}
    virtual void Enter(Player& player) = 0;
    virtual void Update(Player& player, float delta_time) = 0;
};

class PlayerCharging : public PlayerState {
public:
    void Enter(Player& player);
    void Update(Player& player, float delta_time);
};

class PlayerIdle : public PlayerState {
public:
    void Enter(Player& player);
    void Update(Player& player, float delta_time);
};

class PlayerJumping : public PlayerState {
public:
    void Enter(Player& player);
    void Update(Player& player, float delta_time);
};

class PlayerMoving : public PlayerState {
public:
    void Enter(Player& player);
    void Update(Player& player, float delta_time);
};

class Player {
public:
    Vector2 position;
    Vector2 size;
    Color color;
    int health;

    Vector2 velocity;
    float speed;
    float gravity;

    float jumpStrength;
    float jumpChargeTime;

    PlayerCharging charging;
    PlayerIdle idle;
    PlayerJumping jumping;
    PlayerMoving moving;

    Player(Vector2 pos, float rad, float spd, int hp);

    PlayerState* GetCurrentState() const { return current_state; }

    Player(Vector2 pos, Vector2 siz, float spd, int hp);

    void Update(float delta_time);

    void Draw();

    void SetState(PlayerState* new_state);

    void ApplyGravity(float delta_time);
    
    bool IsCollidingEnemy(Enemy& enemy);

    // TODO
    // bool IsCollidingTerrain(Tile& tile);

    void TakeDamage(int damage) { health -= damage; }

private:
    PlayerState* current_state;
};


#endif