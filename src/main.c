#include <engine.h>

#include <time.h>

static void sceneSetup(Engine *engine) {
  srand(time(NULL));
  int numTriangles = 1000;

  for (int i = 0; i < numTriangles; i++) {
    int cx = rand() % 1200;
    int cy = rand() % 800;

    int size = 20 + rand() % 80;

    Vec2i v0 = {cx + (rand() % size) - size / 2, cy + (rand() % size) - size / 2};
    Vec2i v1 = {cx + (rand() % size) - size / 2, cy + (rand() % size) - size / 2};
    Vec2i v2 = {cx + (rand() % size) - size / 2, cy + (rand() % size) - size / 2};

    uint32_t color = rgbu32(rand() % 256, rand() % 256, rand() % 256, 255);
    Triangle2D tri = createTriangle2D(v0, v1, v2, color);
    sceneAddTriangle2D(engine->scene, tri);
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
