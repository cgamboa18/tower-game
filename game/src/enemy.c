#include "raylib.h"

#include "enemy.h"
#include "game_object.h"

void InitEnemy(Enemy *e, Vector3 spawnPoint) {
    e->state = ENEMY_IDLE;
    InitGameObject(&e->object, spawnPoint);
    e->object.id = BEAST_ID;

    e->health = 100;
}