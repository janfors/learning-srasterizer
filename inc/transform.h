#ifndef _TRANSFORM_H
#define _TRANSFORM_H

#include <mesh.h>
#include <pixelbuffer.h>

// takes in mesh in local space and returns a new mesh with vertices in screen space
Mesh *transformMesh(Mesh *mesh, Mat4f mvp, PixelBuffer *pixelBuffer);

#endif // !_TRANSFORM_H
