#include "raylib.h"
#include "raymath.h"

#include "collision.h"

void InitCollisionBody(CollisionBody *cb, Vector3 spawnPoint) {
    cb->transform = (Transform) {0};
    cb->transform.scale = (Vector3) {1.0f, 1.0f, 1.0f};
    cb->transform.translation = spawnPoint; 

    cb->mode = COLLIDE_BODY;
    cb->shapeCount = 0;

    for (int j = 0; j < MAX_SHAPES; j++) {
        cb->shapes[j].type = SHAPE_BOX;
        cb->shapes[j].box = (BoundingBox){
            (Vector3){0.0f, 0.0f, 0.0f},
            (Vector3){0.0f, 0.0f, 0.0f}
        };
    }
}

CollisionBody GetCollisionBodyTransformed(CollisionBody cb) {
    for (int i = 0; i < cb.shapeCount; i++) {
        switch (cb.shapes[i].type) {
            case SHAPE_BOX:
                cb.shapes[i].box.max = Vector3Add(cb.shapes[i].box.max, cb.transform.translation);
                cb.shapes[i].box.min = Vector3Add(cb.shapes[i].box.min, cb.transform.translation);
                break;
            case SHAPE_SPHERE:
                cb.shapes[i].sphere.center = Vector3Add(cb.shapes[i].sphere.center, cb.transform.translation);
                break;
        }
    } 

    return cb;
}

bool CheckCollisionBodies(CollisionBody body1, CollisionBody body2) {
    for (int i = 0; i < body1.shapeCount; i++) {
        CollisionShape shape1 = body1.shapes[i];

        for (int j = 0; j < body2.shapeCount; j++) {
            CollisionShape shape2 = body2.shapes[j];

            if (shape1.type == SHAPE_BOX && shape2.type == SHAPE_BOX) {
                return CheckCollisionBoxes(shape1.box, shape2.box);
            }
            else if (shape1.type == SHAPE_SPHERE && shape2.type == SHAPE_BOX) {
                return CheckCollisionBoxSphere(shape2.box, shape1.sphere.center, shape1.sphere.radius);
            }
            else if (shape1.type == SHAPE_BOX && shape2.type == SHAPE_SPHERE) {
                return CheckCollisionBoxSphere(shape1.box, shape2.sphere.center, shape2.sphere.radius);
            }
            else if (shape1.type == SHAPE_SPHERE && shape2.type == SHAPE_SPHERE) {
                return CheckCollisionSpheres(shape1.sphere.center, shape1.sphere.radius, shape2.sphere.center, shape2.sphere.radius);
            }
        }
    }

    return false;
}