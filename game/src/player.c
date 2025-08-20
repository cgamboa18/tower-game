#include "raylib.h"
#include "raymath.h"
#include "rcamera.h"

#include "player.h"
#include "game_object.h"

void InitPlayer(Player *p, Vector3 spawnPoint) {
    p->camera = (Camera3D) {
        .position = (Vector3){spawnPoint.x + 0.0f, spawnPoint.y + 5.0f, spawnPoint.z + 10.0f},
        .target = spawnPoint,
        .up = (Vector3){0.0f, 1.0f, 0.0f},
        .fovy = 60.0f,
        .projection = CAMERA_PERSPECTIVE
    }; 
    p->state = IDLE;
    InitGameObject(&p->object, spawnPoint);
    p->object.id = PLAYER_ID;

    p->health = 100;
    p->energy = 100;
}

void UpdatePlayer(Player *p, GameObject **gameObjects, int gameObjectCount) {
    // Update motion for game object
    UpdateGameObjectMotion(&p->object, p->object.velocity);
    // Update collisions for game object
    UpdateGameObjectSceneCollisions(&p->object, gameObjects, gameObjectCount, &PlayerCollisionCallback, p);
    // Update the players actions
    UpdatePlayerAction(p);
}

void UpdatePlayerAction(Player *p) {
    Vector3 *playerVelocity = &p->object.velocity;
    Vector3 moveDirection = GetMoveDirection(p->camera);

    // Determine current player state
    UpdatePlayerState(p, moveDirection);

    // Update camera position
    UpdatePlayerCamera(p);
    
    playerVelocity->y -= 0.5f * GetFrameTime();

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

    Vector3 targetDelta = Vector3Subtract(cam->target, lastTarget);
    Vector3 toTarget = Vector3Subtract(cam->target, cam->position);
    // Perpedicular movement
    float deltaDot = Vector3DotProduct(Vector3Normalize(targetDelta), Vector3Normalize(toTarget));

    if (Vector3LengthSqr(targetDelta) > 0.0001f && fabs(deltaDot) > 0.5f) {
        // Move the camera position by the target delta to preserve relative orbit position
        cam->position = Vector3Add(cam->position, targetDelta);
    }

    // Snap camera to orbital
    currentDistance = Vector3Distance(cam->position, cam->target);
    float adjust = desiredDistance - currentDistance;
    CameraMoveToTarget(cam, adjust);

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

    if (IsKeyDown(KEY_W)) moveDirection = Vector3Add(moveDirection, forward);
    if (IsKeyDown(KEY_S)) moveDirection = Vector3Subtract(moveDirection, forward);
    if (IsKeyDown(KEY_D)) moveDirection = Vector3Add(moveDirection, lateral);
    if (IsKeyDown(KEY_A)) moveDirection = Vector3Subtract(moveDirection, lateral);

    // Normalize to prevent faster diagonal movement
    if (Vector3Length(moveDirection) > 0.0f)
        moveDirection = Vector3Normalize(moveDirection);

    return moveDirection;
}

void PlayerCollisionCallback(CollisionInfo collision, GameObject *object1, GameObject *object2, void *ctx) {
    Player *p = (Player *) ctx;

    if (object2->id == SURFACE_ID) {
        CollideAndSlide(&p->object, collision);
    }

    DrawText(TextFormat("Collision callback: %d", p->object.id), 10, 60, 10, DARKGRAY);
}