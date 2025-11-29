#ifndef _RASTERIZER_H
#define _RASTERIZER_H

#include <engine.h>

// clears the whole pixel buffer with {color}
void pixelsClear(PixelBuffer *pixelBuffer, uint32_t color);

static inline void drawPixel(uint32_t *restrict pixels, size_t x, size_t y, size_t w,
                             uint32_t color) {
  pixels[y * w + x] = color;
}

#endif // !_RASTERIZER_H
