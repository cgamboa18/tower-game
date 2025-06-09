#include "raylib.h"

#include "player.h"

void InitPlayer(Player *p, Vector3 sp) {
    p->position = sp;
    p->rotation = 0.0f;

    p->health = 100;
    p->energy = 100;
    
    p->state = IDLE;
    p->camera = (Camera3D) {
        .position = (Vector3){sp.x + 0.0f, sp.y + 5.0f, sp.z + 10.0f},
        .target = sp,
        .up = (Vector3){0.0f, 1.0f, 0.0f},
        .fovy = 60.0f,
        .projection = CAMERA_PERSPECTIVE
    };
}

void UpdatePlayer(Player *p) {
    UpdateCamera(&p->camera, CAMERA_ORBITAL);
}