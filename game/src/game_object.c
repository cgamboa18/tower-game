#include "raylib.h"
#include "raymath.h"

#include "game_object.h"
#include "collision.h"

void InitGameObject(GameObject *go) {
    go->transform = (Transform){0};
    go->transform.scale = (Vector3) {1.0f, 1.0f, 1.0f};

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

void DrawGameObject(GameObject go) {
    Vector3 rotationAxis;
    float rotationAngle;
    QuaternionToAxisAngle(go.transform.rotation, &rotationAxis, &rotationAngle);

    DrawModelEx(
        go.model,
        go.transform.translation,
        rotationAxis,
        rotationAngle,
        go.transform.scale,
        GREEN 
    );
        
}