#include "defines.h"
#include "raylib.h"

#include "field.h"
#include "timer.h"

bool showDebugMenu;

double updateTick;
double updateTime;
double renderTime;

int main()
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, TextFormat("%s v%s", NAME, VERSION));

	SetExitKey(0);
	SetWindowMinSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	if (IsWindowState(FLAG_VSYNC_HINT))

	// Load RenderTexture for PixelPerfect Rendering
	setFieldRenderTexture(LoadRenderTexture(800, 450));

	// Initialize Field
	resetField();

	showDebugMenu = false;
	updateTick = GetTime();

	while(!WindowShouldClose()) 
	{
		STARTTIMER(TIMER_UPDATE);

		// * USER INPUT

		if (IsKeyPressed(KEY_F3))
			showDebugMenu = !showDebugMenu;

		// * USER INPUT

		if (GetTime() > updateTick + 0.5f) 
		{
			updateTime = getTimer(TIMER_UPDATE);// / 1000;
			renderTime = GetFrameTime() / 1000;

			updateTick = GetTime();
		}

		ENDTIMER(TIMER_UPDATE);

		BeginDrawing();

			ClearBackground(BLACK);

			DrawRectangleGradientV(0, 0, GetScreenWidth(), GetScreenHeight(), DARKBLUE, BLACK);

			renderField();

			if (showDebugMenu)
			{
				DrawTextPro(GetFontDefault(), TextFormat("%5.0d FPS", GetFPS()), (Vector2) {GetScreenWidth() - 10, 20}, MeasureTextEx(GetFontDefault(), TextFormat("%5.0d FPS", GetFPS()), 10.0f, 2.0f), 0.0f, 10.0f, 2.0f, RAYWHITE);
				DrawText(TextFormat("UpdateTime: %5.3fs", updateTime), 10, 10, 10.0f, RAYWHITE);
				DrawText(TextFormat("RenderTime: %5.3fms", renderTime), 10, 20, 10.0f, RAYWHITE);
			}

		EndDrawing();
	}

	CloseWindow();

	return 0;
}