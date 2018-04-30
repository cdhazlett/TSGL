/*
* Drawable.h provides a base class from which to extend other drawable shapes.
*/

#ifndef DRAWABLE_H_
#define DRAWABLE_H_

// GLM Library
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtx/decomposition.hpp>

#include <../glad/glad.h>       // Loader for OpenGL function calls
#include <mutex>                // Needed for locking the attribute mutex for thread-safety
#include "Color.h"              // TSGL Color Definitions

namespace tsgl {

  /*! \class Drawable
  *  \brief A class for drawing onto a Canvas or CartesianCanvas.
  *  \details Drawable provides a base class for drawing a shape, text, or image to a Canvas or CartesianCanvas.
  *  \note Drawable is abstract and must be extended.
  */
  class Drawable {
  protected:
    std::mutex      attribMutex;  ///< Protects the attributes of the Drawable from being accessed while simultaneously being changed
    glm::mat4       modelMatrix;  ///< The transformation matrix for this model
    double          *vertexArray; ///< The array which holds (x,y,z) values for each vertex
    double          *vertexColorArray; ///< The array which holds (x,y,z) values for each vertex
    int             numVertices = 0;  ///< The number of vertices in the model
    int             currentVertex = 0;  ///< The current number of initted vertices

  public:

    /*!
    * \brief Constructs a new Drawable.
    * \warning <b>You <i>must</i> inherit the parent's constructor if you are extending Drawable.</b>
    * \note Refer to the Drawable class description for more details.
    */
    Drawable(int numberOfVertices) {
      attribMutex.lock();

      if (numberOfVertices < 1) {
        fprintf(stderr, "Cannot have less than one vertex in a Drawable!\n");
      }

      numVertices = numberOfVertices;
      currentVertex = 0;
      vertexArray = new double[numVertices*3];
      memset(vertexArray, 0, sizeof(double)*numVertices*3);
      vertexColorArray = new double[numVertices*4];
      memset(vertexColorArray, 0, sizeof(double)*numVertices*4);

      modelMatrix = glm::mat4(1.0f);

      attribMutex.unlock();
    }

    /*!
    * \brief Destroys the Drawable.
    * \details Destructor for a Drawable.
    * \details Frees up memory that was allocated to a Drawable object.
    */
    virtual ~Drawable() {
      attribMutex.lock(); //TODO: decide if we need this. Is it necessary so the item isn't destroyed when another thread is using?
      delete[] vertexArray;
      delete[] vertexColorArray;
      attribMutex.unlock();
    }

    /**
    * \brief Rotates an object
    */
    void rotate(float degrees, float x, float y, float z) {
      attribMutex.lock();
      modelMatrix = glm::rotate(modelMatrix, degrees, x, y, z);
      attribMutex.unlock();
    }

    /**
    * \brief Scale an object
    */
    void scale(float x, float y, float z) {
      attribMutex.lock();
      modelMatrix = glm::scale(modelMatrix, glm::vec3(x, y, z));
      attribMutex.unlock();
    }

    /**
    * \brief Translate an object
    */
    void translate(float x, float y, float z) {
      attribMutex.lock();
      modelMatrix = glm::translate(modelMatrix, glm::vec3(x, y, z));
      attribMutex.unlock();
    }

    void setColorForAllVertices(const ColorFloat& color) {
      attribMutex.lock()
      int i;
      for(i=0; i<currentVertex; i++) {
        vertexColorArray[i*4] = color.R;
        vertexColorArray[i*4 +1] = color.G;
        vertexColorArray[i*4 +2] = color.B;
        vertexColorArray[i*4 +3] = color.A;
      }
      attribMutex.unlock()
    }

    int getNumberOfVertices() {
      attribMutex.lock();
      int retVal = numVertices;
      attribMutex.unlock();
      return retVal;
    }

    /**
    * \brief Returns a pointer to the array of vertices
    */
    const double* getVertexArrayPointer() {
      attribMutex.lock();
      double* retVal = vertexArray;
      attribMutex.unlock();
      return retVal;
    }

    /**
    * \brief Returns a pointer to the array of colors
    */
    const double* getVertexColorArrayPointer() {
      attribMutex.lock();
      double* retVal = vertexColorArray;
      attribMutex.unlock();
      return retVal;
    }

