#ifndef COLLISION_H
#define COLLISION_H 

#include "raylib.h"

#define MAX_SHAPES 8

// Determines type of shape
typedef enum {
    SHAPE_BOX,      // Bounding box (min, max)
    SHAPE_SPHERE    // Sphere (center, radius)
} CollisionShapeType;

typedef struct {
    CollisionShapeType type;
    union {
        BoundingBox box;
        struct {
            Vector3 center;
            float radius;
        } sphere;
    };
} CollisionShape;

// Determines behavior of CollisionBody
typedef enum {
    COLLIDE_BODY,   // Body is collidable
    COLLIDE_DETECT, // Body detects intersection
    PHYSICS_BODY,   // Body has physics collision
    PHYSICS_STATIC  // Body has static physics collision
} CollisionMode;

typedef struct {
    CollisionMode mode;
    CollisionShape shapes[MAX_SHAPES];
    int shapeCount;
} CollisionBody;

// Check for collision between two collision bodies
bool CheckCollisionBodies(CollisionBody body1, CollisionBody body2);

#endif