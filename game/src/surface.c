#include "raylib.h"

#include "surface.h"
#include "game_object.h"

void InitSurface(Surface *s, Vector3 position) {
    InitGameObject(&s->object, position);
    s->object.id = SURFACE_ID;
    s->object.model = LoadModel("resources/cube.obj");
    s->object.collisionBodies[0].shapes[0].type = SHAPE_MESH;
    s->object.collisionBodies[0].shapes[0].mesh = LoadModel("resources/cube.obj").meshes[0];
}