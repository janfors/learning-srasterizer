#ifndef _TRIANGLE_H
#define _TRIANGLE_H

#include <engine.h>
#include <math.h>

typedef struct {
  int minX, maxX;
  int minY, maxY;
} BoundingBox;

// calculate the bounding box of a triangle defined in screen space
BoundingBox getTriangleBoundingBox(Vec2i *v1, Vec2i *v2, Vec2i *v3);

// Barycentric edge function
static inline float edgeFunction(Vec2f a, Vec2f b, Vec2f c) {
  return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}
#endif // !_TRIANGLE_H
