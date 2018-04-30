#include "Cube.h"

namespace tsgl {

  Cube::Cube(float x, float y, float z, float width, float height, float depth, const ColorFloat &color, const ColorFloat& outlineColor) : Solid3D(24, color, outlineColor) {

    // front
    addVertex(x, y, z);
    addVertex(x, y+height, z);
    addVertex(x+width, y+height, z);

    addVertex(x, y, z);
    addVertex(x+width, y, z);
    addVertex(x+width, y+height, z);

    // back
    addVertex(x, y, z+depth);
    addVertex(x, y+height, z+depth);
    addVertex(x+width, y+height, z+depth);

    addVertex(x, y, z+depth);
    addVertex(x+width, y, z+depth);
    addVertex(x+width, y+height, z+depth);

    // left side
    addVertex(x, y, z);
    addVertex(x, y, z+depth);
    addVertex(x, y+height, z+depth);

    addVertex(x, y, z);
    addVertex(x, y+height, z);
    addVertex(x, y+height, z+depth);

    // right side
    addVertex(x+width, y, z);
    addVertex(x+width, y, z+depth);
    addVertex(x+width, y+height, z+depth);

    addVertex(x+width, y, z);
    addVertex(x+width, y+height, z);
    addVertex(x+width, y+height, z+depth);

    // top
    addVertex(x, y+height, z);
    addVertex(x+width, y+height, z);
    addVertex(x+width, y+height, z+depth);

    addVertex(x, y+height, z);
    addVertex(x, y+height, z+depth);
    addVertex(x+width, y+height, z+depth);

    // bottom
    addVertex(x, y, z);
    addVertex(x+width, y, z);
    addVertex(x+width, y, z+depth);

    addVertex(x, y, z);
    addVertex(x, y, z+depth);
    addVertex(x+width, y, z+depth);
}

  void Cube::render() {


    // give opengl some info about the array of point we are about to pass into it
    glVertexPointer(
      3,  // how many points per vertex (for us, that's x,y,z)
      GL_FLOAT, // the type of data being passed
      0, // byte offset between vertices
      getPointerToVerticesArray()  // pointer to the array of vertices
    );
    // set the color of the object
    glColor4f(
      getObjectColor()->R,
      getObjectColor()->G,
      getObjectColor()->B,
      getObjectColor()->A
    );
    // draw the array of vertices
    glDrawArrays(
      GL_TRIANGLES,
      0, // The starting index of the array
      getNumberOfVertices() // The number of vertices from the object
    );


  }

}
