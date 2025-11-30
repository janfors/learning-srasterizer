#ifndef _RMATH_H
#define _RMATH_H

#include <math.h>

typedef struct {
  float x, y, z, w;
} Vec4f;
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

typedef struct {
  float m[4][4];
} Mat4f;

static inline Mat4f mat4fIdentity() {
  return (Mat4f){{
      {1, 0, 0, 0},
      {0, 1, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1},
  }};
}
// Returns the result of matrix multiplication of a and b
Mat4f mat4fMul(Mat4f a, Mat4f b);
// Returns a trnslation Mat4f of vec
static inline Mat4f mat4fTranslate(Vec3f vec) {
  return (Mat4f){{
      {1, 0, 0, vec.x},
      {0, 1, 0, vec.y},
      {0, 0, 1, vec.z},
      {0, 0, 0, 1},
  }};
}
// Returns the scale Mat4f of vec
static inline Mat4f mat4fScale(Vec3f vec) {
  return (Mat4f){{
      {vec.x, 0, 0, 0},
      {0, vec.y, 0, 0},
      {0, 0, vec.z, 0},
      {0, 0, 0, 1},
  }};
}

// Returns the rotation Mat4f around the X axis
static inline Mat4f mat4RotX(float angle) {
  return (Mat4f){{
      {1, 0, 0, 0},
      {0, cosf(angle), -sinf(angle), 0},
      {0, sinf(angle), cosf(angle), 0},
      {0, 0, 0, 1},
  }};
}
// Return the rotation Mat4f around the y axis
static inline Mat4f mat4fRotY(float angle) {
  return (Mat4f){{
      {cosf(angle), 0, sinf(angle), 0},
      {0, 1, 0, 0},
      {-sinf(angle), 0, cosf(angle), 0},
      {0, 0, 0, 1},
  }};
}
// Return the rotation Mat4f around the z axis
static inline Mat4f mat4fRotZ(float angle) {
  return (Mat4f){{
      {cosf(angle), -sinf(angle), 0, 0},
      {sinf(angle), cosf(angle), 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1},
  }};
}
// Returns the projection matrix filled in with the parameters
static inline Mat4f mat4fProjection(float fov, float aspect, float znear, float zfar) {
  float f = 1.0f / tanf(fov * 0.5f);

  return (Mat4f){{
      {f / aspect, 0, 0, 0},
      {0, f, 0, 0},
      {0, 0, -(zfar + znear) / (zfar - znear), -(2 * zfar * znear) / (zfar - znear)},
      {0, 0, -1, 0},
  }};
}

static inline Vec4f mat4fMulVec4f(Mat4f m, Vec4f v) {
  return (Vec4f){m.m[0][0] * v.x + m.m[1][0] * v.y + m.m[2][0] * v.z + m.m[3][0] * v.w,
                 m.m[0][1] * v.x + m.m[1][1] * v.y + m.m[2][1] * v.z + m.m[3][1] * v.w,
                 m.m[0][2] * v.x + m.m[1][2] * v.y + m.m[2][2] * v.z + m.m[3][2] * v.w,
                 m.m[0][3] * v.x + m.m[1][3] * v.y + m.m[2][3] * v.z + m.m[3][3] * v.w};
}

Mat4f mat4fLookAt(Vec3f eye, Vec3f target, Vec3f up);

// Returns the normalized Vec3f
Vec3f normalizeVec3f(Vec3f v);

// All of these probably should have been macros...
static inline float maxf(float a, float b) { return a > b ? a : b; }
static inline float minf(float a, float b) { return a < b ? a : b; }
static inline int maxi(int a, int b) { return a > b ? a : b; }
static inline int mini(int a, int b) { return a < b ? a : b; }

static inline float absf(float x) { return x >= 0 ? x : -x; }
static inline int absi(int x) { return x >= 0 ? x : -x; }

static inline float signf(float x) { return x > 0 ? 1.0f : x == 0 ? 0 : -1.0f; }
static inline int signi(int x) { return x > 0 ? 1 : x == 0 ? 0 : -1; }

static inline Vec3f addVec3f(Vec3f a, Vec3f b) { return (Vec3f){a.x + b.x, a.y + b.y, a.z + b.z}; }
static inline Vec3f subVec3f(Vec3f a, Vec3f b) { return (Vec3f){a.x - b.x, a.y - b.y, a.z - b.z}; }
static inline Vec4f addVec4f(Vec4f a, Vec4f b) {
  return (Vec4f){a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}

// Returns the dot product of a and b
static inline float dot(Vec3f a, Vec3f b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
// Returns the cross product of a and b
static inline Vec3f cross(Vec3f a, Vec3f b) {
  return (Vec3f){
      a.y * b.z - a.z * b.y,
      a.z * b.x - a.x * b.z,
      a.x * b.y - a.y * b.x,
  };
}
#endif // !_RMATH_H
