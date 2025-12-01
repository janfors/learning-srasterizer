#include <camera.h>
#include <stdio.h>

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

  cam.pitch = 0;
  cam.yaw = 0;

  return cam;
}

void updateCameraView(Camera *cam) {
  Vec3f forward = {
      cosf(cam->pitch) * sinf(cam->yaw),
      sinf(cam->pitch),
      -cosf(cam->pitch) * cosf(cam->yaw),
  };
  forward = normalizeVec3f(forward);

  cam->target = addVec3f(cam->eye, forward);
  cam->view = mat4fLookAt(cam->eye, cam->target, cam->up);
}

void debugCameraOutput(const Camera *cam) {
  printf("CAM:\n Pos: (%.2f, %.2f, %.2f)\n Yaw:  %.2f°  Pitch: %.2f°\n Target: (%.2f, %.2f, "
         "%.2f)\n\n",
         cam->eye.x, cam->eye.y, cam->eye.z, cam->yaw * 180.0f / M_PI, cam->pitch * 180.0 / M_PI,
         cam->target.x, cam->target.y, cam->target.z);
}
