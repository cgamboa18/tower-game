#include <stddef.h>

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
            /*
            case SHAPE_MESH:
                // KEEP AN EYE ON THIS
                for (int j = 0; j < cb.shapes[i].mesh.vertexCount; j++) {
                    cb.shapes[i].mesh.vertices[j * 3 + 0] += cb.transform.translation.x; 
                    cb.shapes[i].mesh.vertices[j * 3 + 1] += cb.transform.translation.y; 
                    cb.shapes[i].mesh.vertices[j * 3 + 2] += cb.transform.translation.z; 
                }
                break;
            */
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
            else if (shape1.type == SHAPE_MESH && shape2.type == SHAPE_SPHERE) {
                return CheckCollisionMeshSphere(shape1.mesh, shape2.sphere.center, shape2.sphere.radius);
            }
            else if (shape1.type == SHAPE_SPHERE && shape2.type == SHAPE_MESH) {
                return CheckCollisionMeshSphere(shape2.mesh, shape1.sphere.center, shape1.sphere.radius);
            }
        }
    }

    return false;
}

bool CheckCollisionMeshSphere(Mesh mesh, Vector3 center, float radius) {
    if (mesh.vertices == NULL || mesh.triangleCount <= 0) return false;

    Vector3 *vertices = (Vector3 *)mesh.vertices;
    short unsigned int *indices = mesh.indices;

    for (int i = 0; i < mesh.triangleCount; i++) {
        Vector3 a, b, c;

        if (mesh.indices != NULL) {
            a = vertices[indices[i * 3 + 0]];
            b = vertices[indices[i * 3 + 1]];
            c = vertices[indices[i * 3 + 2]];
        } else {
            a = vertices[i * 3 + 0];
            b = vertices[i * 3 + 1];
            c = vertices[i * 3 + 2];
        }

        Vector3 closest = ClosestPointOnTriangle(center, a, b, c);
        Vector3 difference = Vector3Subtract(center, closest);
        float distance = Vector3Length(difference);

        if (distance < radius) return true;
    }

    return false;
}

Vector3 ClosestPointOnTriangle(Vector3 p, Vector3 a, Vector3 b, Vector3 c) { // TODO: Check for optimization using pre calculated mesh elements
    Vector3 ab = Vector3Subtract(b, a);
    Vector3 ac = Vector3Subtract(c, a);
    Vector3 ap = Vector3Subtract(p, a);

    float d1 = Vector3DotProduct(ab, ap);
    float d2 = Vector3DotProduct(ac, ap);
    if (d1 <= 0.0f && d2 <= 0.0f) return a;

    Vector3 bp = Vector3Subtract(p, b);
    float d3 = Vector3DotProduct(ab, bp);
    float d4 = Vector3DotProduct(ac, bp);
    if (d3 >= 0.0f && d4 <= d3) return b;

    float vc = d1 * d4 - d3 * d2;
    if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f) {
        float v = d1 / (d1 - d3);
        return Vector3Add(a, Vector3Scale(ab, v));
    }

    Vector3 cp = Vector3Subtract(p, c);
    float d5 = Vector3DotProduct(ab, cp);
    float d6 = Vector3DotProduct(ac, cp);
    if (d6 >= 0.0f && d5 <= d6) return c;

    float vb = d5 * d2 - d1 * d6;
    if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f) {
        float w = d2 / (d2 - d6);
        return Vector3Add(a, Vector3Scale(ac, w));
    }

    float va = d3 * d6 - d5 * d4;
    if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f) {
        float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
        Vector3 bc = Vector3Subtract(c, b);
        return Vector3Add(b, Vector3Scale(bc, w));
    }

    float denom = 1.0f / (va + vb + vc);
    float v = vb * denom;
    float w = vc * denom;
    Vector3 scaledAb = Vector3Scale(ab, v);
    Vector3 scaledAc = Vector3Scale(ac, w);
    return Vector3Add(a, Vector3Add(scaledAb, scaledAc));
}