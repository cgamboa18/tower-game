#ifndef GAME_OBJECT_H 
#define GAME_OBJECT_H 

#include "raylib.h"

#include "collision.h"

#define MAX_COLLISION_BODIES 8

typedef struct {
    Transform transform;
    Vector3 velocity;

    Model model;
    CollisionBody collisionBodies[MAX_COLLISION_BODIES];
    int collisionBodyCount;
} GameObject;

// Initiallize game object
void InitGameObject(GameObject *go);
// Update game object motion
void UpdateGameObjectMotion(GameObject *go);
// Draw 3D game object to camera view
void DrawGameObject(GameObject go);

#endif