#include "raylib.h"

// Local headers
#include "player.h"

// Local Variables Definition
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

    SetTargetFPS(-1);

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
    UpdatePlayer(&player);

    // Draw
    BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(player.camera);
            DrawCubeWires(originPosition, 2.0f, 2.0f, 2.0f, MAROON);
            DrawGrid(10, 1.0f);

        EndMode3D();

        DrawText("This is a raylib example", 10, 40, 20, DARKGRAY);
        DrawFPS(10, 10);

    EndDrawing();
}
