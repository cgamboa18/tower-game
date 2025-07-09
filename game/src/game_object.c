#include "raylib.h"
#include "raymath.h"

#include "game_object.h"
#include "collision.h"

void InitGameObject(GameObject *go, Vector3 spawnPoint) {
    go->id = NONE_ID;
    go->transform = (Transform){0};
    go->transform.scale = (Vector3) {1.0f, 1.0f, 1.0f};
    go->transform.translation = spawnPoint;

    go->velocity = (Vector3) {0};

    go->model = LoadModel("resources/icosahedron.obj"); 
    go->collisionBodyCount = 0;

    for (int i = 0; i < MAX_COLLISION_BODIES; i++) {
        InitCollisionBody(&go->collisionBodies[i], spawnPoint);
    }

    /* TEST ***********************/
    go->collisionBodyCount = 1; // TEST
    go->collisionBodies[0].mode = COLLIDE_BODY;
    go->collisionBodies[0].shapeCount = 1;
    go->collisionBodies[0].shapes[0].type = SHAPE_SPHERE; 
    go->collisionBodies[0].shapes[0].sphere.center = Vector3Zero();
    go->collisionBodies[0].shapes[0].sphere.radius = 1.5f;
    /* TEST ***********************/
}

void UpdateGameObjectMotion(GameObject *go) {
    // Update position based on velocity
    go->transform.translation.x += go->velocity.x;
    go->transform.translation.y += go->velocity.y;
    go->transform.translation.z += go->velocity.z;

    // Update collision body transforms to match game object
    for (int i = 0; i < go->collisionBodyCount; i++){
        go->collisionBodies[i].transform = go->transform;
    }
}

void DrawGameObject(const GameObject *go) {
    // Get DrawModel rotation arguments
    Vector3 rotationAxis;
    float rotationAngle;
    QuaternionToAxisAngle(go->transform.rotation, &rotationAxis, &rotationAngle);

    // Draw model to screen based on game object position
    DrawModelWiresEx(
        go->model,
        go->transform.translation,
        rotationAxis,
        rotationAngle,
        go->transform.scale,
        BLUE 
    );

    if (true) {
        CollisionBody b = GetCollisionBodyTransformed(go->collisionBodies[0]);
        DrawSphereWires(b.shapes[0].sphere.center, b.shapes[0].sphere.radius, 10, 10, RED);
    }
        
}

void UpdateGameObjectSceneCollisions(GameObject *go, GameObject **gameObjects, int gameObjectCount, void (*CollisionCallback)(GameObject*, GameObject*, int, int, void*), void *ctx) {
    for (int i = 0; i < gameObjectCount; i++) {
        if (go == gameObjects[i])
            continue;

        // Check collision between player bodies and other game object bodies
        // GetCollisionBodyTransformed must be used to adjust for gameObject positions
        for (int j = 0; j < go->collisionBodyCount; j++) { 
            CollisionBody transformedBody1 = GetCollisionBodyTransformed(go->collisionBodies[j]);

            for (int k = 0; k < gameObjects[i]->collisionBodyCount; k++) {
                CollisionBody transformedBody2 = GetCollisionBodyTransformed(gameObjects[i]->collisionBodies[k]);

                if (CheckCollisionBodies(transformedBody1, transformedBody2)) {
                    CollisionCallback(go, gameObjects[i], j, k, ctx); 
                }
            }
        }
    }
}