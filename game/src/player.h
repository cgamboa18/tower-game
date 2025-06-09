#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

typedef enum {
    IDLE,
    RUNNING,
    ATTACKING,
    DASHING,
    FLYING,
    FALLING
} PlayerState;

typedef struct {
    Camera3D camera;
    PlayerState state;

    // Transform
    Vector3 position;
    float rotation;

    // Combat
    float health;
    float energy;
} Player;

void InitPlayer(Player *p, Vector3 sp); // Initialize player struct
void UpdatePlayer(Player *p);           // Initialize player struct

#endif