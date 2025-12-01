#ifndef _MESH_H
#define _MESH_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <vertex.h>

typedef struct {
  Vertex *vertices;
  size_t vertexCount;
  size_t capacity;

  size_t *indices;
  size_t indexCount;
  size_t indexCapacity;

  uint32_t color;
} Mesh;

// Returns an initilized mesh, initialIdxCapacity should be a multiple of 3
// as to not waste memory
Mesh *initMesh(uint32_t color, size_t initialVertexCapacity, size_t initialIdxCapacity);
void freeMesh(Mesh *mesh);

// adds the vertex to the meshes vertices
void addVertex(Mesh *mesh, Vertex v);
// adds the indices defining a triangle to the meshes indices
void addTriangle(Mesh *mesh, size_t i0, size_t i1, size_t i2);

static inline bool insideLeft(Vec4f v) { return v.x >= -v.w; }
static inline bool insideRight(Vec4f v) { return v.x <= v.w; }
static inline bool insideBottom(Vec4f v) { return v.y >= -v.w; }
static inline bool insideTop(Vec4f v) { return v.y <= v.w; }
static inline bool insideNear(Vec4f v) { return v.z >= v.w; }
static inline bool insideFar(Vec4f v) { return v.z <= v.w; }

#endif // !_MESH_H
