/*
 * Cube.h extends Solid3D and provides a class for drawing a cube to a Canvas.
 */

#ifndef CUBE_H_
#define CUBE_H_

#include "Solid3D.h"  // For extending our Solid3D object

namespace tsgl {

/*! \class Cube
 *  \brief Draw a simple Cube.
 *  \details Cube is a class for holding vertex data for a simple Cube.
 */
class Cube : public Solid3D {
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
    Cube(float x, float y, float z, float width, float height, float depth, const ColorFloat &color = BLACK, const ColorFloat& outlineColor = BLACK);

    GLenum getGeometryType() { return GL_QUADS; }

    void render();

};

}

#endif /* CUBE_H_ */
