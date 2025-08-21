#ifndef ENEMY_H 
#define ENEMY_H 

#include "raylib.h"

#include "game_object.h"
#include "player.h"

typedef enum {
    ENEMY_IDLE,
    ENEMY_PURSUIT,
    ENEMY_ATTACK
} EnemyState;

typedef struct {
    EnemyState state;
    GameObject object;

    // Combat
    float health;
} Enemy;

// Initiallize enemy 
void InitEnemy(Enemy *e, Vector3 spawnPoint);
// Update enemy main
void UpdateEnemy(Enemy *e, Player *p, GameObject **gameObjects, int gameObjectCount);
// Update enemy actions based on state
void UpdateEnemyAction(Enemy *e, Player *p);
// Main enemy update function
void UpdateEnemyState(Enemy *e, Player *p);

// Callback function for registering collisions between enemy game objects (TODO: Wrap collision into ctx)
void EnemyCollisionCallback(CollisionInfo collision, GameObject *object1, GameObject *object2, void *ctx);

#endif