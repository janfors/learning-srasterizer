#ifndef _TRIANGLE_H
#define _TRIANGLE_H

#include <pixelbuffer.h>
#include <rmath.h>
#include <vertex.h>

typedef struct {
  int minX, maxX;
  int minY, maxY;
} BoundingBox;

// calculate the bounding box of a triangle defined in screen space
BoundingBox getTriangleBoundingBox(int x1, int y1, int x2, int y2, int x3, int y3);

// clip a triangle against the near plane ( z >= -w)
int clipTriangleNear(Vertex in[3], Vertex out[4], PixelBuffer *pixelBuffer);

// Barycentric edge function
static inline float edgeFunction(Vec2f a, Vec2f b, Vec2f c) {
  return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}
#endif // !_TRIANGLE_H
