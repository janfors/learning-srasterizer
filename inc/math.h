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

static inline float maxf(float a, float b) { return a > b ? a : b; }
static inline float minf(float a, float b) { return a < b ? a : b; }
static inline int maxi(int a, int b) { return a > b ? a : b; }
static inline int mini(int a, int b) { return a < b ? a : b; }

static inline int absi(int x) { return x >= 0 ? x : -x; }
static inline int signi(int x) { return x >= 0 ? 1 : -1; }

static inline Vec3f cross(Vec3f a, Vec3f b) {
  return (Vec3f){
      a.y * b.z - a.z * b.y,
      a.z * b.x - a.x * b.z,
      a.x * b.y - a.y * b.x,
  };
}
#endif // !_MATH_H
