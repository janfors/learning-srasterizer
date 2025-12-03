#include <engine.h>
#include <rasterizer.h>
#include <triangle.h>

void pixelsClear(PixelBuffer *pixelBuffer) {
  memset(pixelBuffer->pixels, 0x06, sizeof(uint32_t) * pixelBuffer->width * pixelBuffer->height);
}

void pixelsClearColor(PixelBuffer *pixelBuffer, uint32_t color) {
  for (int i = 0; i < pixelBuffer->width * pixelBuffer->height; i++)
    pixelBuffer->pixels[i] = color;
}

void drawTriangleFilled(Vertex *v1, Vertex *v2, Vertex *v3, uint32_t color,
                        PixelBuffer *pixelBuffer, Mat4f view) {
  BoundingBox bounds = getTriangleBoundingBox(v1->screenX, v1->screenY, v2->screenX, v2->screenY,
                                              v3->screenX, v3->screenY);
  bounds.minX = maxi(0, bounds.minX);
  bounds.minY = maxi(0, bounds.minY);
  bounds.maxX = mini(pixelBuffer->width - 1, bounds.maxX);
  bounds.maxY = mini(pixelBuffer->height - 1, bounds.maxY);

  Vec2f v1f = (Vec2f){v1->screenX, v1->screenY};
  Vec2f v2f = (Vec2f){v2->screenX, v2->screenY};
  Vec2f v3f = (Vec2f){v3->screenX, v3->screenY};

  float z1 = v1->depth;
  float z2 = v2->depth;
  float z3 = v3->depth;

  float invW1 = v1->invW;
  float invW2 = v2->invW;
  float invW3 = v3->invW;

  // Ensure the correct triangle winding
  float area = edgeFunction(v1f, v2f, v3f);
  if (area < 0) {
    Vec2f tmp = v2f;
    v2f = v3f;
    v3f = tmp;

    float tmpz = z2;
    z2 = z3;
    z3 = tmpz;

    float tmpInvW = invW2;
    invW2 = invW3;
    invW3 = tmpInvW;

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

        float invWInterp = bw0 * invW1 + bw1 * invW2 + bw2 * invW3;
        float z = (bw0 * z1 * invW1 + bw1 * z2 * invW2 + bw2 * z3 * invW3) / invWInterp;

        int idx = y * pixelBuffer->width + x;
        if (z < pixelBuffer->depthBuffer[idx]) {
          Vec3f normalInterp =
              scaleVec3f(addVec3f(addVec3f(scaleVec3f(v1->normals, bw0 * v1->invW),
                                           scaleVec3f(v2->normals, bw1 * v2->invW)),
                                  scaleVec3f(v3->normals, bw2 * v3->invW)),
                         1.0f / invWInterp);
          normalInterp = normalizeVec3f(normalInterp);

          // temp
          Vec3f lightDir = {1, 1, 0};
          lightDir = normalizeVec3f(mat4fMulDir(view, lightDir));

          float lambert = maxf(dot(normalInterp, lightDir), 0.0f);
          uint8_t r = ((color >> 16) & 0xFF) * lambert;
          uint8_t g = ((color >> 8) & 0xFF) * lambert;
          uint8_t b = (color & 0xFF) * lambert;

          pixelBuffer->depthBuffer[idx] = z;
          pixelBuffer->pixels[idx] = rgbu32(r, g, b, 255);
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
  int x0 = (int)roundf(a->screenX);
  int y0 = (int)roundf(a->screenY);
  int x1 = (int)roundf(b->screenX);
  int y1 = (int)roundf(b->screenY);

  float z0 = a->depth;
  float z1 = b->depth;

  bool steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    int tmp;
    tmp = x0;
    x0 = y0;
    y0 = tmp;
    tmp = x1;
    x1 = y1;
    y1 = tmp;
  }

  if (x0 > x1) {
    int tmp;
    tmp = x0;
    x0 = x1;
    x1 = tmp;
    tmp = y0;
    y0 = y1;
    y1 = tmp;
    float ztmp = z0;
    z0 = z1;
    z1 = ztmp;
  }

  int dx = x1 - x0;
  int dy = abs(y1 - y0);
  int error = dx / 2;
  int ystep = (y0 < y1) ? 1 : -1;
  int y = y0;

  for (int x = x0; x <= x1; x++) {
    float t = dx == 0 ? 0.0f : (float)(x - x0) / dx;
    // perspective corectness...
    float invWInterp = (1.0f / a->invW) * (1 - t) + (1.0f / b->invW) * t;
    float z = ((1 - t) * z0 * (1.0f / a->invW) + t * z1 * (1.0f / b->invW)) / invWInterp;

    int drawX = steep ? y : x;
    int drawY = steep ? x : y;

    if (drawX >= 0 && drawX < pixelBuffer->width && drawY >= 0 && drawY < pixelBuffer->height) {
      int idx = drawY * pixelBuffer->width + drawX;
      if (z < pixelBuffer->depthBuffer[idx]) {
        pixelBuffer->depthBuffer[idx] = z;
        pixelBuffer->pixels[idx] = color;
      }
    }

    error -= dy;
    if (error < 0) {
      y += ystep;
      error += dx;
    }
  }
}
