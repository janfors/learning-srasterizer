#include <engine.h>
#include <rasterizer.h>
#include <time.h>

Engine *engineInit(Engine *engine, int width, int height, const char *name) {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    fprintf(stderr, "could not init SDL2: %s\n", SDL_GetError());
    return 0;
  }

  SDL_Window *window = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                        width, height, SDL_WINDOW_SHOWN);
  if (!window) {
    fprintf(stderr, "could not make a window: %s\n", SDL_GetError());
    return 0;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer) {
    fprintf(stderr, "could not make a renderer: %s\n", SDL_GetError());
    return 0;
  }

  SDL_Texture *texture =
      SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, 1200, 800);
  if (!texture) {
    fprintf(stderr, "could not make a texture: %s\n", SDL_GetError());
    return 0;
  }

  engine->window = window;
  engine->renderer = renderer;
  engine->texture = texture;
  engine->width = width;
  engine->height = height;
  engine->running = false;

  uint32_t *pixels = calloc(width * height, sizeof(uint32_t));
  float *depthBuffer = calloc(width * height, sizeof(float));
  engine->pixelBuff = (PixelBuffer){pixels, depthBuffer, width, height};

  Scene *scene = malloc(sizeof(Scene));
  initScene(scene, 32);
  engine->scene = scene;

  return engine;
}

#define FPS_SAMPLE_COUNT 60

static double frameTimes[FPS_SAMPLE_COUNT] = {0};
static int frameIndex = 0;

struct timespec start, end;

static void updateFPS(double frameTime) {
  frameTimes[frameIndex] = frameTime;
  frameIndex = (frameIndex + 1) % FPS_SAMPLE_COUNT;

  double sum = 0;
  for (int i = 0; i < FPS_SAMPLE_COUNT; i++) {
    sum += frameTimes[i];
  }
  double avgFrameTime = sum / FPS_SAMPLE_COUNT;
  double avgFPS = 1.0 / avgFrameTime;

  printf("\rFPS: %.2f (%.2fms)", avgFPS, avgFrameTime * 1000.0);
  fflush(stdout);
}

static void handleEvents(Engine *engine) {
  SDL_Event e;

  while (SDL_PollEvent(&e) != 0) {
    switch (e.type) {
    case SDL_QUIT:
      engine->running = false;
    case SDL_KEYDOWN: {
      if (e.key.keysym.sym == SDLK_x)
        toggleWireframeMode(engine->scene);
    }
    }
  }
}

static void render(Engine *engine) {
  updatePixels(engine);

  SDL_RenderClear(engine->renderer);
  SDL_UpdateTexture(engine->texture, NULL, engine->pixelBuff.pixels,
                    engine->pixelBuff.width * sizeof(uint32_t));
  SDL_RenderCopy(engine->renderer, engine->texture, NULL, NULL);

  SDL_RenderPresent(engine->renderer);

  // SDL_Delay(10);
}

void run(Engine *engine) {
  engine->running = true;

  while (engine->running) {
    clock_gettime(CLOCK_MONOTONIC, &start);

    handleEvents(engine);
    render(engine);
    clock_gettime(CLOCK_MONOTONIC, &end);

    double frameTime = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    updateFPS(frameTime);
  }
}

void updatePixels(Engine *engine) {
  pixelsClear(&engine->pixelBuff);

  for (int i = 0; i < engine->width * engine->height; i++)
    engine->pixelBuff.depthBuffer[i] = 1e10;

  sceneRender(engine->scene, &engine->pixelBuff);
}

void engineDestroy(Engine *engine) {
  sceneDeinit(engine->scene);
  free(engine->pixelBuff.pixels);
  SDL_DestroyTexture(engine->texture);
  SDL_DestroyRenderer(engine->renderer);
  SDL_DestroyWindow(engine->window);
  SDL_Quit();
}
