#ifndef _CAMERA_H
#define _CAMERA_H

#include <rmath.h>

typedef struct {
  Vec3f eye;
  Vec3f target;
  Vec3f up;

  float fov;
  float aspect;
  float znear;
  float zfar;

  Mat4f view;
  Mat4f projection;
} Camera;

Camera initCamera(Vec3f eye, Vec3f target, Vec3f up, float fov, float aspect, float znear,
                  float zfar);

#endif // !_CAMERA_H
