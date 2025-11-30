#include <rasterizer.h>
#include <scene.h>
#include <stdio.h>
#include <stdlib.h>
#include <transform.h>

// to be used when count == capacity
static void growArray(Scene *scene) {
  size_t newCapacity = scene->capacity * 2;
  scene->meshes = realloc(scene->meshes, newCapacity * sizeof(Mesh));
  scene->capacity = newCapacity;
}

// to be called when used memory == capacity / 2
static void shrinkArray(Scene *scene) {
  size_t newCapacity = scene->capacity / 2;
  scene->meshes = realloc(scene->meshes, newCapacity * sizeof(Mesh));
  scene->capacity = newCapacity;
}

void initScene(Scene *scene, size_t initialCapacity) {
  scene->meshes = malloc(sizeof(Mesh) * initialCapacity);
  scene->count = 0;
  scene->capacity = initialCapacity;

  scene->wireframeMode = false;
}

void sceneAddMesh(Scene *scene, Mesh mesh) {
  if (scene->count + 1 >= scene->capacity)
    growArray(scene);

  scene->meshes[scene->count++] = mesh;
}

void sceneRemoveMesh(Scene *scene, size_t idx) {
  if (idx < 0 || idx >= scene->count) {
    fprintf(stderr, "Tried removing a triangle outside of the bounds of the triangle array!\n");
    return;
  }

  scene->meshes[idx] = scene->meshes[scene->count - 1];
  scene->count--;

  if (scene->count < scene->capacity / 2)
    shrinkArray(scene);
}

static void renderMesh(Mesh *mesh, PixelBuffer *pixelBuffer, bool wireframeMode) {
  for (int i = 0; i < mesh->indexCount; i += 3) {
    Vertex *v0 = &mesh->vertices[mesh->indices[i]];
    Vertex *v1 = &mesh->vertices[mesh->indices[i + 1]];
    Vertex *v2 = &mesh->vertices[mesh->indices[i + 2]];

    if (wireframeMode)
      drawTriangleWireframe(v0, v1, v2, mesh->color, pixelBuffer);
    else
      drawTriangleFilled(v0, v1, v2, mesh->color, pixelBuffer);
  }
}

void sceneRender(Scene *scene, PixelBuffer *pixelBuffer) {
  for (int i = 0; i < scene->count; i++) {
    Mat4f mvp = mat4fMul(scene->camera.projection, scene->camera.view);
    Mesh *transformed = transformMesh(&scene->meshes[i], mvp, pixelBuffer);
    renderMesh(transformed, pixelBuffer, scene->wireframeMode);
    freeMesh(transformed);
  }
}

void sceneDeinit(Scene *scene) {
  for (int i = 0; i < scene->count; i++) {
    freeMesh(&scene->meshes[i]);
  }

  free(scene->meshes);
}
