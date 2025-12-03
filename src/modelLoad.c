#include <modelLoad.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

ModelData *modelDataInit(int iniPosCap, int iniNormalCap, int iniUvCap, int iniIdCap) {
  ModelData *modelData = malloc(sizeof(ModelData));
  modelData->posCount = 0;
  modelData->posCap = iniPosCap;
  modelData->normalCount = 0;
  modelData->normalCap = iniNormalCap;
  modelData->uvCount = 0;
  modelData->uvCap = iniUvCap;

  modelData->indexCount = 0;
  modelData->indexCap = iniIdCap;

  modelData->pos = malloc(sizeof(Vec3f) * modelData->posCap);
  modelData->normals = malloc(sizeof(Vec3f) * modelData->normalCap);
  modelData->uv = malloc(sizeof(Vec2f) * modelData->uvCap);

  modelData->vertexIndices = malloc(sizeof(int) * modelData->indexCap);
  modelData->normalIndices = malloc(sizeof(int) * modelData->indexCap);
  modelData->uvIndices = malloc(sizeof(int) * modelData->indexCap);

  return modelData;
}

void freeModelData(ModelData *modelData) {
  free(modelData->pos);
  free(modelData->normals);
  free(modelData->uv);

  free(modelData->vertexIndices);
  free(modelData->normalIndices);
  free(modelData->uvIndices);

  free(modelData);
}

static inline Vec3f parse3f(const char *data, const char *format) {
  float x, y, z;
  sscanf(data, format, &x, &y, &z);
  return (Vec3f){x, y, z};
}
static inline Vec2f parseUV(const char *data) {
  float x, y;
  sscanf(data, "vt %f %f", &x, &y);
  return (Vec2f){x, y};
}

static inline void addTriangle(ModelData *model, int v1, int u1, int n1, int v2, int u2, int n2,
                               int v3, int u3, int n3) {
  model->vertexIndices[model->indexCount] = v1;
  model->normalIndices[model->indexCount] = n1;
  model->uvIndices[model->indexCount] = u1;
  model->indexCount++;

  model->vertexIndices[model->indexCount] = v2;
  model->normalIndices[model->indexCount] = n2;
  model->uvIndices[model->indexCount] = u2;
  model->indexCount++;

  model->vertexIndices[model->indexCount] = v3;
  model->normalIndices[model->indexCount] = n3;
  model->uvIndices[model->indexCount] = u3;
  model->indexCount++;
}

// totally did not have to rewrite it a couple of times...
// the original implementation had a dynamic array to handle an arbitrary polygon
// but that was a pain...
static void parseIndices(const char *line, ModelData *model) {
  const char *ptr = line + 2;

  int verts[16];
  int uvs[16];
  int norms[16];
  int count = 0;

  while (*ptr && *ptr != '\n' && count < 16) {
    int v, u, n;
    if (sscanf(ptr, "%d/%d/%d", &v, &u, &n) == 3) {
      verts[count] = v - 1;
      uvs[count] = u - 1;
      norms[count] = n - 1;
      count++;
    }

    while (*ptr && *ptr != ' ' && *ptr != '\n')
      ptr++;
    while (*ptr == ' ')
      ptr++;
  }

  for (int i = 1; i < count - 1; i++) {
    if (model->indexCount + 3 > model->indexCap) {
      model->vertexIndices = GROW_FIELD(sizeof(int), model->indexCap, model->vertexIndices);
      model->normalIndices = GROW_FIELD(sizeof(int), model->indexCap, model->normalIndices);
      model->uvIndices = GROW_FIELD(sizeof(int), model->indexCap, model->uvIndices);
      model->indexCap *= 2;
    }

    addTriangle(model, verts[0], uvs[0], norms[0], verts[i], uvs[i], norms[i], verts[i + 1],
                uvs[i + 1], norms[i + 1]);
  }
}

void parseObj(ModelData *data, const char *name) {
  char path[256];
  snprintf(path, sizeof(path), "%s%s", RES_DIR, name);

  FILE *fp = fopen(path, "r");
  if (!fp) {
    fprintf(stderr, "Failed to open file: %s\n", path);
    return;
  }

  fseek(fp, 0L, SEEK_END);
  size_t len = ftell(fp);
  rewind(fp);

  char lineBuff[256];
  while (fgets(lineBuff, sizeof(lineBuff), fp)) {
    if (strncmp(lineBuff, "v ", 2) == 0) {
      Vec3f pos = parse3f(lineBuff, "v %f %f %f");
      if (data->posCount >= data->posCap) {
        data->pos = GROW_FIELD(sizeof(Vec3f), data->posCap, data->pos);
        data->posCap *= 2;
      }

      data->pos[data->posCount++] = pos;
      continue;
    } else if (strncmp(lineBuff, "vn", 2) == 0) {
      Vec3f normal = parse3f(lineBuff, "vn %f %f %f");
      if (data->normalCount >= data->normalCap) {
        data->normals = GROW_FIELD(sizeof(Vec3f), data->normalCap, data->normals);
        data->normalCap *= 2;
      }

      data->normals[data->normalCount++] = normal;
      continue;
    } else if (strncmp(lineBuff, "vt", 2) == 0) {
      Vec2f uv = parseUV(lineBuff);
      if (data->uvCount >= data->uvCap) {
        data->uv = GROW_FIELD(sizeof(Vec2f), data->uvCap, data->uv);
        data->uvCap *= 2;
      }

      data->uv[data->uvCount++] = uv;
      continue;
    } else if (strncmp(lineBuff, "f ", 2) == 0) {
      parseIndices(lineBuff, data);
      continue;
    }
  }

  fclose(fp);
}
