#include <triangle.h>

BoundingBox getTriangleBoundingBox(Vec2i *v1, Vec2i *v2, Vec2i *v3) {
  return (BoundingBox){
      .minX = mini(v1->x, mini(v2->x, v3->x)),
      .maxX = maxi(v1->x, maxi(v2->x, v3->x)),
      .minY = mini(v1->y, mini(v2->y, v3->y)),
      .maxY = maxi(v1->y, maxi(v2->y, v3->y)),
  };
}
