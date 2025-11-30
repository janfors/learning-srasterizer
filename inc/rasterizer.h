#ifndef _RASTERIZER_H
#define _RASTERIZER_H

#include <engine.h>

// clears the whole pixel buffer
void pixelsClear(PixelBuffer *pixelBuffer);

// draw a triangle using a Barycentric Algorithm on the pixelBuffer
void drawTriangleFilled(Vec2i *v1, Vec2i *v2, Vec2i *v3, uint32_t color, PixelBuffer *pixelBuffer);
void drawTriangleWireframe(Vec2i *v1, Vec2i *v2, Vec2i *v3, uint32_t color,
                           PixelBuffer *pixelBuffer);

void drawLine(Vec2i *a, Vec2i *b, uint32_t color, PixelBuffer *pixelBuffer);

static inline void drawPixel(uint32_t *restrict pixels, size_t x, size_t y, size_t w,
                             uint32_t color) {
  pixels[y * w + x] = color;
}

#endif // !_RASTERIZER_H
