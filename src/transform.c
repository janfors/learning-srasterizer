#include <rmath.h>
#include <stdio.h>
#include <string.h>
#include <transform.h>

Mesh *transformMesh(Mesh *mesh, Mat4f mvp, PixelBuffer *pixelBuffer) {
  Mesh *out = initMesh(mesh->color, mesh->vertexCount, mesh->indexCount);

  for (int i = 0; i < mesh->vertexCount; i++) {
    Vec4f v = {mesh->vertices[i].pos.x, mesh->vertices[i].pos.y, mesh->vertices[i].pos.z, 1.0f};
    Vec4f clip = mat4fMulVec4f(mvp, v);

    if (fabsf(clip.w) < 1e-6f) {
      fprintf(stderr, "Warning: clip.w near zero for vertex %d\n", i);
      clip.w = 1e-6f;
    }

    float invW = 1.0f / clip.w;
    Vec3f ndc = {clip.x * invW, clip.y * invW, clip.z * invW};

    Vertex outVertex = (Vertex){
        .pos = mesh->vertices[i].pos,
        .screenX = (ndc.x * 0.5f + 0.5f) * pixelBuffer->width,
        .screenY = (1.0f - (ndc.y * 0.5f + 0.5f)) * pixelBuffer->height,
        .depth = ndc.z * 0.5f + 0.5f,
        .invW = invW,
    };

    addVertex(out, outVertex);
  }

  // the indices stay the same
  memcpy(out->indices, mesh->indices, sizeof(size_t) * mesh->indexCount);
  out->vertexCount = mesh->vertexCount;
  out->indexCount = mesh->indexCount;

  return out;
}
