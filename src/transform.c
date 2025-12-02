#include <rmath.h>
#include <stdio.h>
#include <string.h>
#include <transform.h>

Mesh *transformMeshToClipSpace(Mesh *mesh, Mat4f mvp, PixelBuffer *pixelBuffer) {
  Mesh *out = initMesh(mesh->color, mesh->vertexCount, mesh->indexCount);

  for (int i = 0; i < mesh->vertexCount; i++) {
    Vec4f v = {mesh->vertices[i].pos.x, mesh->vertices[i].pos.y, mesh->vertices[i].pos.z, 1.0f};
    Vec4f clip = mat4fMulVec4f(mvp, v);

    Vertex outVertex = mesh->vertices[i];
    outVertex.clip = clip;
    outVertex.invW = 1.0f / clip.w;

    addVertex(out, outVertex);
  }

  // the indices stay the same
  memcpy(out->indices, mesh->indices, sizeof(size_t) * mesh->indexCount);
  out->vertexCount = mesh->vertexCount;
  out->indexCount = mesh->indexCount;

  return out;
}

int clipTriangleAllPlanes(Vertex tri[3], Vertex *out, PixelBuffer *pixelBuffer) {
  Vertex tmpA[9];
  Vertex tmpB[9];

  int n = 3;
  memcpy(tmpA, tri, sizeof(Vertex) * 3);

  n = clipTrianglePlane(tmpA, n, tmpB, PLANE_NEAR, pixelBuffer);
  if (n < 3)
    return 0;
  n = clipTrianglePlane(tmpB, n, tmpA, PLANE_FAR, pixelBuffer);
  if (n < 3)
    return 0;
  n = clipTrianglePlane(tmpA, n, tmpB, PLANE_LEFT, pixelBuffer);
  if (n < 3)
    return 0;
  n = clipTrianglePlane(tmpB, n, tmpA, PLANE_RIGHT, pixelBuffer);
  if (n < 3)
    return 0;
  n = clipTrianglePlane(tmpA, n, tmpB, PLANE_BOTTOM, pixelBuffer);
  if (n < 3)
    return 0;
  n = clipTrianglePlane(tmpB, n, tmpA, PLANE_TOP, pixelBuffer);
  if (n < 3)
    return 0;

  memcpy(out, tmpA, sizeof(Vertex) * n);
  return n;
}

Vec4f intersectPlane(Vec4f a, Vec4f b, Plane plane) {
  float t;
  switch (plane) {
  case PLANE_NEAR:
    t = (-(a.z + a.w)) / ((b.z + b.w) - (a.z + a.w));
    break;
  case PLANE_FAR:
    t = ((a.z - a.w)) / ((a.z - a.w) - (b.z - b.w));
    break;
  case PLANE_LEFT:
    t = (-(a.x + a.w)) / ((b.x + b.w) - (a.x + a.w));
    break;
  case PLANE_RIGHT:
    t = ((a.x - a.w)) / ((a.x - a.w) - (b.x - b.w));
    break;
  case PLANE_BOTTOM:
    t = (-(a.y + a.w)) / ((b.y + b.w) - (a.y + a.w));
    break;
  case PLANE_TOP:
    t = ((a.y - a.w)) / ((a.y - a.w) - (b.y - b.w));
    break;
  }

  Vec4f out = {
      .x = a.x + t * (b.x - a.x),
      .y = a.y + t * (b.y - a.y),
      .z = a.z + t * (b.z - a.z),
      .w = a.w + t * (b.w - a.w),
  };

  return out;
}

static void toNdc(Vertex *v, PixelBuffer *pixelBuffer) {
  Vec3f ndc = {
      v->clip.x * v->invW,
      v->clip.y * v->invW,
      v->clip.z * v->invW,
  };

  v->screenX = (ndc.x * 0.5f + 0.5f) * pixelBuffer->width;
  v->screenY = (1.0f - (ndc.y * 0.5f + 0.5f)) * pixelBuffer->height;
  v->depth = ndc.z * 0.5f + 0.5f;
}

// Probably a cleaner way of doing this exists...
int clipTrianglePlane(Vertex *in, int inCount, Vertex *out, Plane plane, PixelBuffer *pixelBuffer) {
  int outCount = 0;

  for (int i = 0; i < inCount; i++) {
    Vertex curr = in[i];
    Vertex next = in[(i + 1) % inCount];

    bool currInside = insidePlane(curr.clip, plane);
    bool nextInside = insidePlane(next.clip, plane);

    if (currInside && nextInside) {
      out[outCount] = next;
      toNdc(&out[outCount], pixelBuffer);
      outCount++;
    }

    if (currInside && !nextInside) {
      Vec4f ipt = intersectPlane(curr.clip, next.clip, plane);
      float invW = 1.0f / ipt.w;

      Vertex v;
      v.pos = (Vec3f){ipt.x, ipt.y, ipt.z};
      v.clip = ipt;
      v.invW = invW;

      toNdc(&v, pixelBuffer);

      out[outCount++] = v;
    } else if (!currInside && nextInside) {
      Vec4f ipt = intersectPlane(curr.clip, next.clip, plane);
      float invW = 1.0f / ipt.w;

      Vertex v;
      v.pos = (Vec3f){ipt.x, ipt.y, ipt.z};
      v.clip = ipt;
      v.invW = invW;

      toNdc(&v, pixelBuffer);

      out[outCount++] = v;
      out[outCount] = next;
      toNdc(&out[outCount++], pixelBuffer);
    }
  }

  return outCount;
}
