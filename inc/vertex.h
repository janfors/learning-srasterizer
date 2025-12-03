#ifndef _VERTEX_H
#define _VERTEX_H

#include <rmath.h>

typedef struct {
  Vec3f pos;
  Vec4f clip;

  float screenX;
  float screenY;
  float depth;

  Vec3f normals;
  Vec2f uv;

  float invW;
} Vertex;

static inline Vertex newVertex(float x, float y, float z) {
  return (Vertex){
      .pos = {x, y, z},
      .screenX = 0,
      .screenY = 0,
      .depth = 0,
      .invW = 0,
      .clip = {0, 0, 0, 0},
  };
}

#endif // !_VERTEX_H
