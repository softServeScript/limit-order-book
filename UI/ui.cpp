#include "ui.hpp"

void BuildUI() {
    SetConfigFlags(FLAG_WINDOW_HIDDEN); // Hide window || could also use "FLAG_WINDOW_RESIZABLE"
    InitWindow(0, 0, "Limit Order Book"); // (960, 540) another generic option

    int monitor = GetCurrentMonitor();
    int width = GetMonitorWidth(monitor) / 2.5;
    int height = GetMonitorHeight(monitor) / 2.5;

    SetWindowSize(width, height);
    ClearWindowState(FLAG_WINDOW_HIDDEN); // Unhide window

    SetTargetFPS(30);

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);
        EndDrawing();
    }

    CloseWindow();
}