#ifndef COLLISION_H
#define COLLISION_H 

#include "raylib.h"

#define MAX_SHAPES 8

// Determines type of shape
typedef enum {
    SHAPE_BOX,      // Bounding box (min, max)
    SHAPE_SPHERE,   // Sphere (center, radius)
    SHAPE_MESH      // Mesh (Mesh)
} CollisionShapeType;

typedef struct {
    CollisionShapeType type;
    union {
        BoundingBox box;
        struct {
            Vector3 center;
            float radius;
        } sphere;
        Mesh mesh;
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
    Transform transform;
    CollisionMode mode;
    CollisionShape shapes[MAX_SHAPES];
    int shapeCount;
} CollisionBody;

// Initiallize collision body
void InitCollisionBody(CollisionBody *cb, Vector3 spawnPoint);
// Reposition collision body in reference to transform 
CollisionBody GetCollisionBodyTransformed(CollisionBody cb);
// Check for collision between two collision bodies
bool CheckCollisionBodies(CollisionBody body1, CollisionBody body2);

// Check for collision between mesh and sphere TODO: Add transform for mesh
bool CheckCollisionMeshSphere(Mesh mesh, Vector3 center, float radius);
// Get closest point on a triangle to point p
Vector3 ClosestPointOnTriangle(Vector3 p, Vector3 a, Vector3 b, Vector3 c);

#endif