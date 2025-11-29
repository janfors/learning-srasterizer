#include <engine.h>

int main() {
  Engine engine;
  engineInit(&engine, 1200, 800, "Software Rasterizer");
  run(&engine);
  engineDestroy(&engine);
  return 0;
}
