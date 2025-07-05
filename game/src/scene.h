#ifndef SCENE_H 
#define SCENE_H

#include "raylib.h"

#include "player.h"
#include "enemy.h"
#include "game_object.h"

#define MAX_ENEMIES 64 
#define MAX_GAME_OBJECTS 128

typedef struct {
    Player player;
    Enemy enemies[MAX_ENEMIES];
    int enemyCount;

    GameObject *gameObjects[MAX_GAME_OBJECTS]; 
    int gameObjectCount;
} Scene;

// Update all elements of the scene
void UpdateScene(Scene *s);
// Load scene from JSON file
void LoadScene(Scene *s, const char *fileName, bool preservePlayer);
// Draw all of the scenes gameObjects to the screen
void DrawSceneGameObjects(const Scene *s);

#endif