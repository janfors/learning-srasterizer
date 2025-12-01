#include <rasterizer.h>
#include <scene.h>
#include <stdio.h>
#include <stdlib.h>
#include <transform.h>

// to be used when count == capacity
static void growArray(Scene *scene) {
  size_t newCapacity = scene->capacity * 2;
  scene->meshes = realloc(scene->meshes, newCapacity * sizeof(Mesh *));
  scene->capacity = newCapacity;
}

// to be called when used memory == capacity / 2
static void shrinkArray(Scene *scene) {
  size_t newCapacity = scene->capacity / 2;
  scene->meshes = realloc(scene->meshes, newCapacity * sizeof(Mesh *));
  scene->capacity = newCapacity;
}

void initScene(Scene *scene, size_t initialCapacity) {
  scene->meshes = malloc(sizeof(Mesh *) * initialCapacity);
  scene->count = 0;
  scene->capacity = initialCapacity;

  scene->wireframeMode = false;
}

void sceneAddMesh(Scene *scene, Mesh *mesh) {
  if (scene->count + 1 >= scene->capacity)
    growArray(scene);

  scene->meshes[scene->count++] = mesh;
}

void sceneRemoveMesh(Scene *scene, size_t idx) {
  if (idx < 0 || idx >= scene->count) {
    fprintf(stderr, "Tried removing a mesh outside of the bounds of the mesh array!\n");
    return;
  }

  scene->meshes[idx] = scene->meshes[scene->count - 1];
  scene->count--;

  if (scene->count < scene->capacity / 2)
    shrinkArray(scene);
}

// this really should be in mesh.h
static void renderMesh(Scene *scene, Mesh *mesh, PixelBuffer *pixelBuffer, bool wireframeMode) {
  for (int i = 0; i < mesh->indexCount; i += 3) {
    Vertex tri[3] = {
        mesh->vertices[mesh->indices[i]],
        mesh->vertices[mesh->indices[i + 1]],
        mesh->vertices[mesh->indices[i + 2]],
    };

    Vertex clipped[9]; // should be 6 but breaks otherwise???
    int n = clipTriangleAllPlanes(tri, clipped, pixelBuffer);
    if (n < 3)
      continue;

    for (int j = 1; j < n - 1; j++) {
      if (wireframeMode)
        drawTriangleWireframe(&clipped[0], &clipped[j], &clipped[j + 1], mesh->color, pixelBuffer);
      else
        drawTriangleFilled(&clipped[0], &clipped[j], &clipped[j + 1], mesh->color, pixelBuffer);
    }
  }
}

void sceneRender(Scene *scene, PixelBuffer *pixelBuffer) {
  for (int i = 0; i < scene->count; i++) {
    Mat4f mvp = mat4fMul(scene->camera.projection, scene->camera.view);
    Mesh *transformed = transformMeshToClipSpace(scene->meshes[i], mvp, pixelBuffer);

    renderMesh(scene, transformed, pixelBuffer, scene->wireframeMode);
    freeMesh(transformed);
  }
}

void sceneDeinit(Scene *scene) {
  for (int i = 0; i < scene->count; i++) {
    freeMesh(scene->meshes[i]);
  }

  free(scene->meshes);
}
