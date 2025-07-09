#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include "cJSON-1.7.18/cJSON.h"

#include "scene.h"
#include "game_object.h"

void UpdateScene(Scene *s) {
    UpdatePlayer(&s->player, s->gameObjects, s->gameObjectCount);
}

void LoadScene(Scene *s, const char *fileName, bool preservePlayer) {
    // Open JSON file
    FILE *file = fopen(fileName, "rb");
    if (!file) {
        TraceLog(LOG_ERROR, "Could not open scene file: %s", fileName);
        return;
    }

    // Get file size and read content
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    rewind(file);

    char *data = malloc(length + 1);
    fread(data, 1, length, file);
    data[length] = '\0';
    fclose(file);

    cJSON *root = cJSON_Parse(data);
    free(data);

    if (!root) {
        TraceLog(LOG_ERROR, "Failed to parse scene JSON");
        return;
    }

    s->gameObjectCount = 0;
    s->enemyCount = 0;

    // Load Player
    if (!preservePlayer) {
        cJSON *playerObj = cJSON_GetObjectItem(root, "player");
        if (playerObj) {
            cJSON *posArray = cJSON_GetObjectItem(playerObj, "position");
            Vector3 pos = {0};
            if (cJSON_GetArraySize(posArray) == 3) {
                pos.x = (float)cJSON_GetArrayItem(posArray, 0)->valuedouble;
                pos.y = (float)cJSON_GetArrayItem(posArray, 1)->valuedouble;
                pos.z = (float)cJSON_GetArrayItem(posArray, 2)->valuedouble;
            }

            InitPlayer(&s->player, pos);

            cJSON *health = cJSON_GetObjectItem(playerObj, "health");
            cJSON *energy = cJSON_GetObjectItem(playerObj, "energy");

            if (health) s->player.health = health->valueint;
            if (energy) s->player.energy = energy->valueint;
        } else {
            InitPlayer(&s->player, (Vector3){0});
        }
    }

    s->gameObjects[s->gameObjectCount++] = &s->player.object;

    // Load Enemies
    cJSON *enemies = cJSON_GetObjectItem(root, "enemies");
    if (enemies && cJSON_IsArray(enemies)) {
        int enemyIndex = 0;
        cJSON *enemyItem = NULL;

        cJSON_ArrayForEach(enemyItem, enemies) {
            if (enemyIndex >= MAX_ENEMIES) break;

            cJSON *posArray = cJSON_GetObjectItem(enemyItem, "position");
            Vector3 pos = {0};

            if (cJSON_GetArraySize(posArray) == 3) {
                pos.x = (float)cJSON_GetArrayItem(posArray, 0)->valuedouble;
                pos.y = (float)cJSON_GetArrayItem(posArray, 1)->valuedouble;
                pos.z = (float)cJSON_GetArrayItem(posArray, 2)->valuedouble;
            }

            InitEnemy(&s->enemies[enemyIndex], pos);

            s->gameObjects[s->gameObjectCount++] = &s->enemies[enemyIndex].object;
            enemyIndex++;
        }

        s->enemyCount = enemyIndex;
    }

    cJSON_Delete(root);

    /***************/
    InitSurface(&s->surfaces, (Vector3){0,0,0}); 
    s->gameObjects[s->gameObjectCount++] = &s->surfaces.object;
    /***************/
}

void DrawSceneGameObjects(const Scene *s) {
    for (int i = 0; i < s->gameObjectCount; i++) {
       DrawGameObject(s->gameObjects[i]);
    }
}