#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "raymath.h"

#define PLAYER_SIZE (Vector2){ 24, 24 }

class Enemy;
class Tile;
class Player;

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

    int maxHealth;
    int currentHealth;

    PlayerCharging charging;
    PlayerIdle idle;
    PlayerJumping jumping;
    PlayerMoving moving;

    Player(Vector2 pos, Vector2 siz, float spd, int hp, int maxHp);

    PlayerState* GetCurrentState() const { return current_state; }

    void Update(float delta_time);

    void Draw();

    void SetState(PlayerState* new_state);

    void ApplyGravity(float delta_time);

    void TakeDamage(int damage) { health -= damage; }

private:
    PlayerState* current_state;
};


#endif
