#include <raylib.h>

int main()
{
    InitWindow(800, 600, "RaylibTemplate");

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawText("Hello, World!", 10, 10, 20, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}