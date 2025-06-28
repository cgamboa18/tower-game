#include "raylib.h"

#include "player.h"

#define MAX_GAME_OBJECTS 128

// Local Variables Definition
GameObject gameObjects[MAX_GAME_OBJECTS];
Player player = {0};
Vector3 originPosition = {0};

// Local Functions Declaration
static void UpdateDrawFrame(void);

// Main entry point
int main() {
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib");
    InitPlayer(&player, originPosition);

    SetTargetFPS(60);
    DisableCursor();

    // Main game loop
    while (!WindowShouldClose()) {
        UpdateDrawFrame();
    }

    // De-Initialization
    CloseWindow(); // Close window and OpenGL context

    return 0;
}

// Update and draw game frame
static void UpdateDrawFrame(void) {
    // Update
    UpdatePlayerAction(&player);

    // Draw
    BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(player.camera);
            DrawGameObject(player.object);
            DrawGrid(10, 1.0f);

        EndMode3D();

        DrawFPS(10, 10);

    EndDrawing();
}
