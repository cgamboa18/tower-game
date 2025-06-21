#include "raylib.h"

#include "player.h"
#include "game_object.h"

void InitPlayer(Player *p, Vector3 sp) {
    p->camera = (Camera3D) {
        .position = (Vector3){sp.x + 0.0f, sp.y + 5.0f, sp.z + 10.0f},
        .target = sp,
        .up = (Vector3){0.0f, 1.0f, 0.0f},
        .fovy = 60.0f,
        .projection = CAMERA_PERSPECTIVE
    }; 
    p->state = IDLE;
    InitGameObject(&p->object);

    p->health = 100;
    p->energy = 100;
}

void UpdatePlayerAction(Player *p) {
    if (IsKeyDown(KEY_W)) {
        p->object.transform.translation.x += 0.01;
    }

    if (IsKeyDown(KEY_S)) {
        p->object.transform.translation.y += 0.01;
    }

    if (IsKeyDown(KEY_A)) {
        p->object.transform.translation.z += 0.01;
    }
}

void UpdatePlayerCollision(Player *p, GameObject **gameObjects, int gameObjectCount) {
    GameObject *playerGameObject = &p->object;

    for (int i = 0; i < gameObjectCount; i++) {
        if (playerGameObject == gameObjects[i])
            continue;

        for (int j = 0; j < playerGameObject->collisionBodyCount; j++) { 
            CollisionBody playerBody = playerGameObject->collisionBodies[j];

            for (int k = 0; k < gameObjects[i]->collisionBodyCount; k++) {
                CollisionBody otherBody = gameObjects[i]->collisionBodies[k];

                if (playerBody.mode == COLLIDE_DETECT) {
                    // CheckCollisionBodies(playerBody, otherBody);
                }
            }
        }
    }
}

void UpdatePlayerCamera(Player *p) {
    UpdateCamera(&p->camera, CAMERA_ORBITAL);
}