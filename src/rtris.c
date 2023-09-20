#include "defines.h"
#include "rtris.h"
#include "block.h"

#include "raylib.h"

#include <stdio.h>
#include <time.h>
#include <string.h>

#define FIELD_WIDTH 12
#define FIELD_HEIGHT 18

const int screenWidth = WINDOW_WIDTH;
const int screenHeight = WINDOW_HEIGHT;

const int ELEMENT_SIZE = 20;

const Vector2 fieldPos = {280, 60};
char fieldContent[FIELD_WIDTH][FIELD_HEIGHT] = {0};

Vector2 currentBlockPos;
bool currentBlock[4][4];
int currentBlockType;
int currentBlockRotation;

bool nextBlock[4][4];
int nextBlockType;
int nextBlockRotation;

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

int difficulty[15] =
{
	48, 43, 38, 33, 28,
	23, 18, 13,  8,  6,
	 5,  4,  3,  2,  1
};

/** Function to Render the Background of the Field Area
 *  @param fPos Render Position for the Field */
void drawFieldBackground(Vector2 fPos)
{
	DrawRectangle(fPos.x - 5, fPos.y - 5, FIELD_WIDTH * ELEMENT_SIZE + 10, FIELD_HEIGHT * ELEMENT_SIZE + 10, DARKGRAY);
	DrawRectangleLines(fPos.x - 5, fPos.y - 5, FIELD_WIDTH * ELEMENT_SIZE + 10, FIELD_HEIGHT * ELEMENT_SIZE + 10, WHITE);
	DrawRectangleLines(fPos.x - 1, fPos.y - 1, FIELD_WIDTH * ELEMENT_SIZE + 2, FIELD_HEIGHT * ELEMENT_SIZE + 2, WHITE);
}

void drawPreviewBackground()
{
	DrawRectangle(50 - 5, 50 - 5, 4 * ELEMENT_SIZE + 10, 4 * ELEMENT_SIZE + 10, DARKGRAY);
	DrawRectangleLines(50 - 5, 50 - 5, 4 * ELEMENT_SIZE + 10, 4 * ELEMENT_SIZE + 10, WHITE);
	DrawRectangleLines(50 - 1, 50 - 1, 4 * ELEMENT_SIZE + 2, 4 * ELEMENT_SIZE + 2, WHITE);
}

/** Draw one Field of a Tetromino
 *  @param posX X-Position of the Tetromino
 *  @param posY Y-Position of the Tetromino
 *  @param color Color of the Tetromino */
void drawElement(int posX, int posY, Color color)
{
	DrawRectangleGradientEx((Rectangle) { posX, posY, ELEMENT_SIZE, ELEMENT_SIZE }, color, (Color) {color.r * 0.5f, color.g * 0.5f, color.b * 0.5f, 255}, BLACK, (Color) {color.r * 0.5f, color.g * 0.5f, color.b * 0.5f, 255});
	DrawRectangleLines(posX, posY, ELEMENT_SIZE, ELEMENT_SIZE, BLACK);
}

