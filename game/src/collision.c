#include "raylib.h"

#include "collision.h"

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