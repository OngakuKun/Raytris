#include "defines.h"
#include "rtris.h"
#include "block.h"

#include "raylib.h"

#include <stdio.h>
#include <time.h>
#include <string.h>

#define FIELD_WIDTH 12
#define FIELD_HEIGHT 18
#define FIELD_POSX 280
#define FIELD_POSY 60

const int screenWidth = WINDOW_WIDTH;
const int screenHeight = WINDOW_HEIGHT;

const int ELEMENT_SIZE = 20;

char fieldContent[FIELD_WIDTH][FIELD_HEIGHT] = {0};

Vector2 currentBlockPos;
bool currentBlock[4][4];
int currentBlockType;
int currentBlockRotation;

bool nextBlock[4][4];
int nextBlockType;
int nextBlockRotation;

bool isGameOver = false;
int LinesCleared;

const Rectangle fieldSourceRec = {FIELD_POSX, FIELD_POSY, FIELD_WIDTH * ELEMENT_SIZE, FIELD_HEIGHT * ELEMENT_SIZE};
const Rectangle previewSourceRec = {0, 0, 840, 480};

RenderTexture2D fieldRender;
RenderTexture2D previewRender;

Texture2D previewTexture;

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
	Rectangle backgroundRec = {
		GetScreenWidth() / 2 + 7 * ELEMENT_SIZE, GetScreenHeight()/2 - 9 * ELEMENT_SIZE, 
		6 * ELEMENT_SIZE, 6 * ELEMENT_SIZE
	};

	DrawRectangle(backgroundRec.x - 5, backgroundRec.y - 5,
	backgroundRec.width + 10, backgroundRec.height + 10, DARKGRAY);
	DrawRectangleLines(backgroundRec.x - 5,  backgroundRec.y - 5,
	backgroundRec.width + 10, backgroundRec.height + 10, WHITE);
	DrawRectangleLines(backgroundRec.x - 1,  backgroundRec.y - 1,
	backgroundRec.width + 2, backgroundRec.height + 2, WHITE);
}

/** Draw one Field of a Tetromino
 *  @param posX X-Position of the Tetromino
 *  @param posY Y-Position of the Tetromino
 *  @param color Color of the Tetromino */
void drawElement(int posX, int posY, Color color)
{
	DrawRectangleGradientEx((Rectangle) { posX, posY, ELEMENT_SIZE, ELEMENT_SIZE }, color, (Color) {color.r * 0.5f, color.g * 0.5f, color.b * 0.5f, 255}, BLACK, (Color) {color.r * 0.5f, color.g * 0.5f, color.b * 0.5f, 255});
	DrawRectangleLines(posX, posY, ELEMENT_SIZE, ELEMENT_SIZE, DARKGRAY);
}

