#include <engine.h>
#include <modelLoad.h>

static void sceneSetup(Engine *engine) {
  engine->scene->camera = initCamera((Vec3f){0, 0, 5},  // eye
                                     (Vec3f){0, 0, -5}, // target
                                     (Vec3f){0, 1, 0},  // up
                                     M_PI / 2.0f,       // fov 90 deg
                                     (float)engine->pixelBuff.width / engine->pixelBuff.height,
                                     0.1f, // near
                                     100.0f);

  ModelData *fish = modelDataInit(64, 64, 64, 16);
  parseObj(fish, "fish.obj");
  Mesh *fishMesh = initMesh(rgbu32(110, 200, 130, 255), 64, 64);
  meshFromModel(fishMesh, fish);
  sceneAddMesh(engine->scene, fishMesh);

  freeModelData(fish);
}

int main() {
  Engine engine;
  engineInit(&engine, 1200, 800, "Software Rasterizer");

  sceneSetup(&engine);

  run(&engine);
  engineDestroy(&engine);
  return 0;
}
