#include <engine.h>
#include <rasterizer.h>
#include <triangle.h>

void pixelsClear(PixelBuffer *pixelBuffer, uint32_t color) {
  memset(pixelBuffer->pixels, color, sizeof(uint32_t) * pixelBuffer->width * pixelBuffer->height);
}

void drawTriangleFilled(Vec2i *v1, Vec2i *v2, Vec2i *v3, uint32_t color, PixelBuffer *pixelBuffer) {
  BoundingBox bounds = getTriangleBoundingBox(v1, v2, v3);

  Vec2f v1f = (Vec2f){v1->x, v1->y};
  Vec2f v2f = (Vec2f){v2->x, v2->y};
  Vec2f v3f = (Vec2f){v3->x, v3->y};

  // Ensure the correct triangle winding
  float area = edgeFunction(&v1f, &v2f, &v3f);
  if (area < 0) {
    Vec2f tmp = v2f;
    v2f = v3f;
    v3f = tmp;
    area = -area;
  }

  for (int x = bounds.minX; x <= bounds.maxX; x++) {
    for (int y = bounds.minY; y <= bounds.maxY; y++) {
      Vec2f p = (Vec2f){x + 0.5f, y + 0.5f};

      float w0 = edgeFunction(&v2f, &v3f, &p);
      float w1 = edgeFunction(&v3f, &v1f, &p);
      float w2 = edgeFunction(&v1f, &v2f, &p);

      if (w0 >= 0 && w1 >= 0 && w2 >= 0)
        drawPixel(pixelBuffer->pixels, x, y, pixelBuffer->width, color);
    }
  }
}
