#ifndef _PIXEL_BUFFER_H
#define _PIXEL_BUFFER_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
  uint32_t *pixels;
  size_t width;
  size_t height;
} PixelBuffer;

#endif // !_PIXEL_BUFFER_H
