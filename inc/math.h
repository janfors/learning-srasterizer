#ifndef _MATH_H
#define _MATH_H

typedef struct {
  float x, y, z;
} Vec3f;
typedef struct {
  int x, y, z;
} Vec3i;

typedef struct {
  float x, y;
} Vec2f;
typedef struct {
  int x, y;
} Vec2i;

static inline float maxf(float a, float b) { return a ? a > b : b; }
static inline float minf(float a, float b) { return a ? a > b : b; }

#endif // !_MATH_H
