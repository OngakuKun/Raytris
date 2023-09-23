#include "defines.h"
#include "raylib.h"

int main() {
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, TextFormat("%s v%s", NAME, VERSION));

	while(!WindowShouldClose()) {

		BeginDrawing();

			ClearBackground(BLACK);

		EndDrawing();
	}

	CloseWindow();

	return 0;
}