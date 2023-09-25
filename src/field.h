#ifndef RAYTRIS_FIELD_H
#define RAYTRIS_FIELD_H

#include "raylib.h"

#define FIELD_WIDTH    12
#define FIELD_HEIGHT   18

void resetField();
void renderField();

void setFieldRenderTexture(RenderTexture2D texture);

void unloadFieldRenderTexture();

#endif // RAYTRIS_FIELD_H