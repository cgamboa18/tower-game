#include "raylib.h"
#include "raymath.h"
#include "rcamera.h"

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
    Vector3 *playerVelocity = &p->object.velocity;
    Vector3 moveDirection = GetMoveDirection(p->camera);

    // Update physics motion for game object
    UpdateGameObjectMotion(&p->object);

    // Determine current player state
    UpdatePlayerState(p, moveDirection);

    // Update camera position
    UpdatePlayerCamera(p);

    // Control player behavior based on player state
    switch (p->state) {
        case RUNNING:
            playerVelocity->x = moveDirection.x * 10 * GetFrameTime();
            playerVelocity->z = moveDirection.z * 10 * GetFrameTime();
            break;
        case IDLE:
            playerVelocity->x = 0;
            playerVelocity->z = 0; 
            break;
    }
}

void UpdatePlayerCollision(Player *p, GameObject **gameObjects, int gameObjectCount) {
    GameObject *playerGameObject = &p->object;

    for (int i = 0; i < gameObjectCount; i++) {
        if (playerGameObject == gameObjects[i])
            continue;

        // Check collision between player bodies and other game object bodies
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

void UpdatePlayerState(Player *p, Vector3 moveDirection) {
    if (!Vector3Equals(moveDirection, Vector3Zero())) {
        p->state = RUNNING;
    } else {
        p->state = IDLE;
    }
}

void UpdatePlayerCamera(Player *p) {
    Camera3D *cam = &p->camera;

    // Set camera target to player's position
    Vector3 lastTarget = cam->target;
    cam->target = p->object.transform.translation;

    float desiredDistance = 10.0f;
    float currentDistance = Vector3Distance(cam->position, cam->target);

    if (fabs(desiredDistance - currentDistance) >= 0.01) {
        // Move the camera position by the target delta to preserve relative orbit position
        Vector3 targetDelta = Vector3Subtract(cam->target, lastTarget);
        cam->position = Vector3Add(cam->position, targetDelta);

        // Snap camera to orbital
        currentDistance = Vector3Distance(cam->position, cam->target);
        float adjust = desiredDistance - currentDistance;
        CameraMoveToTarget(cam, adjust);
    }

    // Handle mouse input for orbiting
    Vector2 mouseDelta = GetMouseDelta();
    float sensitivity = 0.15f;
    // Orbit around the new target position using yaw and pitch
    CameraYaw(cam, -mouseDelta.x * sensitivity * DEG2RAD, true);                     // Horizontal orbit
    CameraPitch(cam, -mouseDelta.y * sensitivity * DEG2RAD, true, true, false);      // Vertical orbit
    
    // Debug text
    DrawText(TextFormat("Cam Pos: %.2f %.2f %.2f", cam->position.x, cam->position.y, cam->position.z), 10, 40, 10, DARKGRAY);
}

Vector3 GetMoveDirection(Camera c) {
    Vector3 moveDirection = {0};

    // Get the forward direction (flattened to XZ plane)
    Vector3 forward = Vector3Normalize((Vector3) {
        c.target.x - c.position.x,
        0.0f,
        c.target.z - c.position.z
    });

    // Get the lateral direction (perpendicular to forward)
    Vector3 lateral = Vector3Normalize((Vector3) {
        -forward.z,
        0.0f,
        forward.x
    });

    // Accumulate movement input
    if (IsKeyDown(KEY_W)) moveDirection = Vector3Add(moveDirection, forward);
    if (IsKeyDown(KEY_S)) moveDirection = Vector3Subtract(moveDirection, forward);
    if (IsKeyDown(KEY_D)) moveDirection = Vector3Add(moveDirection, lateral);
    if (IsKeyDown(KEY_A)) moveDirection = Vector3Subtract(moveDirection, lateral);

    // Normalize to prevent faster diagonal movement
    if (Vector3Length(moveDirection) > 0.0f)
        moveDirection = Vector3Normalize(moveDirection);

    return moveDirection;
}
