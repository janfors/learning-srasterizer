#ifndef _SCENE_H
#define _SCENE_H

#include <math.h>
#include <pixelbuffer.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
  Vec2i v1, v2, v3;
  uint32_t color;
} Triangle2D;

typedef struct {
  Triangle2D *triangles;
  size_t count, capacity;
  bool wireframeMode;
} Scene;

// toggle using the wireframe
static inline void toggleWireframeMode(Scene *scene) {
  scene->wireframeMode = !scene->wireframeMode;
}

// returns a Triangle2D with the specified values
static inline Triangle2D createTriangle2D(Vec2i a, Vec2i b, Vec2i c, uint32_t color) {
  return (Triangle2D){
      .v1 = a,
      .v2 = b,
      .v3 = c,
      .color = color,
  };
}

// Initializes the scene with the dynamic array starting at initialCapacity
void initScene(Scene *scene, size_t initialCapacity);

// Adds the triangle to the list of scene triangles
void sceneAddTriangle2D(Scene *scene, Triangle2D tri);

// remove the triangle stored at idx
void sceneRemoveTriangle2D(Scene *scene, int idx);

// Make the actual draw calls on the objects within the scene
void sceneRender(Scene *scene, PixelBuffer *pixelBuffer);

// Frees all the allocated memory
void sceneDeinit(Scene *scene);

#endif // !_SCENE_H
