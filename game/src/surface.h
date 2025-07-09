#ifndef SURFACE_H 
#define SURFACE_H

#include "raylib.h"

#include "game_object.h"

// Intended for floors, walls, etc.
typedef struct {
    GameObject object;
} Surface;

// Initiallize surface 
void InitSurface(Surface *s, Vector3 position);

#endif