#include <stddef.h>
#include "raylib.h"

#include "scene.h"
#include "game_object.h"

void LoadScene(Scene *s, const char *fileName, bool preservePlayer) {
    s->enemyCount = 1;
    s->gameObjectCount = 2;

    Vector3 originPosition = {0};
    
    InitPlayer(&s->player, originPosition);
    InitEnemy(&s->enemies[0], originPosition);

    s->gameObjects[0] = &s->player.object;
    s->gameObjects[1] = &s->enemies[0].object;
}

void DrawSceneGameObjects(const Scene *s) {
    //DrawGameObject(&s->player.object);
    //DrawGameObject(&s->enemies[0].object);
    for (int i = 0; i < s->gameObjectCount; i++) {
       DrawGameObject(s->gameObjects[i]);
    }
}