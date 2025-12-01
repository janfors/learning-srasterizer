#include <stdbool.h>
#include <triangle.h>

BoundingBox getTriangleBoundingBox(int x1, int y1, int x2, int y2, int x3, int y3) {
  BoundingBox box;
  box.minX = mini(x1, mini(x2, x3));
  box.minY = mini(y1, mini(y2, y3));
  box.maxX = maxi(x1, maxi(x2, x3));
  box.maxY = maxi(y1, maxi(y2, y3));
  return box;
}

int clipTriangleNear(Vertex *in, Vertex *out, PixelBuffer *pixelBuffer) {
  int outCount = 0;

  for (int i = 0; i < 3; i++) {
    Vertex curr = in[i];
    Vertex next = in[(i + 1) % 3]; // wrap

    float currW = 1.0f / curr.invW;
    float nextW = 1.0f / next.invW;

    bool currInside = curr.pos.z >= -currW;
    bool nextInside = next.pos.z >= -nextW;

    if (currInside && nextInside) {
      out[outCount++] = next;
    } else if (currInside && !nextInside) {
      float t = (currW + curr.pos.z) / ((currW + curr.pos.z) - (nextW + next.pos.z));
      Vec4f outVec = lerpVec4f(&(Vec4f){curr.pos.x, curr.pos.y, curr.pos.z, currW},
                               &(Vec4f){next.pos.x, next.pos.y, next.pos.z, nextW}, t);

      float invW = 1.0f / outVec.w;
      out[outCount] =
          (Vertex){.pos = {outVec.x, outVec.y, outVec.z},
                   .screenX = (outVec.x * invW * 0.5f + 0.5f) * pixelBuffer->width,
                   .screenY = (1.0f - (outVec.y * invW * 0.5f + 0.5f)) * pixelBuffer->height,
                   .depth = outVec.z * invW * 0.5f + 0.5f,
                   .invW = invW};

    } else if (!currInside && nextInside) {
      float t = (currW + curr.pos.z) / ((currW + curr.pos.z) - (nextW + next.pos.z));
      Vec4f outVec = lerpVec4f(&(Vec4f){curr.pos.x, curr.pos.y, curr.pos.z, currW},
                               &(Vec4f){next.pos.x, next.pos.y, next.pos.z, nextW}, t);

      float invW = 1.0f / outVec.w;
      out[outCount] =
          (Vertex){.pos = {outVec.x, outVec.y, outVec.z},
                   .screenX = (outVec.x * invW * 0.5f + 0.5f) * pixelBuffer->width,
                   .screenY = (1.0f - (outVec.y * invW * 0.5f + 0.5f)) * pixelBuffer->height,
                   .depth = outVec.z * invW * 0.5f + 0.5f,
                   .invW = invW};

      out[outCount++] = next;
    }
  }

  return outCount;
}
