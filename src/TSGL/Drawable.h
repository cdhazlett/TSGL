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

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/normal.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <../glad/glad.h>       // Loader for OpenGL function calls
#include <mutex>                // Needed for locking the attribute mutex for thread-safety
#include <algorithm>            // std::min
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
    glm::mat4       rotateMatrix;  ///< The transformation matrix for this model
    glm::mat4       scaleMatrix;  ///< The transformation matrix for this model
    glm::mat4       translateMatrix;  ///< The transformation matrix for this model
    double          *vertexArray; ///< The array which holds (x,y,z) values for each vertex
    float           *vertexColorArray; ///< The array which holds (r,g,b,a) color values for each vertex
    double          *vertexNormalArray; ///< The array which holds (x,y,z) normals for each vertex
    int             numVertices = 0;  ///< The number of vertices in the model
    int             currentVertex = 0;  ///< The current number of initted vertices

  private:
    float deg2rad(float deg) {
      return(deg*3.14159265/180);
    }

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
      vertexColorArray = new float[numVertices*4];
      memset(vertexColorArray, 0, sizeof(float)*numVertices*4);
      vertexNormalArray = new double[numVertices*3];
      memset(vertexNormalArray, 0, sizeof(double)*numVertices*3);


      rotateMatrix = glm::mat4(1.0f);
      scaleMatrix = glm::mat4(1.0f);
      translateMatrix = glm::mat4(1.0f);

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
      rotateMatrix  = glm::rotate(rotateMatrix, deg2rad(degrees), glm::vec3(x, y, z));
      attribMutex.unlock();
    }

    /**
    * \brief Resets the rotation of an object
    */
    void resetRotation() {
      attribMutex.lock();
      rotateMatrix = glm::mat4(1.0f);
      attribMutex.unlock();
    }

    /**
    * \brief Scale an object
    */
    void scale(float x, float y, float z) {
      attribMutex.lock();
      scaleMatrix = glm::scale(scaleMatrix, glm::vec3(x, y, z));
      attribMutex.unlock();
    }

    /**
    * \brief Resets the scale of an object
    */
    void resetScale() {
      attribMutex.lock();
      scaleMatrix = glm::mat4(1.0f);
      attribMutex.unlock();
    }

    /**
    * \brief Translate an object
    */
    void translate(float x, float y, float z) {
      attribMutex.lock();
      translateMatrix = glm::translate(translateMatrix, glm::vec3(x, y, z));
      attribMutex.unlock();
    }

    /**
    * \brief Resets the translation of an object
    */
    void resetTranslation() {
      attribMutex.lock();
      translateMatrix = glm::mat4(1.0f);
      attribMutex.unlock();
    }

    void setColorForAllVertices(const ColorFloat& color) {
      attribMutex.lock();
      int i;
      for(i=0; i<currentVertex; i++) {
        vertexColorArray[i*4] = color.R;
        vertexColorArray[i*4 +1] = color.G;
        vertexColorArray[i*4 +2] = color.B;
        vertexColorArray[i*4 +3] = color.A;
      }
      attribMutex.unlock();
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
    const float* getVertexColorArrayPointer() {
      attribMutex.lock();
      float* retVal = vertexColorArray;
      attribMutex.unlock();
      return retVal;
    }

    /*!
    * \brief Adds another vertex to a Drawable.
    * \details This function initializes the next vertex in the Drawable.
    */
    void addVertexWithRGBA(float x, float y, float z, float r, float g, float b, float a) {
      attribMutex.lock();

      if (currentVertex == numVertices) {
        fprintf(stderr, "This Drawable class is full at %d vertices!\n", numVertices);
        attribMutex.unlock();
        return;
      }

      vertexArray[currentVertex*3] = x;
      vertexArray[currentVertex*3 +1] = y;
      vertexArray[currentVertex*3 +2] = z;

      vertexColorArray[currentVertex*4] = r;
      vertexColorArray[currentVertex*4 +1] = g;
      vertexColorArray[currentVertex*4 +2] = b;
      vertexColorArray[currentVertex*4 +3] = a;

      currentVertex++;

      attribMutex.unlock();

      if (currentVertex%3 == 0) {
        computeNormals(); // Recompute the normal data when a new triangle is added
        // TODO this might be really slow, may need to speed this up some
      }

    }

    glm::vec3 getVertexPosition(int i) {
      attribMutex.lock();
      if (i > currentVertex) {
        fprintf(stderr, "Vertex index %d out of bounds.\n", i);
        attribMutex.unlock();
        return glm::vec3(0.f,0.f,0.f);
      }
      glm::vec3 retval = glm::vec3(vertexArray[i*3], vertexArray[i*3+1], vertexArray[i*3+2]);
      attribMutex.unlock();
      return retval;
    }

    void writeNormalForVertexPosition(int i, glm::vec3 normal) {
      attribMutex.lock();
      if (i > currentVertex) {
        fprintf(stderr, "Vertex index %d out of bounds.\n", i);
        attribMutex.unlock();
        return;
      }
      vertexNormalArray[i*3] = (double)normal.x;
      vertexNormalArray[i*3+1] = (double)normal.y;
      vertexNormalArray[i*3+2] = (double)normal.z;
      attribMutex.unlock();
    }

    void computeNormals() {
      // Reset the normal vector array
      attribMutex.lock();
      memset(vertexNormalArray, 0, sizeof(double)*numVertices*3);
      attribMutex.unlock();

      // For each triangle, find the normal, apply to all vertex in triangle
      int i;
      for (i=0; i<currentVertex/3; i++) {
        // Get the vertices for the current triangle
        glm::vec3 t1, t2, t3;
        t1 = getVertexPosition(i*3);
        t2 = getVertexPosition(i*3+1);
        t3 = getVertexPosition(i*3+2);

        // Compute the surface normal
        glm::vec3 normal = glm::triangleNormal(t1, t2, t3);

        writeNormalForVertexPosition(i*3, normal);
        writeNormalForVertexPosition(i*3+1, normal);
        writeNormalForVertexPosition(i*3+2, normal);
      }

    }

    void changeCapacity(int newNumVertices) {
      attribMutex.lock();
      double *oldVertexArray = vertexArray;
      float *oldVertexColorArray = vertexColorArray;
      double *oldVertexNormalArray = vertexNormalArray;


      vertexArray = new double[newNumVertices*3];
      memset(vertexArray, 0, sizeof(double)*newNumVertices*3);
      vertexColorArray = new float[newNumVertices*4];
      memset(vertexColorArray, 0, sizeof(float)*newNumVertices*4);
      vertexNormalArray = new double[newNumVertices*3];
      memset(vertexNormalArray, 0, sizeof(double)*newNumVertices*3);


      memcpy(vertexArray, oldVertexArray, sizeof(double)*std::min(numVertices, newNumVertices)*3);
      memcpy(vertexColorArray, oldVertexColorArray, sizeof(float)*std::min(numVertices, newNumVertices)*4);
      memcpy(vertexNormalArray, oldVertexNormalArray, sizeof(double)*std::min(numVertices, newNumVertices)*3);

      numVertices = newNumVertices;
      currentVertex = std::min(currentVertex, newNumVertices);

      delete[] oldVertexArray;
      delete[] oldVertexColorArray;
      delete[] oldVertexNormalArray;
      attribMutex.unlock();
    }

    glm::mat4 getModelMatrix() {
      attribMutex.lock();
      glm::mat4 retVal = translateMatrix * rotateMatrix * scaleMatrix;
      attribMutex.unlock();
      return retVal;
    }

    glm::vec3 getTranslation() {
      attribMutex.lock();
      glm::vec3 retVal = glm::vec3(translateMatrix[0][3], translateMatrix[1][3], translateMatrix[2][3]);
      attribMutex.unlock();

      return retVal;
    }

    // glm::quat getRotation() {
    //   // decompose (tmat4x4< T, P > const &modelMatrix, tvec3< T, P > &scale, tquat< T, P > &orientation, tvec3< T, P > &translation, tvec3< T, P > &skew, tvec4< T, P > &perspective)
    //
    //   glm::vec3 scale;
    //   glm::quat orientation;
    //   glm::vec3 translation;
    //   glm::vec3 skew;
    //   glm::vec4 perspective;
    //
    //   attribMutex.lock();
    //   glm::decompose(modelMatrix, scale, orientation, translation, skew, perspective);
    //   attribMutex.unlock();
    //
    //   return orientation;
    // }

    glm::vec3 getScale() {
      attribMutex.lock();
      glm::vec3 retVal = glm::vec3(scaleMatrix[0][0], scaleMatrix[1][1], scaleMatrix[2][2]);
      attribMutex.unlock();

      return retVal;
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

    /**
    * \brief Renders the class to the display
    */
    virtual void render(GLuint &vertexbuffer, GLuint &colorbuffer, GLuint &normalbuffer) = 0;
  };
};


#endif /* DRAWABLE_H_ */
