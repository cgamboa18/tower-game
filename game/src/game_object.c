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
    go->collisionBodies[0].shapes[0].sphere.radius = 1.1f;
    /* TEST ***********************/
}

void UpdateGameObjectMotion(GameObject *go, Vector3 velocity) {
    // Update position based on velocity
    go->transform.translation = Vector3Add(go->transform.translation, velocity);

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
    DrawModelEx(
        go->model,
        go->transform.translation,
        rotationAxis,
        rotationAngle,
        go->transform.scale,
        BLUE 
    );

    bool debugCollision = true;
    if (debugCollision) {
        CollisionBody b = GetCollisionBodyTransformed(go->collisionBodies[0]);

        if (b.shapes[0].type == SHAPE_SPHERE) {
            DrawSphereWires(b.shapes[0].sphere.center, b.shapes[0].sphere.radius, 10, 10, RED);
        } else if (b.shapes[0].type == SHAPE_BOX) {
            DrawBoundingBox(b.shapes[0].box, RED);
        } else if (b.shapes[0].type == SHAPE_MESH) {
            DrawModelWires(b.shapes[0].debugMeshModel, b.transform.translation, 1.0f, RED);
        }
    }
        
}

void UpdateGameObjectSceneCollisions(GameObject *go, GameObject **gameObjects, int gameObjectCount, void (*CollisionCallback)(CollisionInfo, GameObject*, GameObject*, int, int, void*), void *ctx) {
    for (int i = 0; i < gameObjectCount; i++) {
        if (go == gameObjects[i])
            continue;

        // Check collision between player bodies and other game object bodies
        // GetCollisionBodyTransformed must be used to adjust for gameObject positions
        for (int j = 0; j < go->collisionBodyCount; j++) { 
            CollisionBody transformedBody1 = GetCollisionBodyTransformed(go->collisionBodies[j]);

            for (int k = 0; k < gameObjects[i]->collisionBodyCount; k++) {
                CollisionBody transformedBody2 = GetCollisionBodyTransformed(gameObjects[i]->collisionBodies[k]);

                CollisionInfo collision = CheckCollisionBodies(transformedBody1, transformedBody2);
                if (collision.hit == true) { // TODO: Add more collision info, j and k can be wrapped into collision 
                    CollisionCallback(collision, go, gameObjects[i], j, k, ctx); 
                }
            }
        }
    }
}

void CollideAndSlide(GameObject *go, CollisionInfo collision) {
    UpdateGameObjectMotion(go, Vector3Negate(go->velocity));

    float dot = Vector3DotProduct(go->velocity, collision.normal);
    Vector3 slideVelocity;
    
    // If we're moving into the surface (dot < 0)
    if (dot < 0) {
        // Remove only the component going into the surface
        Vector3 normalVelocity = Vector3Scale(collision.normal, dot);
        slideVelocity = Vector3Subtract(go->velocity, normalVelocity);
    } else {
        // Keep original velocity if we're moving away
        slideVelocity = go->velocity;
    }

    // Apply slide movement
    UpdateGameObjectMotion(go, slideVelocity);

    // Update stored velocity for next frame
    if (collision.normal.y > 0.7f) {
        // On ground/floor - zero vertical velocity
        slideVelocity.y = 0;
    }
    go->velocity = slideVelocity;
}