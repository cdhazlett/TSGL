#include "Ellipse.h"

namespace tsgl
{

Ellipse::Ellipse(float x, float y, float xRadius, float yRadius, const ColorFloat &color = BLACK, const ColorFloat &outlineColor = BLACK)
    : ConvexPolygon(360*3 / 2, color, outlineColor)
{
  //TODO: do we need any locking here? All the values we use below are from the constructor
  float sliceLength = 2.0f / (getNumberOfVertices()/3) * PI;
  for (int i = 0; i < getNumberOfVertices()/3; ++i) {
    addVertex(.5+0,.5+0);
    addVertex(.5+cos((i+1) * sliceLength)/2.f, .5+sin((i+1) * sliceLength)/2.f);
    addVertex(.5+cos(i * sliceLength)/2.f, .5+sin(i * sliceLength)/2.f);
  }
    

  scale(xRadius, yRadius, 1);
  translate(x, y, 0);
}

} // namespace tsgl
