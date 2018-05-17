/*
* Cube.h extends Object3D and provides a class for drawing a cube to a Canvas.
*/

#ifndef CUBE_H_
#define CUBE_H_

#include "Object3D.h" // For extending our Solid3D object

namespace tsgl
{

/*! \class Cube
  *  \brief Draw a simple Cube.
  */
class Cube : public Object3D
{
public:
  
  Cube(float x, float y, float z, float width, float height, float depth, const ColorFloat &color = BLACK) : Object3D("assets/models/cube.dae")
  {
    setColorForAllVertices(color);
    scale(width, height, depth);
    translate(x, y, z);
  }
};
}

#endif /* CUBE_H_ */
