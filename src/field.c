#include "raylib.h"
#include "field.h"
#include "tetromino.h"

int field[FIELD_WIDTH][FIELD_HEIGHT];
RenderTexture2D fieldRenderTex;

void resetField()
{
	for (int w = 0; w < FIELD_WIDTH; w++) {
		for (int h = 0; h < FIELD_HEIGHT; h++) {
			field[w][h] = 0;
		}
	}
}

void renderField()
{
	const int fieldPosX = 268;
	const int fieldPosY =  27;

	BeginTextureMode(fieldRenderTex);
		ClearBackground(BLANK);

		DrawRectangle(fieldPosX - 5, fieldPosY - 5, FIELD_WIDTH * ELEMENT_SIZE + 10, FIELD_HEIGHT * ELEMENT_SIZE + 10, Fade(RAYWHITE, 0.5f));
		DrawRectangleLines(fieldPosX - 5, fieldPosY - 5, FIELD_WIDTH * ELEMENT_SIZE + 10, FIELD_HEIGHT * ELEMENT_SIZE + 10, WHITE);
		DrawRectangleLines(fieldPosX - 1, fieldPosY - 1, FIELD_WIDTH * ELEMENT_SIZE + 2, FIELD_HEIGHT * ELEMENT_SIZE + 2, WHITE);

		for (int w = 0; w < FIELD_WIDTH; w++) {
			for (int h = 0; h < FIELD_HEIGHT; h++) {
				if (field[w][h] != 0) {
					Color color = tColors[field[w][h]];
					DrawRectangleGradientEx((Rectangle) { fieldPosX + w * ELEMENT_SIZE, fieldPosY + h * ELEMENT_SIZE, ELEMENT_SIZE, ELEMENT_SIZE }, color, (Color) {color.r * 0.5f, color.g * 0.5f, color.b * 0.5f, 255}, BLACK, (Color) {color.r * 0.5f, color.g * 0.5f, color.b * 0.5f, 255});
					DrawRectangleLines(fieldPosX + w * ELEMENT_SIZE, fieldPosY + h * ELEMENT_SIZE, ELEMENT_SIZE, ELEMENT_SIZE, BLACK);
				}
			}
		}
	EndTextureMode();

	DrawTexturePro(fieldRenderTex.texture, (Rectangle) {0 , 0, 800, -450}, (Rectangle) {GetScreenWidth() / 2, GetScreenHeight() / 2, GetScreenWidth(), GetScreenHeight()}, (Vector2) {GetScreenWidth() / 2, GetScreenHeight() / 2}, 0.0f, WHITE);
}

void setFieldRenderTexture(RenderTexture2D texture)
{
	fieldRenderTex = texture;
}

void unloadFieldRenderTexture()
{
	UnloadRenderTexture(fieldRenderTex);
}