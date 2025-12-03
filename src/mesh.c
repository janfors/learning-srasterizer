#include <mesh.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Mesh *initMesh(uint32_t color, size_t initialVertexCapacity, size_t initialIdxCapacity) {
  Mesh *mesh = malloc(sizeof(Mesh));

  mesh->vertexCount = 0;
  mesh->capacity = initialVertexCapacity;
  mesh->vertices = malloc(sizeof(Vertex) * initialVertexCapacity);

  mesh->indexCount = 0;
  mesh->indexCapacity = initialIdxCapacity;
  mesh->indices = malloc(sizeof(size_t) * initialIdxCapacity);

  if (!mesh->vertices || !mesh->indices) {
    fprintf(stderr, "Failed to allocate mesh memory!\n");
    exit(1);
  }

  mesh->color = color;

  mesh->modelMatrix = mat4fIdentity();

  return mesh;
}

void addVertex(Mesh *mesh, Vertex v) {
  if (mesh->vertexCount >= mesh->capacity) {
    mesh->capacity *= 2;
    mesh->vertices = realloc(mesh->vertices, sizeof(Vertex) * mesh->capacity);
  }

  mesh->vertices[mesh->vertexCount++] = v;
}

void addTriangle(Mesh *mesh, size_t i0, size_t i1, size_t i2) {
  if (mesh->indexCount >= mesh->indexCapacity) {
    mesh->indexCapacity += 3;
    mesh->indices = realloc(mesh->indices, sizeof(size_t) * mesh->indexCapacity);
  }

  mesh->indices[mesh->indexCount++] = i0;
  mesh->indices[mesh->indexCount++] = i1;
  mesh->indices[mesh->indexCount++] = i2;
}

void meshFromModel(Mesh *mesh, ModelData *data) {
  for (int i = 0; i < data->posCount; i++) {
    Vertex v = newVertex(data->pos[i].x, data->pos[i].y, data->pos[i].z);
    addVertex(mesh, v);
  }
  if (mesh->indexCapacity < data->indexCount) {
    mesh->indexCapacity = data->indexCount;
    mesh->indices = realloc(mesh->indices, sizeof(size_t) * (data->indexCount));
  }

  mesh->indexCount = data->indexCount;
  for (int i = 0; i < data->indexCount; i++) {
    mesh->indices[i] = data->vertexIndices[i];
  }
}

void freeMesh(Mesh *mesh) {
  free(mesh->vertices);
  free(mesh->indices);

  mesh->vertices = NULL;
  mesh->indices = NULL;
  mesh->vertexCount = 0;
  mesh->indexCount = 0;
  mesh->capacity = 0;
  mesh->indexCapacity = 0;
  mesh->color = 0;

  free(mesh);
}
