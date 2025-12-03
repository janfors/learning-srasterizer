#ifndef _MODEL_LOAD_H
#define _MODEL_LOAD_H

#include <rmath.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  size_t posCount;
  size_t posCap;
  Vec3f *pos;

  size_t normalCount;
  size_t normalCap;
  Vec3f *normals;

  size_t uvCount;
  size_t uvCap;
  Vec2f *uv;

  size_t indexCount;
  size_t indexCap;
  int *vertexIndices;
  int *normalIndices;
  int *uvIndices;
} ModelData;

// returns a heap allocated ModelData
ModelData *modelDataInit(int iniPosCap, int iniNormalCap, int iniUvCap, int iniIdCap);
void freeModelData(ModelData *modelData);
void parseObj(ModelData *data, const char *name);

#define GROW_FIELD(typeSize, capacity, field) realloc((field), (typeSize) * (capacity) * 2)

#endif // !_MODEL_LOAD_H
