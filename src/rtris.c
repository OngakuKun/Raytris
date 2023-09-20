#include "defines.h"
#include "rtris.h"
#include "block.h"

#include "raylib.h"

#include <stdio.h>
#include <string.h>

#define FIELD_WIDTH 12
#define FIELD_HEIGHT 18

const int screenWidth = WINDOW_WIDTH;
const int screenHeight = WINDOW_HEIGHT;

const int ELEMENT_SIZE = 20;

char fieldContent[FIELD_WIDTH][FIELD_HEIGHT] = {0};

Vector2 currentBlockPos;
bool currentBlock[4][4];
int currentBlockType;
int currentBlockRotation;

bool isGameOver = false;

RenderTexture2D fieldRender;

enum ColorsT
{
	BLOCK_EMPTY,
	BLOCK_I,
	BLOCK_J,
	BLOCK_L,
	BLOCK_O,
	BLOCK_S,
	BLOCK_T,
	BLOCK_Z
};

/** Colors for the Tetromino */
const Color tColors[8] =
{
	{0, 0, 0, 255},
	{0, 255, 255, 255},
	{0, 0, 255, 255},
	{255, 127, 0, 255},
	{255, 255, 0, 255},
	{0, 255, 0, 255},
	{128, 0, 128, 255},
	{255, 0, 0, 255}
};

/** Function to Render the Background of the Field Area
 *  @param fPos Render Position for the Field */
void drawFieldBackground(Vector2 fPos)
{
	DrawRectangle(fPos.x - 5, fPos.y - 5, FIELD_WIDTH * ELEMENT_SIZE + 10, FIELD_HEIGHT * ELEMENT_SIZE + 10, DARKGRAY);
	DrawRectangleLines(fPos.x - 5, fPos.y - 5, FIELD_WIDTH * ELEMENT_SIZE + 10, FIELD_HEIGHT * ELEMENT_SIZE + 10, WHITE);
	DrawRectangleLines(fPos.x - 1, fPos.y - 1, FIELD_WIDTH * ELEMENT_SIZE + 2, FIELD_HEIGHT * ELEMENT_SIZE + 2, WHITE);
}

/** Draw one Field of a Tetromino
 *  @param posX X-Position of the Tetromino
 *  @param posY Y-Position of the Tetromino
 *  @param color Color of the Tetromino */
void drawElement(int posX, int posY, Color color)
{
	DrawRectangle(posX, posY, ELEMENT_SIZE, ELEMENT_SIZE, color);
	DrawRectangleLines(posX, posY, ELEMENT_SIZE, ELEMENT_SIZE, WHITE);
}

