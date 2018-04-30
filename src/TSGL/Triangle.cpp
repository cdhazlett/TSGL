#include "Triangle.h"
#include <algorithm>

namespace tsgl
{

Triangle::Triangle(float x, float y, float width, float height, float pointLocation, const ColorFloat &color = BLACK, const ColorFloat &outlineColor = BLACK) : ConvexPolygon(3, color, outlineColor)
{
  addVertex(0,1);
  addVertex(1,1);
  addVertex(pointLocation, 0);

  scale(width, height, 1);
  translate(x, y, 0);
}

Triangle::Triangle(float x1, float y1, float x2, float y2, float x3, float y3, const ColorFloat &color, const ColorFloat &outlineColor) : ConvexPolygon(3, color, outlineColor)
{
  addVertex(x1, y1);
  addVertex(x2, y2);
  addVertex(x3, y3);
}

} // namespace tsgl
