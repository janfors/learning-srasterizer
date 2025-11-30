#include <engine.h>
#include <rasterizer.h>
#include <triangle.h>

void pixelsClear(PixelBuffer *pixelBuffer) {
  memset(pixelBuffer->pixels, 0, sizeof(uint32_t) * pixelBuffer->width * pixelBuffer->height);
}

void drawTriangleFilled(Vec3f *v1, Vec3f *v2, Vec3f *v3, uint32_t color, PixelBuffer *pixelBuffer) {
  BoundingBox bounds = getTriangleBoundingBox(v1, v2, v3);
  bounds.minX = maxi(0, bounds.minX);
  bounds.minY = maxi(0, bounds.minY);
  bounds.maxX = mini(pixelBuffer->width - 1, bounds.maxX);
  bounds.maxY = mini(pixelBuffer->height - 1, bounds.maxY);

  Vec2f v1f = (Vec2f){v1->x, v1->y};
  Vec2f v2f = (Vec2f){v2->x, v2->y};
  Vec2f v3f = (Vec2f){v3->x, v3->y};

  float z1 = v1->z;
  float z2 = v2->z;
  float z3 = v3->z;

  // Ensure the correct triangle winding
  float area = edgeFunction(v1f, v2f, v3f);
  if (area < 0) {
    Vec2f tmp = v2f;
    v2f = v3f;
    v3f = tmp;

    float tmpz = z2;
    z2 = z3;
    z3 = tmpz;

    area = -area;
  }

  if (area == 0)
    return;

  float invArea = 1.0f / area;

  float w0_dx = v3f.y - v2f.y;
  float w0_dy = v2f.x - v3f.x;

  float w1_dx = v1f.y - v3f.y;
  float w1_dy = v3f.x - v1f.x;

  float w2_dx = v2f.y - v1f.y;
  float w2_dy = v1f.x - v2f.x;

  Vec2f p = (Vec2f){bounds.minX + 0.5f, bounds.minY + 0.5f};
  float w0_row = edgeFunction(v2f, v3f, p);
  float w1_row = edgeFunction(v3f, v1f, p);
  float w2_row = edgeFunction(v1f, v2f, p);

  for (int y = bounds.minY; y <= bounds.maxY; y++) {
    float w0 = w0_row;
    float w1 = w1_row;
    float w2 = w2_row;

    for (int x = bounds.minX; x <= bounds.maxX; x++) {
      if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
        float bw0 = w0 * invArea;
        float bw1 = w1 * invArea;
        float bw2 = w2 * invArea;

        float z = bw0 * z1 + bw1 * z2 + bw2 * z3;
        int idx = y * pixelBuffer->width + x;

        if (z < pixelBuffer->depthBuffer[idx]) {
          pixelBuffer->depthBuffer[idx] = z;
          pixelBuffer->pixels[idx] = color;
        }
      }

      w0 += w0_dx;
      w1 += w1_dx;
      w2 += w2_dx;
    }

    w0_row += w0_dy;
    w1_row += w1_dy;
    w2_row += w2_dy;
  }
}

void drawTriangleWireframe(Vec3f *v1, Vec3f *v2, Vec3f *v3, uint32_t color,
                           PixelBuffer *pixelBuffer) {
  drawLine(v1, v2, color, pixelBuffer);
  drawLine(v2, v3, color, pixelBuffer);
  drawLine(v3, v1, color, pixelBuffer);
}

void drawLine(Vec3f *a, Vec3f *b, uint32_t color, PixelBuffer *pixelBuffer) {
  if ((a->x < 0 && b->x < 0) || (a->x >= pixelBuffer->width && b->x >= pixelBuffer->width))
    return;
  if ((a->y < 0 && b->y < 0) || (a->y >= pixelBuffer->height && b->y >= pixelBuffer->height))
    return;

  int x = (int)a->x;
  int y = (int)a->y;

  int dx = (int)absf(b->x - a->x);
  int dy = (int)absf(b->y - a->y);
  float dz = b->z - a->z;

  int signx = (int)signf(b->x - a->x);
  int signy = (int)signf(b->y - a->y);

  bool steep = dy > dx;
  if (steep) {
    float tmp = dx;
    dx = dy;
    dy = tmp;
  }

  if (dx == 0) {
    if (x >= 0 && x < pixelBuffer->width && y >= 0 && y < pixelBuffer->height) {
      int idx = y * pixelBuffer->width + x;
      if (a->z < pixelBuffer->depthBuffer[idx]) {
        pixelBuffer->depthBuffer[idx] = a->z;
        pixelBuffer->pixels[idx] = color;
      }
    }

    return;
  }

  float t = 0.0f;
  float dt = 1.0f / (float)dx;

  float e = 2 * dy - dx;
  for (int i = 0; i <= dx; i++) {
    if (x >= 0 && x < pixelBuffer->width && y >= 0 && y < pixelBuffer->height) {
      float z = a->z + t * dz;
      int idx = y * pixelBuffer->width + x;
      if (z < pixelBuffer->depthBuffer[idx]) {
        pixelBuffer->depthBuffer[idx] = z;
        pixelBuffer->pixels[idx] = color;
      }
    }

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
