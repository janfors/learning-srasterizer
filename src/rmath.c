#include <immintrin.h>
#include <rmath.h>

Mat4f mat4fMul(Mat4f a, Mat4f b) {
  Mat4f c;

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      c.m[i][j] = 0;
    }
  }

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      for (int k = 0; k < 4; k++) {
        c.m[i][j] += a.m[i][k] * b.m[k][j];
      }
    }
  }

  return c;
}

Vec3f normalizeVec3f(Vec3f v) {
  // for pussies (and portable production code...)
  // float invLen = 1 / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
  float lensq = v.x * v.x + v.y * v.y + v.z * v.z;
  float invLen = _mm_cvtss_f32(_mm_rsqrt_ss(_mm_set_ss(lensq)));
  return (Vec3f){v.x * invLen, v.y * invLen, v.z * invLen};
}

Mat4f mat4fLookAt(Vec3f eye, Vec3f target, Vec3f up) {
  Vec3f f = normalizeVec3f(subVec3f(target, eye));
  Vec3f s = normalizeVec3f(cross(f, up));
  Vec3f u = cross(s, f);

  return (Mat4f){{
      {s.x, s.y, s.z, -dot(s, eye)},
      {u.x, u.y, u.z, -dot(u, eye)},
      {-f.x, -f.y, -f.z, dot(f, eye)},
      {0.0f, 0.0f, 0.0f, 1.0f},
  }};
}
