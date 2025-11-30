#include <engine.h>

static Mesh *createTestCube() {
  Mesh *mesh = initMesh(rgbu32(100, 180, 129, 255), 8, 36);

  // Vertices
  addVertex(mesh, newVertex(-0.5f, -0.5f, -0.5f)); // 0
  addVertex(mesh, newVertex(0.5f, -0.5f, -0.5f));  // 1
  addVertex(mesh, newVertex(0.5f, 0.5f, -0.5f));   // 2
  addVertex(mesh, newVertex(-0.5f, 0.5f, -0.5f));  // 3
  addVertex(mesh, newVertex(-0.5f, -0.5f, 0.5f));  // 4
  addVertex(mesh, newVertex(0.5f, -0.5f, 0.5f));   // 5
  addVertex(mesh, newVertex(0.5f, 0.5f, 0.5f));    // 6
  addVertex(mesh, newVertex(-0.5f, 0.5f, 0.5f));   // 7

  // Triangles (CCW winding)
  // Front face
  addTriangle(mesh, 0, 1, 2);
  addTriangle(mesh, 0, 2, 3);
  // Back face
  addTriangle(mesh, 5, 4, 7);
  addTriangle(mesh, 5, 7, 6);
  // Left face
  addTriangle(mesh, 4, 0, 3);
  addTriangle(mesh, 4, 3, 7);
  // Right face
  addTriangle(mesh, 1, 5, 6);
  addTriangle(mesh, 1, 6, 2);
  // Top face
  addTriangle(mesh, 3, 2, 6);
  addTriangle(mesh, 3, 6, 7);
  // Bottom face
  addTriangle(mesh, 4, 5, 1);
  addTriangle(mesh, 4, 1, 0);

  return mesh;
}

static void sceneSetup(Engine *engine) {
  engine->scene->camera = initCamera((Vec3f){0, 0, 3}, // eye
                                     (Vec3f){0, 0, 0}, // target
                                     (Vec3f){0, 1, 0}, // up
                                     M_PI / 2.0f,      // fov 60 deg
                                     (float)engine->pixelBuff.width / engine->pixelBuff.height,
                                     0.1f, // near
                                     100.0f);

  Mesh *mesh = createTestCube();
  for (size_t i = 0; i < mesh->vertexCount; i++) {
    mesh->vertices[i].pos.z -= 5.0f;
  }
  sceneAddMesh(engine->scene, mesh);
}

int main() {
  Engine engine;
  engineInit(&engine, 1200, 800, "Software Rasterizer");

  sceneSetup(&engine);

  run(&engine);
  engineDestroy(&engine);
  return 0;
}
