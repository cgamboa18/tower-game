#include "raylib.h"
#include "raymath.h"

#include "game_object.h"
#include "collision.h"

void InitGameObject(GameObject *go, Vector3 spawnPoint) {
    go->transform = (Transform){0};
    go->transform.scale = (Vector3) {1.0f, 1.0f, 1.0f};
    go->transform.translation = spawnPoint;

    go->velocity = (Vector3) {0};

    go->model = LoadModel("resources/icosahedron.obj"); 
    go->collisionBodyCount = 0;
    for (int i = 0; i < MAX_COLLISION_BODIES; i++) {
        go->collisionBodies[i].mode = COLLIDE_BODY;
        go->collisionBodies[i].shapeCount = 0;

        for (int j = 0; j < MAX_SHAPES; j++) {
            go->collisionBodies[i].shapes[j].type = SHAPE_BOX;
            go->collisionBodies[i].shapes[j].box = (BoundingBox){
                (Vector3){0.0f, 0.0f, 0.0f},
                (Vector3){0.0f, 0.0f, 0.0f}
            };
        }
    }
}

void UpdateGameObjectMotion(GameObject *go) {
    // Update position based on velocity
    go->transform.translation.x += go->velocity.x;
    go->transform.translation.y += go->velocity.y;
    go->transform.translation.z += go->velocity.z;
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
        
}