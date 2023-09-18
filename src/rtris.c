#include "defines.h"
#include "rtris.h"
#include "block.h"

#include "raylib.h"

#include <stdio.h>
#include <string.h>

const int screenWidth = WINDOW_WIDTH;
const int screenHeight = WINDOW_HEIGHT;

const int FIELD_WIDTH = 12;
const int FIELD_HEIGHT = 18;

const int ELEMENT_SIZE = 20;

enum ColorsT
{
	BLOCK_I,
	BLOCK_J,
	BLOCK_L,
	BLOCK_O,
	BLOCK_S,
	BLOCK_T,
	BLOCK_Z
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

void drawFieldBackground(Vector2 fPos)
{
	DrawRectangle(fPos.x - 5, fPos.y - 5, FIELD_WIDTH * ELEMENT_SIZE + 10, FIELD_HEIGHT * ELEMENT_SIZE + 10, DARKGRAY);
	DrawRectangleLines(fPos.x - 5, fPos.y - 5, FIELD_WIDTH * ELEMENT_SIZE + 10, FIELD_HEIGHT * ELEMENT_SIZE + 10, WHITE);
	DrawRectangleLines(fPos.x - 1, fPos.y - 1, FIELD_WIDTH * ELEMENT_SIZE + 2, FIELD_HEIGHT * ELEMENT_SIZE + 2, WHITE);
}

void drawElement(int posX, int posY, Color color)
{
	DrawRectangle(posX, posY, ELEMENT_SIZE, ELEMENT_SIZE, color);
	DrawRectangleLines(posX, posY, ELEMENT_SIZE, ELEMENT_SIZE, WHITE);
}

void updateCurrentBlock(bool blocks[4][4], int rotation, int type)
{
	switch (type)
	{
		case BLOCK_I:
			memcpy(blocks[0], &blockI[rotation][0], 4 * sizeof(bool));
			memcpy(blocks[1], &blockI[rotation][4], 4 * sizeof(bool));
			memcpy(blocks[2], &blockI[rotation][8], 4 * sizeof(bool));
			memcpy(blocks[3], &blockI[rotation][12], 4 * sizeof(bool));
			break;
		case BLOCK_J:
			memcpy(blocks[0], &blockJ[rotation][0], 4 * sizeof(bool));
			memcpy(blocks[1], &blockJ[rotation][4], 4 * sizeof(bool));
			memcpy(blocks[2], &blockJ[rotation][8], 4 * sizeof(bool));
			memcpy(blocks[3], &blockJ[rotation][12], 4 * sizeof(bool));
			break;
		case BLOCK_L:
			memcpy(blocks[0], &blockL[rotation][0], 4 * sizeof(bool));
			memcpy(blocks[1], &blockL[rotation][4], 4 * sizeof(bool));
			memcpy(blocks[2], &blockL[rotation][8], 4 * sizeof(bool));
			memcpy(blocks[3], &blockL[rotation][12], 4 * sizeof(bool));
			break;
		case BLOCK_O:
			memcpy(blocks[0], &blockO[rotation][0], 4 * sizeof(bool));
			memcpy(blocks[1], &blockO[rotation][4], 4 * sizeof(bool));
			memcpy(blocks[2], &blockO[rotation][8], 4 * sizeof(bool));
			memcpy(blocks[3], &blockO[rotation][12], 4 * sizeof(bool));
			break;
		case BLOCK_S:
			memcpy(blocks[0], &blockS[rotation][0], 4 * sizeof(bool));
			memcpy(blocks[1], &blockS[rotation][4], 4 * sizeof(bool));
			memcpy(blocks[2], &blockS[rotation][8], 4 * sizeof(bool));
			memcpy(blocks[3], &blockS[rotation][12], 4 * sizeof(bool));
			break;
		case BLOCK_T:
			memcpy(blocks[0], &blockT[rotation][0], 4 * sizeof(bool));
			memcpy(blocks[1], &blockT[rotation][4], 4 * sizeof(bool));
			memcpy(blocks[2], &blockT[rotation][8], 4 * sizeof(bool));
			memcpy(blocks[3], &blockT[rotation][12], 4 * sizeof(bool));
			break;
		case BLOCK_Z:
			memcpy(blocks[0], &blockZ[rotation][0], 4 * sizeof(bool));
			memcpy(blocks[1], &blockZ[rotation][4], 4 * sizeof(bool));
			memcpy(blocks[2], &blockZ[rotation][8], 4 * sizeof(bool));
			memcpy(blocks[3], &blockZ[rotation][12], 4 * sizeof(bool));
			break;
	}
}

bool DoesPieceFit(bool currentBlock[4][4], char field[FIELD_WIDTH][FIELD_HEIGHT], Vector2 fPos, Vector2 pPos)
{


	return true;
}

void appMain()
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(screenWidth, screenHeight, TextFormat("Raytris v%s", VERSION));

