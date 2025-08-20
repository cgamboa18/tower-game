#ifndef GAME_OBJECT_H 
#define GAME_OBJECT_H 

#include "raylib.h"

#include "collision.h"

#define MAX_COLLISION_BODIES 8

typedef enum {
    NONE_ID,
    SURFACE_ID,
    PLAYER_ID,
    BEAST_ID
} GameObjectId;

typedef struct {
    GameObjectId id;

    Transform transform;
    Vector3 velocity;

    Model model;
    CollisionBody collisionBodies[MAX_COLLISION_BODIES];
    int collisionBodyCount;
} GameObject;

// Initiallize game object
void InitGameObject(GameObject *go, Vector3 spawnPoint);
// Update game object motion
void UpdateGameObjectMotion(GameObject *go, Vector3 velocity);

// Draw 3D game object to camera view
void DrawGameObject(const GameObject *go);
// Update collisions for game object using parent's collision callback function (ctx contains parent pointer context for callback updates) 
void UpdateGameObjectSceneCollisions(GameObject *go, GameObject **gameObjects, int gameObjectCount, void (*CollisionCallback)(CollisionInfo, GameObject*, GameObject*, void*), void *ctx);
// Physics collision behavior (Assume that motion has been already been applied)
void CollideAndSlide(GameObject *go, CollisionInfo collision);

#endif