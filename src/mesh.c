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

// Will figure something out later
static size_t addUniqueVertex(Mesh *mesh, Vec3f pos, Vec3f normal, Vec2f uv) {
  for (size_t i = 0; i < mesh->vertexCount; i++) {
    Vertex v = mesh->vertices[i];

    if (memcmp(&v.pos, &pos, sizeof(Vec3f)) == 0 &&
        memcmp(&v.normals, &normal, sizeof(Vec3f)) == 0 && memcmp(&v.uv, &uv, sizeof(Vec2f)) == 0) {
      return i;
    }
  }

  Vertex v;
  v.pos = pos;
  v.normals = normal;
  v.uv = uv;
  addVertex(mesh, v);
  return mesh->vertexCount - 1;
}

void meshFromModel(Mesh *mesh, ModelData *data) {
  if (mesh->indexCapacity < data->indexCount) {
    mesh->indexCapacity = data->indexCount;
    mesh->indices = realloc(mesh->indices, mesh->indexCapacity * sizeof(size_t));
  }

  for (int i = 0; i < data->indexCount; i++) {
    size_t posIdx = data->vertexIndices[i];
    size_t uvIdx = data->uvIndices[i];
    size_t normalsIdx = data->normalIndices[i];

    Vec3f pos = data->pos[posIdx];
    Vec3f normals = data->normals[normalsIdx];
    Vec2f uv = data->uv[uvIdx];

    Vertex v = newVertex(pos.x, pos.y, pos.z);
    v.uv = uv;
    v.normals = normals;

    addVertex(mesh, v);
    mesh->indices[mesh->indexCount++] = mesh->vertexCount - 1;
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
