#include "raylib.h"

#include "surface.h"
#include "game_object.h"

void InitSurface(Surface *s, Vector3 position) {
    InitGameObject(&s->object, position);
    s->object.id = SURFACE_ID;
    s->object.model = LoadModel("resources/cube.obj");
    s->object.collisionBodies[0].shapes[0].type = SHAPE_MESH;

    s->object.collisionBodies[0].shapes[0].debugMeshModel = LoadModel("resources/cube.obj");
    s->object.collisionBodies[0].shapes[0].mesh = s->object.collisionBodies[0].shapes[0].debugMeshModel.meshes[0];

    ApplyTransformCollisionShapeMesh(&s->object.collisionBodies[0].shapes[0], s->object.transform);

    s->object.collisionBodies[0].shapes[0].debugMeshModel.meshes[0] = s->object.collisionBodies[0].shapes[0].mesh; 
    UploadMesh(&s->object.collisionBodies[0].shapes[0].debugMeshModel.meshes[0], false);
}