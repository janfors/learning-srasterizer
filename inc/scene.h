#ifndef _SCENE_H
#define _SCENE_H

#include <camera.h>
#include <mesh.h>
#include <pixelbuffer.h>
#include <rmath.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct {
  Mesh **meshes;
  size_t count, capacity;

  Camera camera;
  bool wireframeMode;
} Scene;

// toggle using the wireframe
static inline void toggleWireframeMode(Scene *scene) {
  scene->wireframeMode = !scene->wireframeMode;
}

// Add a new mesh to the scene
void sceneAddMesh(Scene *scene, Mesh *mesh);
// remove the mesh at idx
void sceneRemoveMesh(Scene *scene, size_t idx);

// Initializes the scene with the dynamic array starting at initialCapacity
void initScene(Scene *scene, size_t initialCapacity);

// Make the actual draw calls on the objects within the scene
void sceneRender(Scene *scene, PixelBuffer *pixelBuffer);

// Frees all the allocated memory
void sceneDeinit(Scene *scene);

#endif // !_SCENE_H