void updateBlock(bool current, int rotation, int type)
{
	switch (type)
	{
		case BLOCK_I:
			if (current) {
				memcpy(currentBlock[0], &blockI[rotation][0], 4 * sizeof(bool));
				memcpy(currentBlock[1], &blockI[rotation][4], 4 * sizeof(bool));
				memcpy(currentBlock[2], &blockI[rotation][8], 4 * sizeof(bool));
				memcpy(currentBlock[3], &blockI[rotation][12], 4 * sizeof(bool));
			} else {
				memcpy(nextBlock[0], &blockI[rotation][0], 4 * sizeof(bool));
				memcpy(nextBlock[1], &blockI[rotation][4], 4 * sizeof(bool));
				memcpy(nextBlock[2], &blockI[rotation][8], 4 * sizeof(bool));
				memcpy(nextBlock[3], &blockI[rotation][12], 4 * sizeof(bool));
			}
			break;
		case BLOCK_J:
			if (current) {
				memcpy(currentBlock[0], &blockJ[rotation][0], 4 * sizeof(bool));
				memcpy(currentBlock[1], &blockJ[rotation][4], 4 * sizeof(bool));
				memcpy(currentBlock[2], &blockJ[rotation][8], 4 * sizeof(bool));
				memcpy(currentBlock[3], &blockJ[rotation][12], 4 * sizeof(bool));
			} else {
				memcpy(nextBlock[0], &blockJ[rotation][0], 4 * sizeof(bool));
				memcpy(nextBlock[1], &blockJ[rotation][4], 4 * sizeof(bool));
				memcpy(nextBlock[2], &blockJ[rotation][8], 4 * sizeof(bool));
				memcpy(nextBlock[3], &blockJ[rotation][12], 4 * sizeof(bool));
			}
			break;
		case BLOCK_L:
			if (current) {
				memcpy(currentBlock[0], &blockL[rotation][0], 4 * sizeof(bool));
				memcpy(currentBlock[1], &blockL[rotation][4], 4 * sizeof(bool));
				memcpy(currentBlock[2], &blockL[rotation][8], 4 * sizeof(bool));
				memcpy(currentBlock[3], &blockL[rotation][12], 4 * sizeof(bool));
			} else {
				memcpy(nextBlock[0], &blockL[rotation][0], 4 * sizeof(bool));
				memcpy(nextBlock[1], &blockL[rotation][4], 4 * sizeof(bool));
				memcpy(nextBlock[2], &blockL[rotation][8], 4 * sizeof(bool));
				memcpy(nextBlock[3], &blockL[rotation][12], 4 * sizeof(bool));
			}
			break;
		case BLOCK_O:
			if (current) {
				memcpy(currentBlock[0], &blockO[rotation][0], 4 * sizeof(bool));
				memcpy(currentBlock[1], &blockO[rotation][4], 4 * sizeof(bool));
				memcpy(currentBlock[2], &blockO[rotation][8], 4 * sizeof(bool));
				memcpy(currentBlock[3], &blockO[rotation][12], 4 * sizeof(bool));
			} else {
				memcpy(nextBlock[0], &blockO[rotation][0], 4 * sizeof(bool));
				memcpy(nextBlock[1], &blockO[rotation][4], 4 * sizeof(bool));
				memcpy(nextBlock[2], &blockO[rotation][8], 4 * sizeof(bool));
				memcpy(nextBlock[3], &blockO[rotation][12], 4 * sizeof(bool));
			}
			break;
		case BLOCK_S:
			if (current) {
				memcpy(currentBlock[0], &blockS[rotation][0], 4 * sizeof(bool));
				memcpy(currentBlock[1], &blockS[rotation][4], 4 * sizeof(bool));
				memcpy(currentBlock[2], &blockS[rotation][8], 4 * sizeof(bool));
				memcpy(currentBlock[3], &blockS[rotation][12], 4 * sizeof(bool));
			} else {
				memcpy(nextBlock[0], &blockS[rotation][0], 4 * sizeof(bool));
				memcpy(nextBlock[1], &blockS[rotation][4], 4 * sizeof(bool));
				memcpy(nextBlock[2], &blockS[rotation][8], 4 * sizeof(bool));
				memcpy(nextBlock[3], &blockS[rotation][12], 4 * sizeof(bool));
			}
			break;
		case BLOCK_T:
			if (current) {
				memcpy(currentBlock[0], &blockT[rotation][0], 4 * sizeof(bool));
				memcpy(currentBlock[1], &blockT[rotation][4], 4 * sizeof(bool));
				memcpy(currentBlock[2], &blockT[rotation][8], 4 * sizeof(bool));
				memcpy(currentBlock[3], &blockT[rotation][12], 4 * sizeof(bool));
			} else {
				memcpy(nextBlock[0], &blockT[rotation][0], 4 * sizeof(bool));
				memcpy(nextBlock[1], &blockT[rotation][4], 4 * sizeof(bool));
				memcpy(nextBlock[2], &blockT[rotation][8], 4 * sizeof(bool));
				memcpy(nextBlock[3], &blockT[rotation][12], 4 * sizeof(bool));
			}
			break;
		case BLOCK_Z:
			if (current) {
				memcpy(currentBlock[0], &blockZ[rotation][0], 4 * sizeof(bool));
				memcpy(currentBlock[1], &blockZ[rotation][4], 4 * sizeof(bool));
				memcpy(currentBlock[2], &blockZ[rotation][8], 4 * sizeof(bool));
				memcpy(currentBlock[3], &blockZ[rotation][12], 4 * sizeof(bool));
			} else {
				memcpy(nextBlock[0], &blockZ[rotation][0], 4 * sizeof(bool));
				memcpy(nextBlock[1], &blockZ[rotation][4], 4 * sizeof(bool));
				memcpy(nextBlock[2], &blockZ[rotation][8], 4 * sizeof(bool));
				memcpy(nextBlock[3], &blockZ[rotation][12], 4 * sizeof(bool));
			}
			break;
	}
}

