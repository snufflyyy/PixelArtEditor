#include <raylib.h>

#include "application.h"

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1280, 720, "Pixel Art Editor");

    SetTargetFPS(60);

    createApplication();

    while (!WindowShouldClose()) {
        updateApplication();

        BeginDrawing();
            renderApplication();
        EndDrawing();
    }

    destroyApplication();
    CloseWindow();

    return 0;
}
