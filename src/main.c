#include <engine.h>

static void sceneSetup(Engine *engine) {
  Triangle2D tri1 = createTriangle2D((Vec2i){100, 200}, (Vec2i){200, 100}, (Vec2i){200, 500},
                                     rgbu32(100, 100, 100, 255));
  Triangle2D tri2 = createTriangle2D((Vec2i){300, 240}, (Vec2i){220, 110}, (Vec2i){240, 600},
                                     rgbu32(255, 10, 50, 255));
  Triangle2D tri3 = createTriangle2D((Vec2i){10, 220}, (Vec2i){304, 100}, (Vec2i){207, 770},
                                     rgbu32(255, 255, 100, 255));
  Triangle2D tri4 = createTriangle2D((Vec2i){122, 300}, (Vec2i){213, 102}, (Vec2i){240, 40},
                                     rgbu32(10, 10, 100, 255));

  sceneAddTriangle2D(engine->scene, tri1);
  sceneAddTriangle2D(engine->scene, tri2);
  sceneAddTriangle2D(engine->scene, tri3);
  sceneAddTriangle2D(engine->scene, tri4);
}

int main() {
  Engine engine;
  engineInit(&engine, 1200, 800, "Software Rasterizer");

  sceneSetup(&engine);

  run(&engine);
  engineDestroy(&engine);
  return 0;
}
