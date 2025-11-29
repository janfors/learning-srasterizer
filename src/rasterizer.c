#include <engine.h>
#include <rasterizer.h>

void pixelsClear(PixelBuffer *pixelBuffer, uint32_t color) {
  memset(pixelBuffer->pixels, color, sizeof(uint32_t) * pixelBuffer->width * pixelBuffer->height);
}
