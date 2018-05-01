/*
 * Shape3D.h extends Drawable and provides a class for drawing a 3D Shape.
 */

#ifndef SHAPE3D_H_
#define SHAPE3D_H_

#include "Drawable.h"       // For extending our Drawable object

namespace tsgl {

  /*! \class Shape3D //TODO
   */
class Shape3D : public Drawable {
protected:
    ColorFloat color; ///< Color of the Shape
 public:

    /*!
     * \brief Explicitly constructs a new Shape.
     * \details Explicit constructor for a Convex Shape object.
     *   \param numVertices the number of vertices the complete Shape will have.
     *   \param c The color of the Shape.
     * \warning An invariant is held where if numVertices is less than 2 then an error message is given.
     * \return A new Shape with a buffer for storing the specified numbered of vertices.
     */
    Shape3D(int numVertices, const ColorFloat& c) : Drawable(numVertices) {
      attribMutex.lock();
      color = c;
      attribMutex.unlock();
    };

    /*!
     * \brief Destroys a Shape object.
     * \details Destructor for a Shape.
     * \details Frees up memory that was allocated to a Shape object.
     */
    virtual ~Shape3D() { return; }

    /*!
     * \brief Adds another vertex to a Shape.
     * \details This function initializes the next vertex in the Shape.
     *   \param x The x position of the vertex.
     *   \param y The y position of the vertex.
     *   \param z The z position of the vertex.
     * \note This function does nothing if the vertex buffer is already full.
     * \note A message is given indicating when the vertex buffer is full.
     */
    virtual void addVertex(float x, float y, float z) {
      addVertexWithRGBA(x, y, z, color.R, color.G, color.B, color.A);
    }

    /**
     * \brief Accessor for the current color of the Shape.
     * \return The ColorFloat of the Shape.
     */
    virtual ColorFloat getColor() {
      attribMutex.lock();
      ColorFloat c = color;
      attribMutex.unlock();
      return c;
    }

    /**
     * \brief Sets the Shape to a new color.
     * \param c The new ColorFloat.
     */
    virtual void setColor(const ColorFloat& c) {
      attribMutex.lock();
      color = c;
      attribMutex.unlock();
      setColorForAllVertices(c);
    }

    /**
     * \brief Moves the Shape to new coordinates.
     * \param x The new center x coordinate.
     * \param y The new center y coordinate.
     * \param z The new center z coordinate.
     */
    virtual void setCenter(float x, float y, float z) {
      glm::vec3 trans = getTranslation();
      translate(-trans.x, -trans.y, -trans.z);
      translate(x, y, z);
    }

    /**
     * \brief Returns the x coordinate of the Shape.
     * \return A float, the center x coordinate.
     */
    virtual float getX() {
      return getTranslation().x;
    }

    /**
     * \brief Returns the y coordinate of the Shape.
     * \return A float, the center y coordinate.
     */
    virtual float getY() {
      return getTranslation().y;
    }

    /**
     * \brief Returns the z coordinate of the Shape.
     * \return A float, the center z coordinate.
     */
    virtual float getZ() {
      return getTranslation().z;
    }

    /**
     * \brief Rotates the Shape by an angle.
     * \details Rotates clockwise around the center of the shape.
     * \param angle Angle to rotate by, in radians.
     */
    virtual void centeredRotation(float angle) {
      fprintf(stderr, "Tried to use centeredRotation in Shape3D.%s\n");
    };
};

}

#endif /* SHAPE3D_H_ */
