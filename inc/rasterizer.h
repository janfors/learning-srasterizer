#ifndef _RASTERIZER_H
#define _RASTERIZER_H

#include <engine.h>

// clears the whole pixel buffer
void pixelsClear(PixelBuffer *pixelBuffer);

// draw a triangle using a Barycentric Algorithm on the pixelBuffer
void drawTriangleFilled(Vec3f *v1, Vec3f *v2, Vec3f *v3, uint32_t color, PixelBuffer *pixelBuffer);
void drawTriangleWireframe(Vec3f *v1, Vec3f *v2, Vec3f *v3, uint32_t color,
                           PixelBuffer *pixelBuffer);

void drawLine(Vec3f *a, Vec3f *b, uint32_t color, PixelBuffer *pixelBuffer);

static inline void drawPixel(uint32_t *restrict pixels, size_t x, size_t y, size_t w,
                             uint32_t color) {
  pixels[y * w + x] = color;
}

#endif // !_RASTERIZER_H
