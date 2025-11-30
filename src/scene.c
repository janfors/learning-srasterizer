#include <rasterizer.h>
#include <scene.h>

// to be used when count == capacity
static void growArray(Scene *scene) {
  size_t newCapacity = scene->capacity * 2;
  scene->triangles = realloc(scene->triangles, newCapacity * sizeof(Triangle3D));
  scene->capacity = newCapacity;
}

// to be called when used memory == capacity / 2
static void shrinkArray(Scene *scene) {
  size_t newCapacity = scene->capacity / 2;
  scene->triangles = realloc(scene->triangles, newCapacity * sizeof(Triangle3D));
  scene->capacity = newCapacity;
}

void initScene(Scene *scene, size_t initialCapacity) {
  scene->triangles = malloc(sizeof(Triangle3D) * initialCapacity);
  scene->count = 0;
  scene->capacity = initialCapacity;

  scene->wireframeMode = false;
}

void sceneAddTriangle3D(Scene *scene, Triangle3D tri) {
  if (scene->count + 1 >= scene->capacity)
    growArray(scene);

  scene->triangles[scene->count++] = tri;
}

void sceneRemoveTriangle3D(Scene *scene, int idx) {
  if (idx < 0 || idx >= scene->count) {
    fprintf(stderr, "Tried removing a triangle outside of the bounds of the triangle array!\n");
    return;
  }

  scene->triangles[idx] = scene->triangles[scene->count - 1];
  scene->count--;

  if (scene->count < scene->capacity / 2)
    shrinkArray(scene);
}

void sceneRender(Scene *scene, PixelBuffer *pixelBuffer) {
  for (int i = 0; i < scene->count; i++) {
    Vec3f *a = &scene->triangles[i].v1;
    Vec3f *b = &scene->triangles[i].v2;
    Vec3f *c = &scene->triangles[i].v3;
    uint32_t color = scene->triangles[i].color;

    if (scene->wireframeMode)
      drawTriangleWireframe(a, b, c, color, pixelBuffer);
    else
      drawTriangleFilled(a, b, c, color, pixelBuffer);
  }
}

void sceneDeinit(Scene *scene) { free(scene->triangles); }
