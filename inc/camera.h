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

  float yaw;   // y rotation
  float pitch; // x rotation
} Camera;

Camera initCamera(Vec3f eye, Vec3f target, Vec3f up, float fov, float aspect, float znear,
                  float zfar);

void updateCameraView(Camera *cam);

void debugCameraOutput(const Camera *cam);

#endif // !_CAMERA_H
