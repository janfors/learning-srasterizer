#include <camera.h>

Camera initCamera(Vec3f eye, Vec3f target, Vec3f up, float fov, float aspect, float znear,
                  float zfar) {
  Camera cam;
  cam.eye = eye;
  cam.target = target;
  cam.up = up;
  cam.fov = fov;
  cam.aspect = aspect;
  cam.znear = znear;
  cam.zfar = zfar;

  cam.view = mat4fLookAt(eye, target, up);
  cam.projection = mat4fProjection(fov, aspect, znear, zfar);

  return cam;
}
