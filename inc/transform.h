#ifndef _TRANSFORM_H
#define _TRANSFORM_H

#include <mesh.h>
#include <pixelbuffer.h>

typedef enum {
  PLANE_NEAR,
  PLANE_FAR,
  PLANE_LEFT,
  PLANE_RIGHT,
  PLANE_BOTTOM,
  PLANE_TOP,
} Plane;

// takes in mesh in local space and returns a new mesh with vertices in screen space
Mesh *transformMeshToClipSpace(Mesh *mesh, Mat4f mvp, PixelBuffer *pixelBuffer);

Vec4f intersectPlane(Vec4f a, Vec4f b, Plane plane);
int clipTrianglePlane(Vertex *in, int inCount, Vertex *out, Plane plane, PixelBuffer *pixelBuffer);

// clips the triangle on all planes
int clipTriangleAllPlanes(Vertex tri[3], Vertex *out, PixelBuffer *pixelBuffer);

static inline bool insideLeft(Vec4f v) { return v.x >= -v.w; }
static inline bool insideRight(Vec4f v) { return v.x <= v.w; }
static inline bool insideBottom(Vec4f v) { return v.y >= -v.w; }
static inline bool insideTop(Vec4f v) { return v.y <= v.w; }
static inline bool insideNear(Vec4f v) { return v.z >= -v.w; }
static inline bool insideFar(Vec4f v) { return v.z <= v.w; }

static inline bool insidePlane(Vec4f v, Plane plane) {
  switch (plane) {
  case PLANE_NEAR:
    return insideNear(v);
  case PLANE_FAR:
    return insideFar(v);
  case PLANE_LEFT:
    return insideLeft(v);
  case PLANE_RIGHT:
    return insideRight(v);
  case PLANE_BOTTOM:
    return insideBottom(v);
  case PLANE_TOP:
    return insideTop(v);
  }

  return false;
}

#endif // !_TRANSFORM_H
