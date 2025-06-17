#ifndef GAME_OBJECT_H 
#define GAME_OBJECT_H 

#include "raylib.h"

#include "collision.h"

#define MAX_COLLISION_BODIES 8

typedef struct {
    Model model;
    CollisionBody collisionBodies[MAX_COLLISION_BODIES];
    int collisionBodyCount;
    Vector3 position;
    Vector3 rotation; // Roll, Pitch, Yaw
} GameObject;

// Initiallize game object
void InitGameObject(GameObject *go);

#endif