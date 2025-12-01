#include <stdbool.h>
#include <triangle.h>

BoundingBox getTriangleBoundingBox(int x1, int y1, int x2, int y2, int x3, int y3) {
  BoundingBox box;
  box.minX = mini(x1, mini(x2, x3));
  box.minY = mini(y1, mini(y2, y3));
  box.maxX = maxi(x1, maxi(x2, x3));
  box.maxY = maxi(y1, maxi(y2, y3));
  return box;
}
