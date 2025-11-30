#include <triangle.h>

BoundingBox getTriangleBoundingBox(Vec3f *v1, Vec3f *v2, Vec3f *v3) {
  return (BoundingBox){
      .minX = minf(v1->x, minf(v2->x, v3->x)),
      .maxX = maxf(v1->x, maxf(v2->x, v3->x)),
      .minY = minf(v1->y, minf(v2->y, v3->y)),
      .maxY = maxf(v1->y, maxf(v2->y, v3->y)),
  };
}