void renderPreviewTexture()
{
	previewRender = LoadRenderTexture(previewSourceRec.width, previewSourceRec.height);

	BeginTextureMode(previewRender);
		Vector2 drawIndex = {0,0};
		TraceLog(LOG_INFO, "Drawing BLOCK I Preview Rotation 0");
		drawElement(drawIndex.x + 40, drawIndex.y + 20, tColors[BLOCK_I]);
		drawElement(drawIndex.x + 40, drawIndex.y + 40, tColors[BLOCK_I]);
		drawElement(drawIndex.x + 40, drawIndex.y + 60, tColors[BLOCK_I]);
		drawElement(drawIndex.x + 40, drawIndex.y + 80, tColors[BLOCK_I]);

		drawIndex = (Vector2) {0, 6 * ELEMENT_SIZE};
		TraceLog(LOG_INFO, "Drawing BLOCK I Preview Rotation 1");
		drawElement(drawIndex.x + 20, drawIndex.y + 40, tColors[BLOCK_I]);
		drawElement(drawIndex.x + 40, drawIndex.y + 40, tColors[BLOCK_I]);
		drawElement(drawIndex.x + 60, drawIndex.y + 40, tColors[BLOCK_I]);
		drawElement(drawIndex.x + 80, drawIndex.y + 40, tColors[BLOCK_I]);

		drawIndex = (Vector2) {0, 12 * ELEMENT_SIZE};
		TraceLog(LOG_INFO, "Drawing BLOCK I Preview Rotation 2");
		drawElement(drawIndex.x + 60, drawIndex.y + 20, tColors[BLOCK_I]);
		drawElement(drawIndex.x + 60, drawIndex.y + 40, tColors[BLOCK_I]);
		drawElement(drawIndex.x + 60, drawIndex.y + 60, tColors[BLOCK_I]);
		drawElement(drawIndex.x + 60, drawIndex.y + 80, tColors[BLOCK_I]);

		drawIndex = (Vector2) {0, 18 * ELEMENT_SIZE};
		TraceLog(LOG_INFO, "Drawing BLOCK I Preview Rotation 3");
		drawElement(drawIndex.x + 20, drawIndex.y + 60, tColors[BLOCK_I]);
		drawElement(drawIndex.x + 40, drawIndex.y + 60, tColors[BLOCK_I]);
		drawElement(drawIndex.x + 60, drawIndex.y + 60, tColors[BLOCK_I]);
		drawElement(drawIndex.x + 80, drawIndex.y + 60, tColors[BLOCK_I]);

		//////////////////////////////////////////////////////////////////////////////////////////////

		drawIndex = (Vector2) {6.5f * ELEMENT_SIZE, 0.5f * ELEMENT_SIZE};
		TraceLog(LOG_INFO, "Drawing BLOCK J Preview Rotation 0");
		drawElement(drawIndex.x + 2.5f * ELEMENT_SIZE, drawIndex.y + 1 * ELEMENT_SIZE, tColors[BLOCK_J]);
		drawElement(drawIndex.x + 2.5f * ELEMENT_SIZE, drawIndex.y + 2 * ELEMENT_SIZE, tColors[BLOCK_J]);
		drawElement(drawIndex.x + 2.5f * ELEMENT_SIZE, drawIndex.y + 3 * ELEMENT_SIZE, tColors[BLOCK_J]);
		drawElement(drawIndex.x + 1.5f * ELEMENT_SIZE, drawIndex.y + 3 * ELEMENT_SIZE, tColors[BLOCK_J]);

		drawIndex = (Vector2) {6.5f * ELEMENT_SIZE, 6.5f * ELEMENT_SIZE};
		TraceLog(LOG_INFO, "Drawing BLOCK J Preview Rotation 1");
		drawElement(drawIndex.x + 1 * ELEMENT_SIZE, drawIndex.y + 1.5f * ELEMENT_SIZE, tColors[BLOCK_J]);
		drawElement(drawIndex.x + 1 * ELEMENT_SIZE, drawIndex.y + 2.5f * ELEMENT_SIZE, tColors[BLOCK_J]);
		drawElement(drawIndex.x + 2 * ELEMENT_SIZE, drawIndex.y + 2.5f * ELEMENT_SIZE, tColors[BLOCK_J]);
		drawElement(drawIndex.x + 3 * ELEMENT_SIZE, drawIndex.y + 2.5f * ELEMENT_SIZE, tColors[BLOCK_J]);

		drawIndex = (Vector2) {6.5f * ELEMENT_SIZE, 12.5f * ELEMENT_SIZE};
		TraceLog(LOG_INFO, "Drawing BLOCK J Preview Rotation 2");
		drawElement(drawIndex.x + 1.5f * ELEMENT_SIZE, drawIndex.y + 1 * ELEMENT_SIZE, tColors[BLOCK_J]);
		drawElement(drawIndex.x + 1.5f * ELEMENT_SIZE, drawIndex.y + 2 * ELEMENT_SIZE, tColors[BLOCK_J]);
		drawElement(drawIndex.x + 1.5f * ELEMENT_SIZE, drawIndex.y + 3 * ELEMENT_SIZE, tColors[BLOCK_J]);
		drawElement(drawIndex.x + 2.5f * ELEMENT_SIZE, drawIndex.y + 1 * ELEMENT_SIZE, tColors[BLOCK_J]);

		drawIndex = (Vector2) {6.5f * ELEMENT_SIZE, 18.5f * ELEMENT_SIZE};
		TraceLog(LOG_INFO, "Drawing BLOCK J Preview Rotation 3");
		drawElement(drawIndex.x + 3 * ELEMENT_SIZE, drawIndex.y + 2.5f * ELEMENT_SIZE, tColors[BLOCK_J]);
		drawElement(drawIndex.x + 1 * ELEMENT_SIZE, drawIndex.y + 1.5f * ELEMENT_SIZE, tColors[BLOCK_J]);
		drawElement(drawIndex.x + 2 * ELEMENT_SIZE, drawIndex.y + 1.5f * ELEMENT_SIZE, tColors[BLOCK_J]);
		drawElement(drawIndex.x + 3 * ELEMENT_SIZE, drawIndex.y + 1.5f * ELEMENT_SIZE, tColors[BLOCK_J]);

		//////////////////////////////////////////////////////////////////////////////////////////////

		drawIndex = (Vector2) {12.5f * ELEMENT_SIZE, 0.5f * ELEMENT_SIZE};
		TraceLog(LOG_INFO, "Drawing BLOCK L Preview Rotation 0");
		drawElement(drawIndex.x + 1.5f * ELEMENT_SIZE, drawIndex.y + 1 * ELEMENT_SIZE, tColors[BLOCK_L]);
		drawElement(drawIndex.x + 1.5f * ELEMENT_SIZE, drawIndex.y + 2 * ELEMENT_SIZE, tColors[BLOCK_L]);
		drawElement(drawIndex.x + 1.5f * ELEMENT_SIZE, drawIndex.y + 3 * ELEMENT_SIZE, tColors[BLOCK_L]);
		drawElement(drawIndex.x + 2.5f * ELEMENT_SIZE, drawIndex.y + 3 * ELEMENT_SIZE, tColors[BLOCK_L]);

		drawIndex = (Vector2) {12.5f * ELEMENT_SIZE, 6.5f * ELEMENT_SIZE};
		TraceLog(LOG_INFO, "Drawing BLOCK L Preview Rotation 1");
		drawElement(drawIndex.x + 1 * ELEMENT_SIZE, drawIndex.y + 2.5f * ELEMENT_SIZE, tColors[BLOCK_L]);
		drawElement(drawIndex.x + 1 * ELEMENT_SIZE, drawIndex.y + 1.5f * ELEMENT_SIZE, tColors[BLOCK_L]);
		drawElement(drawIndex.x + 2 * ELEMENT_SIZE, drawIndex.y + 1.5f * ELEMENT_SIZE, tColors[BLOCK_L]);
		drawElement(drawIndex.x + 3 * ELEMENT_SIZE, drawIndex.y + 1.5f * ELEMENT_SIZE, tColors[BLOCK_L]);

		drawIndex = (Vector2) {12.5f * ELEMENT_SIZE, 12.5f * ELEMENT_SIZE};
		TraceLog(LOG_INFO, "Drawing BLOCK L Preview Rotation 2");
		drawElement(drawIndex.x + 2.5f * ELEMENT_SIZE, drawIndex.y + 1 * ELEMENT_SIZE, tColors[BLOCK_L]);
		drawElement(drawIndex.x + 2.5f * ELEMENT_SIZE, drawIndex.y + 2 * ELEMENT_SIZE, tColors[BLOCK_L]);
		drawElement(drawIndex.x + 2.5f * ELEMENT_SIZE, drawIndex.y + 3 * ELEMENT_SIZE, tColors[BLOCK_L]);
		drawElement(drawIndex.x + 1.5f * ELEMENT_SIZE, drawIndex.y + 1 * ELEMENT_SIZE, tColors[BLOCK_L]);

		drawIndex = (Vector2) {12.5f * ELEMENT_SIZE, 18.5f * ELEMENT_SIZE};
		TraceLog(LOG_INFO, "Drawing BLOCK L Preview Rotation 3");
		drawElement(drawIndex.x + 3 * ELEMENT_SIZE, drawIndex.y + 1.5f * ELEMENT_SIZE, tColors[BLOCK_L]);
		drawElement(drawIndex.x + 1 * ELEMENT_SIZE, drawIndex.y + 2.5f * ELEMENT_SIZE, tColors[BLOCK_L]);
		drawElement(drawIndex.x + 2 * ELEMENT_SIZE, drawIndex.y + 2.5f * ELEMENT_SIZE, tColors[BLOCK_L]);
		drawElement(drawIndex.x + 3 * ELEMENT_SIZE, drawIndex.y + 2.5f * ELEMENT_SIZE, tColors[BLOCK_L]);

		//////////////////////////////////////////////////////////////////////////////////////////////

		drawIndex = (Vector2) {18 * ELEMENT_SIZE, 0};
		TraceLog(LOG_INFO, "Drawing BLOCK O Preview Rotation 0");
		drawElement(drawIndex.x + 2 * ELEMENT_SIZE, drawIndex.y + 2 * ELEMENT_SIZE, tColors[BLOCK_O]);
		drawElement(drawIndex.x + 2 * ELEMENT_SIZE, drawIndex.y + 3 * ELEMENT_SIZE, tColors[BLOCK_O]);
		drawElement(drawIndex.x + 3 * ELEMENT_SIZE, drawIndex.y + 2 * ELEMENT_SIZE, tColors[BLOCK_O]);
		drawElement(drawIndex.x + 3 * ELEMENT_SIZE, drawIndex.y + 3 * ELEMENT_SIZE, tColors[BLOCK_O]);

		drawIndex = (Vector2) {18 * ELEMENT_SIZE, 6 * ELEMENT_SIZE};
		TraceLog(LOG_INFO, "Drawing BLOCK O Preview Rotation 1");
		drawElement(drawIndex.x + 2 * ELEMENT_SIZE, drawIndex.y + 2 * ELEMENT_SIZE, tColors[BLOCK_O]);
		drawElement(drawIndex.x + 2 * ELEMENT_SIZE, drawIndex.y + 3 * ELEMENT_SIZE, tColors[BLOCK_O]);
		drawElement(drawIndex.x + 3 * ELEMENT_SIZE, drawIndex.y + 2 * ELEMENT_SIZE, tColors[BLOCK_O]);
		drawElement(drawIndex.x + 3 * ELEMENT_SIZE, drawIndex.y + 3 * ELEMENT_SIZE, tColors[BLOCK_O]);

		drawIndex = (Vector2) {18 * ELEMENT_SIZE, 12 * ELEMENT_SIZE};
		TraceLog(LOG_INFO, "Drawing BLOCK O Preview Rotation 2");
		drawElement(drawIndex.x + 2 * ELEMENT_SIZE, drawIndex.y + 2 * ELEMENT_SIZE, tColors[BLOCK_O]);
		drawElement(drawIndex.x + 2 * ELEMENT_SIZE, drawIndex.y + 3 * ELEMENT_SIZE, tColors[BLOCK_O]);
		drawElement(drawIndex.x + 3 * ELEMENT_SIZE, drawIndex.y + 2 * ELEMENT_SIZE, tColors[BLOCK_O]);
		drawElement(drawIndex.x + 3 * ELEMENT_SIZE, drawIndex.y + 3 * ELEMENT_SIZE, tColors[BLOCK_O]);

		drawIndex = (Vector2) {18 * ELEMENT_SIZE, 18 * ELEMENT_SIZE};
		TraceLog(LOG_INFO, "Drawing BLOCK O Preview Rotation 3");
		drawElement(drawIndex.x + 2 * ELEMENT_SIZE, drawIndex.y + 2 * ELEMENT_SIZE, tColors[BLOCK_O]);
		drawElement(drawIndex.x + 2 * ELEMENT_SIZE, drawIndex.y + 3 * ELEMENT_SIZE, tColors[BLOCK_O]);
		drawElement(drawIndex.x + 3 * ELEMENT_SIZE, drawIndex.y + 2 * ELEMENT_SIZE, tColors[BLOCK_O]);
		drawElement(drawIndex.x + 3 * ELEMENT_SIZE, drawIndex.y + 3 * ELEMENT_SIZE, tColors[BLOCK_O]);

		//////////////////////////////////////////////////////////////////////////////////////////////

		drawIndex = (Vector2) {24.5f * ELEMENT_SIZE, 0.5f * ELEMENT_SIZE};
		TraceLog(LOG_INFO, "Drawing BLOCK S Preview Rotation 0");
		drawElement(drawIndex.x + 2 * ELEMENT_SIZE, drawIndex.y + 1.5f * ELEMENT_SIZE, tColors[BLOCK_S]);
		drawElement(drawIndex.x + 1 * ELEMENT_SIZE, drawIndex.y + 2.5f * ELEMENT_SIZE, tColors[BLOCK_S]);
		drawElement(drawIndex.x + 2 * ELEMENT_SIZE, drawIndex.y + 2.5f * ELEMENT_SIZE, tColors[BLOCK_S]);
		drawElement(drawIndex.x + 3 * ELEMENT_SIZE, drawIndex.y + 1.5f * ELEMENT_SIZE, tColors[BLOCK_S]);

		drawIndex = (Vector2) {24.5f * ELEMENT_SIZE, 6.5f * ELEMENT_SIZE};
		TraceLog(LOG_INFO, "Drawing BLOCK S Preview Rotation 1");
		drawElement(drawIndex.x + 1.5f * ELEMENT_SIZE, drawIndex.y + 1 * ELEMENT_SIZE, tColors[BLOCK_S]);
		drawElement(drawIndex.x + 1.5f * ELEMENT_SIZE, drawIndex.y + 2 * ELEMENT_SIZE, tColors[BLOCK_S]);
		drawElement(drawIndex.x + 2.5f * ELEMENT_SIZE, drawIndex.y + 2 * ELEMENT_SIZE, tColors[BLOCK_S]);
		drawElement(drawIndex.x + 2.5f * ELEMENT_SIZE, drawIndex.y + 3 * ELEMENT_SIZE, tColors[BLOCK_S]);

		drawIndex = (Vector2) {24.5f * ELEMENT_SIZE, 12.5f * ELEMENT_SIZE};
		TraceLog(LOG_INFO, "Drawing BLOCK S Preview Rotation 2");
		drawElement(drawIndex.x + 2 * ELEMENT_SIZE, drawIndex.y + 1.5f * ELEMENT_SIZE, tColors[BLOCK_S]);
		drawElement(drawIndex.x + 1 * ELEMENT_SIZE, drawIndex.y + 2.5f * ELEMENT_SIZE, tColors[BLOCK_S]);
		drawElement(drawIndex.x + 2 * ELEMENT_SIZE, drawIndex.y + 2.5f * ELEMENT_SIZE, tColors[BLOCK_S]);
		drawElement(drawIndex.x + 3 * ELEMENT_SIZE, drawIndex.y + 1.5f * ELEMENT_SIZE, tColors[BLOCK_S]);

		drawIndex = (Vector2) {24.5f * ELEMENT_SIZE, 18.5f * ELEMENT_SIZE};
		TraceLog(LOG_INFO, "Drawing BLOCK S Preview Rotation 3");
		drawElement(drawIndex.x + 1.5f * ELEMENT_SIZE, drawIndex.y + 1 * ELEMENT_SIZE, tColors[BLOCK_S]);
		drawElement(drawIndex.x + 1.5f * ELEMENT_SIZE, drawIndex.y + 2 * ELEMENT_SIZE, tColors[BLOCK_S]);
		drawElement(drawIndex.x + 2.5f * ELEMENT_SIZE, drawIndex.y + 2 * ELEMENT_SIZE, tColors[BLOCK_S]);
		drawElement(drawIndex.x + 2.5f * ELEMENT_SIZE, drawIndex.y + 3 * ELEMENT_SIZE, tColors[BLOCK_S]);

		//////////////////////////////////////////////////////////////////////////////////////////////

		drawIndex = (Vector2) {30 * ELEMENT_SIZE, 0};
		TraceLog(LOG_INFO, "Drawing BLOCK T Preview Rotation 0");
		drawElement(drawIndex.x + 2 * ELEMENT_SIZE, drawIndex.y + 1 * ELEMENT_SIZE, tColors[BLOCK_T]);
		drawElement(drawIndex.x + 2 * ELEMENT_SIZE, drawIndex.y + 2 * ELEMENT_SIZE, tColors[BLOCK_T]);
		drawElement(drawIndex.x + 2 * ELEMENT_SIZE, drawIndex.y + 3 * ELEMENT_SIZE, tColors[BLOCK_T]);
		drawElement(drawIndex.x + 2 * ELEMENT_SIZE, drawIndex.y + 4 * ELEMENT_SIZE, tColors[BLOCK_T]);

		drawIndex = (Vector2) {30 * ELEMENT_SIZE, 6 * ELEMENT_SIZE};
		TraceLog(LOG_INFO, "Drawing BLOCK T Preview Rotation 1");
		drawElement(drawIndex.x + 1 * ELEMENT_SIZE, drawIndex.y + 2 * ELEMENT_SIZE, tColors[BLOCK_T]);
		drawElement(drawIndex.x + 2 * ELEMENT_SIZE, drawIndex.y + 2 * ELEMENT_SIZE, tColors[BLOCK_T]);
		drawElement(drawIndex.x + 3 * ELEMENT_SIZE, drawIndex.y + 2 * ELEMENT_SIZE, tColors[BLOCK_T]);
		drawElement(drawIndex.x + 4 * ELEMENT_SIZE, drawIndex.y + 2 * ELEMENT_SIZE, tColors[BLOCK_T]);

		drawIndex = (Vector2) {30 * ELEMENT_SIZE, 12 * ELEMENT_SIZE};
		TraceLog(LOG_INFO, "Drawing BLOCK T Preview Rotation 2");
		drawElement(drawIndex.x + 3 * ELEMENT_SIZE, drawIndex.y + 1 * ELEMENT_SIZE, tColors[BLOCK_T]);
		drawElement(drawIndex.x + 3 * ELEMENT_SIZE, drawIndex.y + 2 * ELEMENT_SIZE, tColors[BLOCK_T]);
		drawElement(drawIndex.x + 3 * ELEMENT_SIZE, drawIndex.y + 3 * ELEMENT_SIZE, tColors[BLOCK_T]);
		drawElement(drawIndex.x + 3 * ELEMENT_SIZE, drawIndex.y + 4 * ELEMENT_SIZE, tColors[BLOCK_T]);

		drawIndex = (Vector2) {30 * ELEMENT_SIZE, 18 * ELEMENT_SIZE};
		TraceLog(LOG_INFO, "Drawing BLOCK T Preview Rotation 3");
		drawElement(drawIndex.x + 1 * ELEMENT_SIZE, drawIndex.y + 3 * ELEMENT_SIZE, tColors[BLOCK_T]);
		drawElement(drawIndex.x + 2 * ELEMENT_SIZE, drawIndex.y + 3 * ELEMENT_SIZE, tColors[BLOCK_T]);
		drawElement(drawIndex.x + 3 * ELEMENT_SIZE, drawIndex.y + 3 * ELEMENT_SIZE, tColors[BLOCK_T]);
		drawElement(drawIndex.x + 4 * ELEMENT_SIZE, drawIndex.y + 3 * ELEMENT_SIZE, tColors[BLOCK_T]);

		//////////////////////////////////////////////////////////////////////////////////////////////

		drawIndex = (Vector2) {36.5f * ELEMENT_SIZE, 0.5f * ELEMENT_SIZE};
		TraceLog(LOG_INFO, "Drawing BLOCK Z Preview Rotation 0");
		drawElement(drawIndex.x + 2 * ELEMENT_SIZE, drawIndex.y + 2.5f * ELEMENT_SIZE, tColors[BLOCK_Z]);
		drawElement(drawIndex.x + 1 * ELEMENT_SIZE, drawIndex.y + 1.5f * ELEMENT_SIZE, tColors[BLOCK_Z]);
		drawElement(drawIndex.x + 2 * ELEMENT_SIZE, drawIndex.y + 1.5f * ELEMENT_SIZE, tColors[BLOCK_Z]);
		drawElement(drawIndex.x + 3 * ELEMENT_SIZE, drawIndex.y + 2.5f * ELEMENT_SIZE, tColors[BLOCK_Z]);

		drawIndex = (Vector2) {36.5f * ELEMENT_SIZE, 6.5f * ELEMENT_SIZE};
		TraceLog(LOG_INFO, "Drawing BLOCK Z Preview Rotation 1");
		drawElement(drawIndex.x + 2.5f * ELEMENT_SIZE, drawIndex.y + 1 * ELEMENT_SIZE, tColors[BLOCK_Z]);
		drawElement(drawIndex.x + 2.5f * ELEMENT_SIZE, drawIndex.y + 2 * ELEMENT_SIZE, tColors[BLOCK_Z]);
		drawElement(drawIndex.x + 1.5f * ELEMENT_SIZE, drawIndex.y + 2 * ELEMENT_SIZE, tColors[BLOCK_Z]);
		drawElement(drawIndex.x + 1.5f * ELEMENT_SIZE, drawIndex.y + 3 * ELEMENT_SIZE, tColors[BLOCK_Z]);

		drawIndex = (Vector2) {36.5f * ELEMENT_SIZE, 12.5f * ELEMENT_SIZE};
		TraceLog(LOG_INFO, "Drawing BLOCK Z Preview Rotation 2");
		drawElement(drawIndex.x + 2 * ELEMENT_SIZE, drawIndex.y + 2.5f * ELEMENT_SIZE, tColors[BLOCK_Z]);
		drawElement(drawIndex.x + 1 * ELEMENT_SIZE, drawIndex.y + 1.5f * ELEMENT_SIZE, tColors[BLOCK_Z]);
		drawElement(drawIndex.x + 2 * ELEMENT_SIZE, drawIndex.y + 1.5f * ELEMENT_SIZE, tColors[BLOCK_Z]);
		drawElement(drawIndex.x + 3 * ELEMENT_SIZE, drawIndex.y + 2.5f * ELEMENT_SIZE, tColors[BLOCK_Z]);

		drawIndex = (Vector2) {36.5f * ELEMENT_SIZE, 18.5f * ELEMENT_SIZE};
		TraceLog(LOG_INFO, "Drawing BLOCK Z Preview Rotation 3");
		drawElement(drawIndex.x + 2.5f * ELEMENT_SIZE, drawIndex.y + 1 * ELEMENT_SIZE, tColors[BLOCK_Z]);
		drawElement(drawIndex.x + 2.5f * ELEMENT_SIZE, drawIndex.y + 2 * ELEMENT_SIZE, tColors[BLOCK_Z]);
		drawElement(drawIndex.x + 1.5f * ELEMENT_SIZE, drawIndex.y + 2 * ELEMENT_SIZE, tColors[BLOCK_Z]);
		drawElement(drawIndex.x + 1.5f * ELEMENT_SIZE, drawIndex.y + 3 * ELEMENT_SIZE, tColors[BLOCK_Z]);

	EndTextureMode();

}

