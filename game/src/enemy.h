#ifndef ENEMY_H 
#define ENEMY_H 

#include "raylib.h"

#include "game_object.h"

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

// Initiallize game object
void InitEnemy(Enemy *e, Vector3 sp);

#endif