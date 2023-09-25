#ifndef RAYTRIS_TETROMINO_H
#define RAYTRIS_TETROMINO_H
#include "raylib.h"

#define ELEMENT_SIZE 22

#define BLOCK_EMPTY   0
#define BLOCK_I       1
#define BLOCK_J       2
#define BLOCK_L       3
#define BLOCK_O       4
#define BLOCK_S       5
#define BLOCK_T       6
#define BLOCK_Z       7

/** All Colors needed for the Tetromino */
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

#endif // RAYTRIS_TETROMINO_H