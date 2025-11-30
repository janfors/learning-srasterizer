#include <rmath.h>
#include <string.h>
#include <transform.h>

Mesh *transformMesh(Mesh *mesh, Mat4f mvp, PixelBuffer *pixelBuffer) {
  Mesh *out = initMesh(mesh->color, mesh->vertexCount, mesh->indexCount);

  for (int i = 0; i < mesh->vertexCount; i++) {
    Vec4f v = {mesh->vertices[i].pos.x, mesh->vertices[i].pos.y, mesh->vertices[i].pos.z, 1.0f};
    Vec4f clip = mat4fMulVec4f(mvp, v);
    float invW = 1.0f / clip.w;
    Vec3f ndc = {clip.x * invW, clip.y * invW, clip.z * invW};

    out->vertices[i].screenX = (ndc.x * 0.5f + 0.5f) * pixelBuffer->width;
    out->vertices[i].screenY = (1.0f - (ndc.y * 0.5f + 0.5f)) * pixelBuffer->height;
    out->vertices[i].depth = ndc.z * 0.5f + 0.5f;
    out->vertices[i].invW = invW;

    out->vertices[i].pos = mesh->vertices[i].pos;
  }

  // the indices stay the same
  memcpy(out->indices, mesh->indices, sizeof(size_t) * mesh->indexCount);
  out->vertexCount = mesh->vertexCount;
  out->indexCount = mesh->indexCount;

  return out;
}