void updateCurrentBlock(int rotation, int type)
{
	switch (type)
	{
		case BLOCK_I:
			memcpy(currentBlock[0], &blockI[rotation][0], 4 * sizeof(bool));
			memcpy(currentBlock[1], &blockI[rotation][4], 4 * sizeof(bool));
			memcpy(currentBlock[2], &blockI[rotation][8], 4 * sizeof(bool));
			memcpy(currentBlock[3], &blockI[rotation][12], 4 * sizeof(bool));
			break;
		case BLOCK_J:
			memcpy(currentBlock[0], &blockJ[rotation][0], 4 * sizeof(bool));
			memcpy(currentBlock[1], &blockJ[rotation][4], 4 * sizeof(bool));
			memcpy(currentBlock[2], &blockJ[rotation][8], 4 * sizeof(bool));
			memcpy(currentBlock[3], &blockJ[rotation][12], 4 * sizeof(bool));
			break;
		case BLOCK_L:
			memcpy(currentBlock[0], &blockL[rotation][0], 4 * sizeof(bool));
			memcpy(currentBlock[1], &blockL[rotation][4], 4 * sizeof(bool));
			memcpy(currentBlock[2], &blockL[rotation][8], 4 * sizeof(bool));
			memcpy(currentBlock[3], &blockL[rotation][12], 4 * sizeof(bool));
			break;
		case BLOCK_O:
			memcpy(currentBlock[0], &blockO[rotation][0], 4 * sizeof(bool));
			memcpy(currentBlock[1], &blockO[rotation][4], 4 * sizeof(bool));
			memcpy(currentBlock[2], &blockO[rotation][8], 4 * sizeof(bool));
			memcpy(currentBlock[3], &blockO[rotation][12], 4 * sizeof(bool));
			break;
		case BLOCK_S:
			memcpy(currentBlock[0], &blockS[rotation][0], 4 * sizeof(bool));
			memcpy(currentBlock[1], &blockS[rotation][4], 4 * sizeof(bool));
			memcpy(currentBlock[2], &blockS[rotation][8], 4 * sizeof(bool));
			memcpy(currentBlock[3], &blockS[rotation][12], 4 * sizeof(bool));
			break;
		case BLOCK_T:
			memcpy(currentBlock[0], &blockT[rotation][0], 4 * sizeof(bool));
			memcpy(currentBlock[1], &blockT[rotation][4], 4 * sizeof(bool));
			memcpy(currentBlock[2], &blockT[rotation][8], 4 * sizeof(bool));
			memcpy(currentBlock[3], &blockT[rotation][12], 4 * sizeof(bool));
			break;
		case BLOCK_Z:
			memcpy(currentBlock[0], &blockZ[rotation][0], 4 * sizeof(bool));
			memcpy(currentBlock[1], &blockZ[rotation][4], 4 * sizeof(bool));
			memcpy(currentBlock[2], &blockZ[rotation][8], 4 * sizeof(bool));
			memcpy(currentBlock[3], &blockZ[rotation][12], 4 * sizeof(bool));
			break;
	}
}

/** @param currentBlock Current Tetromino
 *  @param field Field Array
 *  @param pPos Tetromino Position to Move
 *  @returns TRUE if no Collisions accoures */
bool DoesPieceFit(Vector2 pPos)
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (!currentBlock[i][j])
				continue;

			// X,Y Position in Field
			int posX = pPos.x + j;
			int posY = pPos.y + i;

			if (posX < 0 || posX >= FIELD_WIDTH || posY >= FIELD_HEIGHT || fieldContent[posX][posY] != 0)
				return false;
		}
	}
	return true;
}

void clearLines()
{
	for (int i = FIELD_HEIGHT - 1; i >= 0; i--) {
		bool isFull = true;
		TraceLog(LOG_TRACE, "Row %d isFull = %d", i, isFull);
		for (int j = 0; j < FIELD_WIDTH; j++) {
			if (fieldContent[j][i] == 0) {
				isFull = false;
				TraceLog(LOG_TRACE, "Row %d Colum %d isFull = %d", j, i, isFull);
				break;
			}
		}

		if (isFull) {
			// Shift all lines above this line down
			for (int k = i; k > 0; k--)
			{
				for (int j = 0; j < FIELD_WIDTH; j++)
				{
					fieldContent[j][k] = fieldContent[j][k - 1];
				}
			}

			// Clear the top line
			for (int j = 0; j < FIELD_WIDTH; j++)
			{
				fieldContent[j][0] = 0;
			}
		}
	}
}

void spawnTetromino() {
	currentBlockPos = (Vector2) {5, 0};
	currentBlockType = GetRandomValue(BLOCK_I, BLOCK_Z);
	currentBlockRotation = GetRandomValue(0, 3);

	if (!DoesPieceFit(currentBlockPos)) {
		isGameOver = true;
	}
}

