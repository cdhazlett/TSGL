#include "Rectangle.h"

namespace tsgl {

  Rectangle::Rectangle(float x, float y, float width, float height, const ColorFloat &color, const ColorFloat& outlineColor) : ConvexPolygon(6, color, outlineColor) {
    addVertex(0,0);
    addVertex(0,1);
    addVertex(1,1);
    addVertex(1,1);
    addVertex(1,0);
    addVertex(0,0);

    // translate(width/2, height/2, 0);
    scale(width, height, 1);
    translate(x, y, 0);
}

}
