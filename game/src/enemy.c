#include "raylib.h"
#include "raymath.h"

#include "enemy.h"
#include "player.h"
#include "game_object.h"

void InitEnemy(Enemy *e, Vector3 spawnPoint) {
    e->state = ENEMY_IDLE;
    InitGameObject(&e->object, spawnPoint);
    e->object.id = BEAST_ID;

    e->health = 100;
}

void UpdateEnemy(Enemy *e, Player *p, GameObject **gameObjects, int gameObjectCount) {
    // Update motion for game object
    UpdateGameObjectMotion(&e->object, e->object.velocity);
    // Update collisions for game object
    UpdateGameObjectSceneCollisions(&e->object, gameObjects, gameObjectCount, &EnemyCollisionCallback, e);
    // Update the enemy's actions
    UpdateEnemyAction(e, p);

}

void UpdateEnemyAction(Enemy *e, Player *p) {
    Vector3 *enemyVelocity = &e->object.velocity;

    // Determine current player state
    UpdateEnemyState(e, p);

    Vector3 toPlayer = Vector3Subtract(p->object.transform.translation, 
                                       e->object.transform.translation);
    Vector3 toPlayerDir = Vector3Normalize(toPlayer);

    enemyVelocity->y -= 0.5f * GetFrameTime();

    // Control player behavior based on player state
    switch (e->state) {
        case ENEMY_PURSUIT:
            Vector3 pursuitDir = toPlayerDir;

            enemyVelocity->x = pursuitDir.x * 5 * GetFrameTime();
            enemyVelocity->z = pursuitDir.z * 5 * GetFrameTime();
            break;
        case ENEMY_IDLE:
            enemyVelocity->x = 0;
            enemyVelocity->z = 0;
            break;
    }
}

void UpdateEnemyState(Enemy *e, Player *p) {
    Vector3 toPlayer = Vector3Subtract(p->object.transform.translation, 
                                       e->object.transform.translation);

    if (Vector3Length(toPlayer) > 5.0f) {
        e->state = ENEMY_PURSUIT;
    } else {
        e->state = ENEMY_IDLE;
    }
}

void EnemyCollisionCallback(CollisionInfo collision, GameObject *object1, GameObject *object2, void *ctx) {
    Enemy *e = (Enemy *) ctx;

    if (object2->id == SURFACE_ID) {
        CollideAndSlide(&e->object, collision);
    }

    DrawText(TextFormat("enemy collision callback: %d", e->object.id), 10, 80, 10, DARKGRAY);
}