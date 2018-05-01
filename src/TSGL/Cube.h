/*
* Cube.h extends Solid3D and provides a class for drawing a cube to a Canvas.
*/

#ifndef CUBE_H_
#define CUBE_H_

#include "Shape3D.h"  // For extending our Solid3D object

namespace tsgl {

  /*! \class Cube
  *  \brief Draw a simple Cube.
  *  \details Cube is a class for holding vertex data for a simple Cube.
  */
  class Cube : public Shape3D {
  public:

    /*!
    * \brief Explicitly constructs a Rectangle. //TODO
    * \details This is the constructor for the Rectangle class.
    *   \param x The x coordinate of the Rectangle's left edge.
    *   \param y The y coordinate of the Rectangle's top edge.
    *   \param width The width of the Rectangle.
    *   \param height The height of the Rectangle.
    *   \param color The color of the Rectangle. (Defaults to BLACK)
    *   \param outlineColor The color of the Rectangle's outline. (Defaults to BLACK)
    * \return A new Rectangle with the specified top left corner, dimensions, and color.
    */
    Cube(float x, float y, float z, float width, float height, float depth, const ColorFloat &color = BLACK) : Shape3D(36, color) {

      addVertex(-1.0f,-1.0f,-1.0f);
      addVertex(-1.0f,-1.0f, 1.0f);
      addVertex(-1.0f, 1.0f, 1.0f);
      addVertex(1.0f, 1.0f,-1.0f);
      addVertex(-1.0f,-1.0f,-1.0f);
      addVertex(-1.0f, 1.0f,-1.0f);
      addVertex(1.0f,-1.0f, 1.0f);
      addVertex(-1.0f,-1.0f,-1.0f);
      addVertex(1.0f,-1.0f,-1.0f);
      addVertex(1.0f, 1.0f,-1.0f);
      addVertex(1.0f,-1.0f,-1.0f);
      addVertex(-1.0f,-1.0f,-1.0f);
      addVertex(-1.0f,-1.0f,-1.0f);
      addVertex(-1.0f, 1.0f, 1.0f);
      addVertex(-1.0f, 1.0f,-1.0f);
      addVertex(1.0f,-1.0f, 1.0f);
      addVertex(-1.0f,-1.0f, 1.0f);
      addVertex(-1.0f,-1.0f,-1.0f);
      addVertex(-1.0f, 1.0f, 1.0f);
      addVertex(-1.0f,-1.0f, 1.0f);
      addVertex(1.0f,-1.0f, 1.0f);
      addVertex(1.0f, 1.0f, 1.0f);
      addVertex(1.0f,-1.0f,-1.0f);
      addVertex(1.0f, 1.0f,-1.0f);
      addVertex(1.0f,-1.0f,-1.0f);
      addVertex(1.0f, 1.0f, 1.0f);
      addVertex(1.0f,-1.0f, 1.0f);
      addVertex(1.0f, 1.0f, 1.0f);
      addVertex(1.0f, 1.0f,-1.0f);
      addVertex(-1.0f, 1.0f,-1.0f);
      addVertex(1.0f, 1.0f, 1.0f);
      addVertex(-1.0f, 1.0f,-1.0f);
      addVertex(-1.0f, 1.0f, 1.0f);
      addVertex(1.0f, 1.0f, 1.0f);
      addVertex(-1.0f, 1.0f, 1.0f);
      addVertex(1.0f,-1.0f, 1.0f);

    }

    void render() {

    }

    void render(GLuint &vertexbuffer, GLuint &colorbuffer, GLuint &normalbuffer) {

      // Copy over the vertex position data
      glEnableVertexAttribArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer); // & ?
      glVertexAttribPointer(
        0,            // attribute 0
        3,            // Size per vertex (eg. X, Y, Z values for each)
        GL_DOUBLE,    // type
        GL_FALSE,     // normalized
        0,            // stride
        (void*)0      // array buffer offset
      );
      glBufferData(GL_ARRAY_BUFFER, sizeof(double)*numVertices*3, vertexArray, GL_DYNAMIC_DRAW);

      // Copy over the vertex color data
      glEnableVertexAttribArray(1);
      glBindBuffer(GL_ARRAY_BUFFER, colorbuffer); // & ?
      glVertexAttribPointer(
        1,            // attribute 1
        3,            // Size per vertex (eg. R, G, B, A values for each)
        GL_FLOAT,     // type
        GL_FALSE,     // normalized
        0,            // stride
        (void*)0      // array buffer offset
      );
      glBufferData(GL_ARRAY_BUFFER, sizeof(float)*numVertices*4, vertexColorArray, GL_DYNAMIC_DRAW);

      // Copy over the vertex normal data
      glEnableVertexAttribArray(2);
      glBindBuffer(GL_ARRAY_BUFFER, normalbuffer); // & ?
      glVertexAttribPointer(
        2,            // attribute 2
        3,            // Size per vertex (eg. X, Y, Z values for each)
        GL_DOUBLE,    // type
        GL_FALSE,     // normalized
        0,            // stride
        (void*)0      // array buffer offset
      );
      glBufferData(GL_ARRAY_BUFFER, sizeof(double)*numVertices*3, vertexNormalArray, GL_DYNAMIC_DRAW);

      // Draw the object
      glDrawArrays(GL_TRIANGLES, 0, getNumberOfVertices());

      glDisableVertexAttribArray(0);
      glDisableVertexAttribArray(1);
      glDisableVertexAttribArray(2);


    }

  };

}

#endif /* CUBE_H_ */
