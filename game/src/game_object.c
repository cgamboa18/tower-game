#include "raylib.h"

#include "game_object.h"
#include "collision.h"

void InitGameObject(GameObject *go) {
    go->position = (Vector3){0.0f, 0.0f, 0.0f};
    go->rotation = (Vector3){0.0f, 0.0f, 0.0f};

    go->model = (Model){0};
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