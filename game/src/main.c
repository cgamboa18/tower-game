#include <stddef.h>
#include "raylib.h"

// Header Dependancies
#define CJSON_IMPLEMENTATION
#include "cJSON-1.7.18/cJSON.h"

#include "scene.h"
#include "player.h"
#include "enemy.h"

#define MAX_GAME_OBJECTS 128

// Local Variables Definition
Scene gameScene = {0};

// Local Functions Declaration
static void UpdateDrawFrame(void);

// Main entry point
int main() {
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib");
    LoadScene(&gameScene, "resources/scene.json", false);

    SetTargetFPS(60);
    DisableCursor();

    // Main game loop
    while (!WindowShouldClose()) {
        // Update
        UpdateScene(&gameScene);

        UpdateDrawFrame();
    }

    // De-Initialization
    CloseWindow(); // Close window and OpenGL context

    return 0;
}

// Update and draw game frame
static void UpdateDrawFrame(void) {
    BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(gameScene.player.camera);
            DrawSceneGameObjects(&gameScene);
            DrawGrid(10, 1.0f);

        EndMode3D();

        DrawFPS(10, 10);

    EndDrawing();
}
