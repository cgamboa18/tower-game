#include "raylib.h"

#include "enemy.h"
#include "game_object.h"

void InitEnemy(Enemy *e, Vector3 sp) {
    e->state = ENEMY_IDLE;
    InitGameObject(&e->object);

    e->health = 100;
}