/** Checks if the Next Position is Valid
 *  @param currentBlock Current Tetromino
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
		for (int j = 0; j < FIELD_WIDTH; j++) {
			if (fieldContent[j][i] == 0) {
				isFull = false;
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

/** Spawns new Tetromino
 * 
*/
void spawnTetromino() {

	int newBlock = GetRandomValue(BLOCK_I, BLOCK_Z);
	while (newBlock == currentBlockType)
		newBlock = GetRandomValue(BLOCK_I, BLOCK_Z);
	
	currentBlockPos = (Vector2) {5, 0};
	currentBlockType = nextBlockType;
	currentBlockRotation = GetRandomValue(0, 3);

	nextBlockType = newBlock;
	nextBlockRotation = GetRandomValue(0, 3);

	updateBlock(true, currentBlockRotation, currentBlockType);
	updateBlock(false, nextBlockRotation, nextBlockType);
	
	if (!DoesPieceFit(currentBlockPos)) {
		isGameOver = true;
	}
}

void appMain()
{
	SetTraceLogLevel(LOG_TRACE);
	SetRandomSeed((unsigned int)time(NULL));

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(screenWidth, screenHeight, TextFormat("Raytris v%s", VERSION));
	SetWindowMinSize(screenWidth, screenHeight);

	InitAudioDevice();

	SetExitKey(0);
	SetTargetFPS(60);

	TraceLog(LOG_TRACE, "RNG-Seed: %u", (unsigned int)time(NULL));
	
	fieldRender = LoadRenderTexture(screenWidth, screenHeight);

	Music soundtrack = LoadMusicStream("res/soundtrack.mp3");
	PlayMusicStream(soundtrack);

	SetAudioStreamVolume(soundtrack.stream, 0.1f);

	nextBlockType = GetRandomValue(1, 7);
	spawnTetromino();

	TraceLog(LOG_DEBUG, "CurrentBlockType[%d]", currentBlockType);
	TraceLog(LOG_DEBUG, "NextBlockType[%d]", nextBlockType);

	// Init fieldContent to 0
	for (int w = 0; w < FIELD_WIDTH; w++)
		for (int h = 0; h < FIELD_HEIGHT; h++)
			fieldContent[w][h] = 0;

	updateBlock(true, currentBlockRotation, currentBlockType);

	double lastTick;

	int LinesCleared = 0;
	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!isGameOver && !WindowShouldClose())
	{
		// Update
		//----------------------------------------------------------------------------------
		// Update Music Stream
		UpdateMusicStream(soundtrack);
		if (GetMusicTimePlayed(soundtrack) == GetMusicTimeLength(soundtrack))
			PlayMusicStream(soundtrack);
		
		if (IsKeyPressed(KEY_E) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
		{ // CLOCKWISE ROTATION
			currentBlockRotation++;
			if (currentBlockRotation > 3)
				currentBlockRotation = 0;
			updateBlock(true, currentBlockRotation, currentBlockType);
			if (!DoesPieceFit(currentBlockPos)) {
				currentBlockRotation--;
				updateBlock(true, currentBlockRotation, currentBlockType);
			}
			if (currentBlockRotation < 0)
				currentBlockRotation = 3;
		}
		else if (IsKeyPressed(KEY_Q) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{ // COUNTERCLOCKWISE ROTATION
			currentBlockRotation--;
			if (currentBlockRotation < 0)
				currentBlockRotation = 3;
			updateBlock(true, currentBlockRotation, currentBlockType);
			if (!DoesPieceFit(currentBlockPos)) {
				currentBlockRotation++;
				updateBlock(true, currentBlockRotation, currentBlockType);
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

		updateBlock(true, currentBlockRotation, currentBlockType);

		if (GetTime() >= lastTick + 0.48f) {
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
		}
		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginTextureMode(fieldRender);
			drawFieldBackground(fieldPos);
			drawPreviewBackground();

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

			// Draw Next Block
			for (int w = 0; w < 4; w++) {
				for (int h = 0; h < 4; h++) {
					if (nextBlock[w][h]) {
						drawElement(
							50 + h * ELEMENT_SIZE,
							50 + w * ELEMENT_SIZE,
							tColors[nextBlockType]);
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
			DrawText(TextFormat("FPS: %d", GetFPS()), GetScreenWidth() - 75, 10, 18, RAYWHITE);

			DrawRectanglePro((Rectangle) {0, GetScreenHeight() - 1, GetScreenWidth(), 1}, (Vector2) {0, 0}, 0.0f, DARKGRAY);
			DrawRectanglePro((Rectangle) {0, GetScreenHeight() - 1, (GetMusicTimePlayed(soundtrack)/GetMusicTimeLength(soundtrack)) * GetScreenWidth(), 1}, (Vector2) {0, 0}, 0.0f, RED);
		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	UnloadRenderTexture(fieldRender);
	UnloadMusicStream(soundtrack);
	CloseAudioDevice();
	CloseWindow();
}