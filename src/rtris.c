#include "defines.h"
#include "rtris.h"

#include "raylib.h"

const int screenWidth = WINDOW_WIDTH;
const int screenHeight = WINDOW_HEIGHT;

const Vector2 fieldPos = {60, 60};
const int FIELD_WIDTH = 12;
const int FIELD_HEIGHT = 18;

const int ELEMENT_SIZE = 20;

enum ColorsT {
    COLOR_I,
    COLOR_J,
    COLOR_L,
    COLOR_O,
    COLOR_S,
    COLOR_T,
    COLOR_Z
};

const Color tColors[7] = 
{
    {0, 255, 255, 255},
    {0, 0, 255, 255},
    {255, 127, 0, 255},
    {255, 255, 0, 255},
    {0, 255, 0, 255},
    {128, 0, 128, 255},
    {255, 0, 0, 255}
};

void drawFieldBackground()
{
    DrawRectangle(fieldPos.x - 5, fieldPos.y - 5, FIELD_WIDTH * ELEMENT_SIZE + 10, FIELD_HEIGHT * ELEMENT_SIZE + 10, DARKGRAY);
    DrawRectangleLines(fieldPos.x - 5, fieldPos.y - 5,  FIELD_WIDTH * ELEMENT_SIZE + 10, FIELD_HEIGHT * ELEMENT_SIZE + 10, WHITE);
    DrawRectangleLines(fieldPos.x - 1, fieldPos.y - 1, FIELD_WIDTH * ELEMENT_SIZE + 2, FIELD_HEIGHT * ELEMENT_SIZE + 2, WHITE);

}

void drawElement(int posX, int posY, Color color)
{
    DrawRectangle(posX, posY, ELEMENT_SIZE, ELEMENT_SIZE, color);
    DrawRectangleLines(posX, posY, ELEMENT_SIZE, ELEMENT_SIZE, WHITE);
}

void appMain()
{
    InitWindow(screenWidth, screenHeight, "Raytris - Basic Window");

    // SetExitKey(0);

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
        //----------------------------------------------------------------------------------

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK);

            drawFieldBackground();

            for (int w = 0; w < FIELD_WIDTH; w++)
                for (int h = 0; h < FIELD_HEIGHT; h++)
                    if (GetRandomValue(0,100) < 16) drawElement(fieldPos.x + (w * ELEMENT_SIZE), fieldPos.y + (h * ELEMENT_SIZE), tColors[GetRandomValue(0, 6)]);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();
}