	// SetExitKey(0);
	SetTargetFPS(60);

	SetTraceLogLevel(LOG_TRACE);

	char fieldContent[FIELD_WIDTH][FIELD_HEIGHT];
	Vector2 fieldPos = {GetScreenWidth()/2 - (FIELD_WIDTH*ELEMENT_SIZE)/2, GetScreenHeight()/2 - (FIELD_HEIGHT*ELEMENT_SIZE)/2};
	
	Vector2 currentBlockPos = { 5, 0 };
	bool currentBlock[4][4];
	int currentBlockType = 0;
	int currentBlockRotation = 0;

	// Init fieldContent to 0
	for (int w = 0; w < FIELD_WIDTH; w++)
		for (int h = 0; h < FIELD_HEIGHT; h++)
			fieldContent[w][h] = 0;

	int LinesCleared = 0;
	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose())
	{
		// Update
		//----------------------------------------------------------------------------------
		// LEFT MOVEMENT
		if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT))
		{
			if (DoesPieceFit(currentBlock, fieldContent, fieldPos, currentBlockPos)) {
				currentBlockPos.x--;
			}
		}

		// RIGHT MOVEMENT
		if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT))
		{
			if (currentBlockPos.x != FIELD_WIDTH)
				currentBlockPos.x++;
		}

		// DOWN MOVEMENT
		if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN))
		{
			if (currentBlockPos.y != FIELD_HEIGHT)
				currentBlockPos.y++;
		}

		// CLOCKWISE ROTATION
		if (IsKeyPressed(KEY_E) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
		{
			currentBlockRotation++;
			if (currentBlockRotation > 3)
				currentBlockRotation = 0;
		}
		
		// COUNTERCLOCKWISE ROTATION
		if (IsKeyPressed(KEY_Q) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			currentBlockRotation--;
			if (currentBlockRotation < 0)
				currentBlockRotation = 3;
		}



		updateCurrentBlock(currentBlock, currentBlockRotation, currentBlockType);
		
		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(BLACK);

		drawFieldBackground(fieldPos);

		for (int w = 0; w < FIELD_WIDTH; w++)
		{
			for (int h = 0; h < FIELD_HEIGHT; h++)
			{
				unsigned char c = fieldContent[w][h];
				if (c != 0)
					drawElement(fieldPos.x + (w * ELEMENT_SIZE), fieldPos.y + (h * ELEMENT_SIZE), tColors[c]);
			}
		}

		// Draw Current Block
		for (int w = 0; w < 4; w++) {
			for (int h = 0; h < 4; h++) {
				if (currentBlock[w][h]) {
					drawElement(
						fieldPos.x + (currentBlockPos.x + h) * ELEMENT_SIZE,
						fieldPos.y + (currentBlockPos.y + w) * ELEMENT_SIZE,
						tColors[currentBlockType]
					);
				}
			}
		}

		DrawText(TextFormat("Rows Cleared: %d", LinesCleared), 10, 10, 18, RAYWHITE);

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseWindow();
}