    /*!
    * \brief Adds another vertex to a Drawable.
    * \details This function initializes the next vertex in the Drawable.
    */
    virtual void addVertex(float x, float y, float z, ColorFloat color) {
      attribMutex.lock();

      if (currentVertex == numVertices) {
        fprintf(stderr, "This Drawable class is full at %d vertices!\n", numVertices);
        attribMutex.unlock();
        return;
      }

      vertexArray[currentVertex*3] = x;
      vertexArray[currentVertex*3 +1] = y;
      vertexArray[currentVertex*3 +2] = z;

      vertexColorArray[currentVertex*4] = color.R;
      vertexColorArray[currentVertex*4 +1] = color.G;
      vertexColorArray[currentVertex*4 +2] = color.B;
      vertexColorArray[currentVertex*4 +3] = color.A;

      currentVertex++;

      attribMutex.unlock();
    }

    void changeCapacity(int newNumVertices) {
      attribMutex.lock();
      double *oldVertexArray = vertexArray;
      double *oldVertexColorArray = vertexColorArray;

      vertexArray = new double[newNumVertices*3];
      memset(vertexArray, 0, sizeof(double)*newNumVertices*3);
      vertexColorArray = new double[newNumVertices*4];
      memset(vertexColorArray, 0, sizeof(double)*newNumVertices*4);

      memcpy(vertexArray, oldVertexArray, sizeof(double)*min(numVertices, newNumVertices)*3);
      memcpy(vertexColorArray, oldVertexColorArray, sizeof(double)*min(numVertices, newNumVertices)*4);

      numVertices = newNumVertices;
      currentVertex = min(currentVertex, newNumVertices);

      delete[] oldVertexArray;
      delete[] oldVertexColorArray;
      attribMutex.unlock();
    }

    glm::vec3 getTranslation() {
      // decompose (tmat4x4< T, P > const &modelMatrix, tvec3< T, P > &scale, tquat< T, P > &orientation, tvec3< T, P > &translation, tvec3< T, P > &skew, tvec4< T, P > &perspective)

      glm::vec3 scale;
      glm::quat orientation;
      glm::vec3 translation;
      glm::vec3 skew;
      glm::vec4 perspective;

      attribMutex.lock();
      glm::decompose(modelMatrix, scale, orientation, translation, skew, perspective);
      attribMutex.unlock();

      return translation;
    }

    glm::quat getRotation() {
      // decompose (tmat4x4< T, P > const &modelMatrix, tvec3< T, P > &scale, tquat< T, P > &orientation, tvec3< T, P > &translation, tvec3< T, P > &skew, tvec4< T, P > &perspective)

      glm::vec3 scale;
      glm::quat orientation;
      glm::vec3 translation;
      glm::vec3 skew;
      glm::vec4 perspective;

      attribMutex.lock();
      glm::decompose(modelMatrix, scale, orientation, translation, skew, perspective);
      attribMutex.unlock();

      return orientation;
    }

    glm::vec3 getScale() {
      // decompose (tmat4x4< T, P > const &modelMatrix, tvec3< T, P > &scale, tquat< T, P > &orientation, tvec3< T, P > &translation, tvec3< T, P > &skew, tvec4< T, P > &perspective)

      glm::vec3 scale;
      glm::quat orientation;
      glm::vec3 translation;
      glm::vec3 skew;
      glm::vec4 perspective;

      attribMutex.lock();
      glm::decompose(modelMatrix, scale, orientation, translation, skew, perspective);
      attribMutex.unlock();

      return scale;
    }

    // /**
    // * \brief Accessor for the rotation details of the Drawable
    // */
    // void getRotation(float &deg, float &x, float &y) {
    //   // TODO Implement this thing
    //   fprintf (stderr, "Drawable is actually using the getRotation function!\n");
    //   // attribMutex.lock();
    //   // deg = rotationDeg;
    //   // x = rotationX;
    //   // y = rotationY;
    //   // attribMutex.unlock();
    // }

    /**
    * \brief Renders the class to the display
    */
    virtual void render() = 0;
  };
};


#endif /* DRAWABLE_H_ */
