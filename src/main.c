#include <engine.h>

static void sceneSetup(Engine *engine) {
  engine->scene->camera = initCamera((Vec3f){0, 0, 3}, // position
                                     (Vec3f){0, 0, 0}, // target
                                     (Vec3f){0, 1, 0}, // up
                                     M_PI / 3.0f,      // fov 60 deg
                                     (float)engine->pixelBuff.width / engine->pixelBuff.height,
                                     0.1f, // near
                                     100.0f);

  Mesh *mesh = initMesh(0xFF00FFFF, 3, 3);

  addVertex(mesh, newVertex(0, 1, 0));
  addVertex(mesh, newVertex(-1, -1, 0));
  addVertex(mesh, newVertex(1, -1, 0));
  addTriangle(mesh, 0, 1, 2);

  sceneAddMesh(engine->scene, *mesh);
}

int main() {
  Engine engine;
  engineInit(&engine, 1200, 800, "Software Rasterizer");

  sceneSetup(&engine);

  run(&engine);
  engineDestroy(&engine);
  return 0;
}
