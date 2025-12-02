#ifndef _ENGINE_H
#define _ENGINE_H

#include <SDL2/SDL.h>
#include <pixelbuffer.h>
#include <scene.h>
#include <stdbool.h>

typedef struct {
  int width;
  int height;

  PixelBuffer pixelBuff;
  Scene *scene;

  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *texture;

  bool running;
} Engine;

// Sets up the engine struct with all the required fields and initalizes the pixel array
// to all 0 running is initialy set to false
// returns:
// *engine on success and 0 on failure
Engine *engineInit(Engine *engine, int width, int height, const char *name);

// calls the actual rasterizer
void updatePixels(Engine *engine);

// initiates the main loop of the program
void run(Engine *engine);

// frees all allocated memory by the engine and deinitializes SDL
void engineDestroy(Engine *engine);

// converts 4 uint8_t -> uint32_t
static inline uint32_t rgbu32(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  return (uint32_t)(a << 24 | r << 16 | g << 8 | b);
}

#endif // !_ENGINE_H
