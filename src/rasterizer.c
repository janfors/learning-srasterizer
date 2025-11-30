#include <engine.h>
#include <rasterizer.h>
#include <triangle.h>

void pixelsClear(PixelBuffer *pixelBuffer) {
  memset(pixelBuffer->pixels, 0, sizeof(uint32_t) * pixelBuffer->width * pixelBuffer->height);
}

void pixelsClearColor(PixelBuffer *pixelBuffer, uint32_t color) {
  for (int i = 0; i < pixelBuffer->width * pixelBuffer->height; i++)
    pixelBuffer->pixels[i] = color;
}

void drawTriangleFilled(Vertex *v1, Vertex *v2, Vertex *v3, uint32_t color,
                        PixelBuffer *pixelBuffer) {
  Vec3f v1Screen = {v1->screenX, v1->screenY, v1->depth};
  Vec3f v2Screen = {v2->screenX, v2->screenY, v2->depth};
  Vec3f v3Screen = {v3->screenX, v3->screenY, v3->depth};

  // DEBUG
  printf("Triangle: (%f, %f, %f)\n", v1->screenX, v1->screenY, v1->depth);
  fflush(stdout);

  BoundingBox bounds = getTriangleBoundingBox(v1->screenX, v1->screenY, v2->screenX, v2->screenY,
                                              v3->screenX, v3->screenY);
  bounds.minX = maxi(0, bounds.minX);
  bounds.minY = maxi(0, bounds.minY);
  bounds.maxX = mini(pixelBuffer->width - 1, bounds.maxX);
  bounds.maxY = mini(pixelBuffer->height - 1, bounds.maxY);

  Vec2f v1f = (Vec2f){v1Screen.x, v1Screen.y};
  Vec2f v2f = (Vec2f){v2Screen.x, v2Screen.y};
  Vec2f v3f = (Vec2f){v3Screen.x, v3Screen.y};

  float z1 = v1Screen.z;
  float z2 = v2Screen.z;
  float z3 = v3Screen.z;

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

void drawTriangleWireframe(Vertex *v1, Vertex *v2, Vertex *v3, uint32_t color,
                           PixelBuffer *pixelBuffer) {
  drawLine(v1, v2, color, pixelBuffer);
  drawLine(v2, v3, color, pixelBuffer);
  drawLine(v3, v1, color, pixelBuffer);
}

void drawLine(Vertex *a, Vertex *b, uint32_t color, PixelBuffer *pixelBuffer) {
  int x0 = (int)a->screenX;
  int y0 = (int)a->screenY;
  int x1 = (int)b->screenX;
  int y1 = (int)b->screenY;

  float z0 = a->depth;
  float z1 = b->depth;

  int dx = absi(x1 - x0);
  int dy = absi(y1 - y0);
  int signx = signi(x1 - x0);
  int signy = signi(y1 - y0);

  bool steep = dy > dx;
  if (steep) {
    int tmp = dx;
    dx = dy;
    dy = tmp;
  }

  int x = x0;
  int y = y0;
  float dz = z1 - z0;
  float t = 0.0f;
  float dt = (dx != 0) ? 1.0f / (float)dx : 0.0f;

  int e = 2 * dy - dx;
  for (int i = 0; i <= dx; i++) {
    if (x >= 0 && x < pixelBuffer->width && y >= 0 && y < pixelBuffer->height) {
      float z = z0 + t * dz;
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
    t += dt;
  }
}
