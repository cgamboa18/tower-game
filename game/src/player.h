#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

#include "game_object.h"

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
    GameObject object;

    // Combat
    float health;
    float energy;
} Player;

// Initialize player struct
void InitPlayer(Player *p, Vector3 spawnPoint);
// Update player main 
void UpdatePlayer(Player *p, GameObject **gameObjects, int gameObjectCount);
// Update player actions based on state
void UpdatePlayerAction(Player *p);
// Main player update function
void UpdatePlayerState(Player *p, Vector3 moveDirection);
// Update player collisions against all gameObjects
void UpdatePlayerCollision(Player *p, GameObject **gameObjects, int gameObjectCount);
// Update player camera
void UpdatePlayerCamera(Player *p);
// Returns input movement direction as Vector3
Vector3 GetMoveDirection(Camera c);
// Callback function for registering collisions between player game objects
void PlayerCollisionCallback(GameObject *object1, GameObject *object2, int collisionBodyIdx1, int collisionBodyIdx2, void *ctx);

#endif