void updateBlock(int rotation, int type)
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
			LinesCleared++;
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

	updateBlock(currentBlockRotation, currentBlockType);
	
	if (!DoesPieceFit(currentBlockPos)) {
		isGameOver = true;
	}
}

void appMain()
{
	SetTraceLogLevel(LOG_TRACE);
	SetRandomSeed((unsigned int)time(NULL));

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	SetConfigFlags(FLAG_WINDOW_ALWAYS_RUN);
	SetConfigFlags(FLAG_WINDOW_TOPMOST);
	InitWindow(screenWidth, screenHeight, TextFormat("Raytris v%s", VERSION));
	SetWindowMinSize(screenWidth, screenHeight);

	InitAudioDevice();

	SetExitKey(0);
	SetTargetFPS(60);

	TraceLog(LOG_TRACE, "RNG-Seed: %u", (unsigned int)time(NULL));
	
	renderPreviewTexture();

	fieldRender = LoadRenderTexture(screenWidth, screenHeight);

	Music soundtrack = LoadMusicStream("res/soundtrack.mp3");
	// PlayMusicStream(soundtrack);

	SetAudioStreamVolume(soundtrack.stream, 0.2f);
	soundtrack.looping = true;

	nextBlockType = GetRandomValue(1, 7);
	spawnTetromino();

	TraceLog(LOG_DEBUG, "CurrentBlockType[%d]", currentBlockType);
	TraceLog(LOG_DEBUG, "NextBlockType[%d]", nextBlockType);

	// Init fieldContent to 0
	for (int w = 0; w < FIELD_WIDTH; w++)
		for (int h = 0; h < FIELD_HEIGHT; h++)
			fieldContent[w][h] = 0;

	updateBlock(currentBlockRotation, currentBlockType);

	double lastTick;

	LinesCleared = 0;
	//--------------------------------------------------------------------------------------

	// Main game loop
	//while (!isGameOver && !WindowShouldClose())
	while (!WindowShouldClose())
	{
		// Update
		//----------------------------------------------------------------------------------
		// Update Music Stream
		UpdateMusicStream(soundtrack);
		
		if (IsKeyPressed(KEY_E) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
		{ // CLOCKWISE ROTATION
			currentBlockRotation++;
			if (currentBlockRotation > 3)
				currentBlockRotation = 0;
			updateBlock(currentBlockRotation, currentBlockType);
			if (!DoesPieceFit(currentBlockPos)) {
				currentBlockRotation--;
				updateBlock(currentBlockRotation, currentBlockType);
			}
			if (currentBlockRotation < 0)
				currentBlockRotation = 3;
		}
		else if (IsKeyPressed(KEY_Q) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{ // COUNTERCLOCKWISE ROTATION
			currentBlockRotation--;
			if (currentBlockRotation < 0)
				currentBlockRotation = 3;
			updateBlock(currentBlockRotation, currentBlockType);
			if (!DoesPieceFit(currentBlockPos)) {
				currentBlockRotation++;
				updateBlock(currentBlockRotation, currentBlockType);
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

		updateBlock(currentBlockRotation, currentBlockType);

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
			// Draw FieldBackground
			drawFieldBackground((Vector2) {FIELD_POSX, FIELD_POSY});

			// Draw Field
			for (int w = 0; w < FIELD_WIDTH; w++) {
				for (int h = 0; h < FIELD_HEIGHT; h++) {
					unsigned char c = fieldContent[w][h];
					if (c != 0)
						drawElement(FIELD_POSX + (w * ELEMENT_SIZE), FIELD_POSY + (h * ELEMENT_SIZE), tColors[c]);
				}
			}

			// Draw Current Block
			for (int w = 0; w < 4; w++) {
				for (int h = 0; h < 4; h++) {
					if (currentBlock[w][h]) {
						drawElement(
							FIELD_POSX + (currentBlockPos.x + h) * ELEMENT_SIZE,
							FIELD_POSY + (currentBlockPos.y + w) * ELEMENT_SIZE,
							tColors[currentBlockType]);
					}
				}
			}

			// Draw PreviewBackground
			drawPreviewBackground();

			// Draw Preview
			DrawTexturePro(previewRender.texture, 
			(Rectangle) {120 * (nextBlockType - 1), 120 * (nextBlockRotation - 1), 120, -120},
			(Rectangle) {600, 120, 120, 120},
			(Vector2) {60, 60}, 0.0f, WHITE);


		EndTextureMode();

		BeginDrawing();
			ClearBackground(BLACK);
			DrawRectangleGradientV(0, 0, GetScreenWidth(), GetScreenHeight(),
			DARKBLUE, BLACK);

			DrawTexturePro(fieldRender.texture, (Rectangle) {0, 0, screenWidth, -screenHeight}, (Rectangle) {GetScreenWidth()/2, GetScreenHeight()/2, GetScreenWidth(), GetScreenHeight()}, 
			(Vector2) {GetScreenWidth()/2, GetScreenHeight()/2}, 0.0f, WHITE);


			DrawText(TextFormat("Score: %d", LinesCleared), 10, 10, 18, RAYWHITE);
			DrawText(TextFormat("FPS: %d", GetFPS()), GetScreenWidth() - 75, 10, 18, RAYWHITE);

			DrawText(TextFormat("Next Block: %d", nextBlockType), GetScreenWidth() / 2 + 7 * ELEMENT_SIZE, 10, 18, RAYWHITE);

			// ! DEBUG RENDERING
			DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
			// Draw Preview
			DrawTexturePro(previewRender.texture, 
			(Rectangle) {0, 0, previewSourceRec.width, -previewSourceRec.height},
			previewSourceRec,
			(Vector2) {0, 0}, 0.0f, WHITE);
			// ! DEBUG RENDERING

			// Draw Soundtrack Timeline
			DrawText("Soundtrack made by Melody Ayres-Griffiths", GetScreenWidth() - 382, GetScreenHeight() - 25, 18, WHITE);
			DrawRectanglePro((Rectangle) {0, GetScreenHeight() - 3, GetScreenWidth(), 3}, (Vector2) {0, 0}, 0.0f, DARKGRAY);
			DrawRectanglePro((Rectangle) {0, GetScreenHeight() - 3, (GetMusicTimePlayed(soundtrack)/GetMusicTimeLength(soundtrack)) * GetScreenWidth(), 3}, (Vector2) {0, 0}, 0.0f, RED);
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