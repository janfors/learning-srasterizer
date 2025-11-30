#include <engine.h>
#include <rasterizer.h>
#include <triangle.h>

void pixelsClear(PixelBuffer *pixelBuffer) {
  memset(pixelBuffer->pixels, 0, sizeof(uint32_t) * pixelBuffer->width * pixelBuffer->height);
}

void drawTriangleFilled(Vec2i *v1, Vec2i *v2, Vec2i *v3, uint32_t color, PixelBuffer *pixelBuffer) {
  BoundingBox bounds = getTriangleBoundingBox(v1, v2, v3);
  bounds.minX = maxi(0, bounds.minX);
  bounds.minY = maxi(0, bounds.minY);
  bounds.maxX = mini(pixelBuffer->width - 1, bounds.maxX);
  bounds.maxY = mini(pixelBuffer->height - 1, bounds.maxY);

  Vec2f v1f = (Vec2f){v1->x, v1->y};
  Vec2f v2f = (Vec2f){v2->x, v2->y};
  Vec2f v3f = (Vec2f){v3->x, v3->y};

  // Ensure the correct triangle winding
  float area = edgeFunction(v1f, v2f, v3f);
  if (area < 0) {
    Vec2f tmp = v2f;
    v2f = v3f;
    v3f = tmp;
    area = -area;
  }

  float dw0 = v3f.y - v2f.y;
  float dw1 = v1f.y - v3f.y;
  float dw2 = v2f.y - v1f.y;

  for (int y = bounds.minY; y <= bounds.maxY; y++) {
    Vec2f p = (Vec2f){bounds.minX + 0.5f, y + 0.5f};

    // Calculate at the start of the scanline instead of per pixel
    float w0 = edgeFunction(v2f, v3f, p);
    float w1 = edgeFunction(v3f, v1f, p);
    float w2 = edgeFunction(v1f, v2f, p);

    for (int x = bounds.minX; x <= bounds.maxX; x++) {
      if (w0 >= 0 && w1 >= 0 && w2 >= 0)
        drawPixel(pixelBuffer->pixels, x, y, pixelBuffer->width, color);

      // they change linearly so we can not calculate them and just offset the w values
      w0 += dw0;
      w1 += dw1;
      w2 += dw2;
    }
  }
}

void drawTriangleWireframe(Vec2i *v1, Vec2i *v2, Vec2i *v3, uint32_t color,
                           PixelBuffer *pixelBuffer) {
  drawLine(v1, v2, color, pixelBuffer);
  drawLine(v2, v3, color, pixelBuffer);
  drawLine(v3, v1, color, pixelBuffer);
}

void drawLine(Vec2i *a, Vec2i *b, uint32_t color, PixelBuffer *pixelBuffer) {
  int x = a->x;
  int y = a->y;

  int dx = absi(b->x - a->x);
  int dy = absi(b->y - a->y);
  int signx = signi(b->x - a->x);
  int signy = signi(b->y - a->y);

  bool steep = dy > dx;
  if (steep) {
    int tmp = dx;
    dx = dy;
    dy = tmp;
  }

  float e = 2 * dy - dx;
  for (int i = 0; i <= dx; i++) {
    drawPixel(pixelBuffer->pixels, x, y, pixelBuffer->width, color);

    while (e >= 0) {
      if (steep)
        x += signx;
      else
        y += signy;

      e -= 2 * dx;
    }

    if (steep)
      y += signy;
    else
      x += signx;

    e += 2 * dy;
  }
}
