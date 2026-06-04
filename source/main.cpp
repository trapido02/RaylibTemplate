#include <raylib.h>

auto main() -> int
{
    constexpr int windowWidth = 800;
    constexpr int windowHeight = 600;

    InitWindow(windowWidth, windowHeight, "RaylibTemplate");

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        constexpr int positionX = 10;
        constexpr int positionY = 10;
        constexpr int fontSize = 20;

        DrawText("Hello, World!", positionX, positionY, fontSize, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
