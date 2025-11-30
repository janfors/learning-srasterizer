#include <engine.h>

#include <time.h>

static void sceneSetup(Engine *engine) {
  srand(time(NULL));
  int numTriangles = 1000;

  for (int i = 0; i < numTriangles; i++) {
    // Random center
    float cx = rand() % 1200;
    float cy = rand() % 800;

    // Small triangle size (20-100 pixels)
    float size = 20 + rand() % 80;

    // Generate triangle vertices near the center
    Vec3f v0 = {
        cx + (rand() % (int)size) - size / 2.0f, cy + (rand() % (int)size) - size / 2.0f,
        (float)(rand() % 1000) // Random depth 0-1000
    };
    Vec3f v1 = {cx + (rand() % (int)size) - size / 2.0f, cy + (rand() % (int)size) - size / 2.0f,
                (float)(rand() % 1000)};
    Vec3f v2 = {cx + (rand() % (int)size) - size / 2.0f, cy + (rand() % (int)size) - size / 2.0f,
                (float)(rand() % 1000)};

    uint32_t color = rgbu32(rand() % 256, rand() % 256, rand() % 256, 255);
    Triangle3D tri = createTriangle3D(v0, v1, v2, color);
    sceneAddTriangle3D(engine->scene, tri);
  }
}

int main() {
  Engine engine;
  engineInit(&engine, 1200, 800, "Software Rasterizer");

  sceneSetup(&engine);

  run(&engine);
  engineDestroy(&engine);
  return 0;
}
