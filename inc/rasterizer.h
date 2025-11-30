#ifndef _RASTERIZER_H
#define _RASTERIZER_H

#include <pixelbuffer.h>
#include <rmath.h>
#include <vertex.h>

// clears the whole pixel buffer
void pixelsClear(PixelBuffer *pixelBuffer);
// clears the whole pixel buffer with a color (slower)
void pixelsClearColor(PixelBuffer *pixelBuffer, uint32_t color);

// draw a triangle using a Barycentric Algorithm on the pixelBuffer
void drawTriangleFilled(Vertex *v1, Vertex *v2, Vertex *v3, uint32_t color,
                        PixelBuffer *pixelBuffer);
void drawTriangleWireframe(Vertex *v1, Vertex *v2, Vertex *v3, uint32_t color,
                           PixelBuffer *pixelBuffer);

void drawLine(Vertex *a, Vertex *b, uint32_t color, PixelBuffer *pixelBuffer);

static inline void drawPixel(uint32_t *restrict pixels, size_t x, size_t y, size_t w,
                             uint32_t color) {
  pixels[y * w + x] = color;
}

#endif // !_RASTERIZER_H