void appMain()
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(screenWidth, screenHeight, TextFormat("Raytris v%s", VERSION));

	SetExitKey(0);
	SetTraceLogLevel(LOG_TRACE);
	SetRandomSeed(time(NULL));
	SetTargetFPS(60);
	
	fieldRender = LoadRenderTexture(screenWidth, screenHeight);

	spawnTetromino();
	
	Vector2 fieldPos = {280, 60};

	// Init fieldContent to 0
	for (int w = 0; w < FIELD_WIDTH; w++)
		for (int h = 0; h < FIELD_HEIGHT; h++)
			fieldContent[w][h] = 0;

	updateCurrentBlock(currentBlockRotation, currentBlockType);

	double lastTick;

	int LinesCleared = 0;
	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!isGameOver)
	{
		// Update
		//----------------------------------------------------------------------------------
		if (IsKeyPressed(KEY_E) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
		{ // CLOCKWISE ROTATION
			currentBlockRotation++;
			if (currentBlockRotation > 3)
				currentBlockRotation = 0;
			updateCurrentBlock( currentBlockRotation, currentBlockType);
			if (!DoesPieceFit(currentBlockPos)) {
				currentBlockRotation--;
				updateCurrentBlock(currentBlockRotation, currentBlockType);
			}
			if (currentBlockRotation < 0)
				currentBlockRotation = 3;
		}
		else if (IsKeyPressed(KEY_Q) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{ // COUNTERCLOCKWISE ROTATION
			currentBlockRotation--;
			if (currentBlockRotation < 0)
				currentBlockRotation = 3;
			updateCurrentBlock(currentBlockRotation, currentBlockType);
			if (!DoesPieceFit(currentBlockPos)) {
				currentBlockRotation++;
				updateCurrentBlock(currentBlockRotation, currentBlockType);
			}
			if (currentBlockRotation > 3)
				currentBlockRotation = 0;
		}
		else if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT))
		{ // LEFT MOVEMENT
			currentBlockPos.x--;
			if (!DoesPieceFit(currentBlockPos)) {
				currentBlockPos.x++;
			}
		}
		else if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT))
		{ // RIGHT MOVEMENT
			currentBlockPos.x++;
			if (!DoesPieceFit(currentBlockPos))
				currentBlockPos.x--;
		}
		else if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN))
		{ // DOWN MOVEMENT
			currentBlockPos.y++;
			if (!DoesPieceFit(currentBlockPos))
				currentBlockPos.y--;
		}

		updateCurrentBlock(currentBlockRotation, currentBlockType);

		if (GetTime() >= lastTick + 2.0f) {
			if (DoesPieceFit((Vector2) {currentBlockPos.x, currentBlockPos.y + 1})){
				currentBlockPos.y++;
			} 
			else {
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++) {
						if (!currentBlock[i][j])
							continue;

						// X,Y Position in Field
						int posX = currentBlockPos.x + j;
						int posY = currentBlockPos.y + i;

						if (posX < 0 || posX >= FIELD_WIDTH || posY >= FIELD_HEIGHT || fieldContent[posX][posY] != 0)
							break;

						fieldContent[posX][posY] = currentBlockType;
					}
				}
				clearLines();
				spawnTetromino();
			}
			lastTick = GetTime();
			if (!isGameOver)
				updateCurrentBlock(currentBlockRotation, currentBlockType);
		}
		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginTextureMode(fieldRender);
			drawFieldBackground(fieldPos);

			for (int w = 0; w < FIELD_WIDTH; w++) {
				for (int h = 0; h < FIELD_HEIGHT; h++) {
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
							tColors[currentBlockType]);
					}
				}
			}
		EndTextureMode();

		BeginDrawing();
			ClearBackground(BLACK);
			DrawRectangleGradientV(0, 0, GetScreenWidth(), GetScreenHeight(),
			DARKBLUE, BLACK);

			DrawTexturePro(fieldRender.texture, (Rectangle) {0, 0, screenWidth, -screenHeight}, (Rectangle) {GetScreenWidth()/2, GetScreenHeight()/2, GetScreenWidth(), GetScreenHeight()}, 
			(Vector2) {GetScreenWidth()/2, GetScreenHeight()/2}, 0.0f, (Color) {255, 255, 255, 255});

			DrawText(TextFormat("Rows Cleared: %d", LinesCleared), 10, 10, 18, RAYWHITE);
		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	UnloadRenderTexture(fieldRender);
	CloseWindow();
}