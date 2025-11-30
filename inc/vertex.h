#ifndef _VERTEX_H
#define _VERTEX_H

#include <rmath.h>

typedef struct {
  Vec3f pos;
  float screenX;
  float screenY;
  float depth;
  float invW;
} Vertex;

static inline Vertex newVertex(float x, float y, float z) {
  return (Vertex){
      .pos = {x, y, z},
      .screenX = 0,
      .screenY = 0,
      .depth = 0,
      .invW = 0,
  };
}

#endif // !_VERTEX_H
