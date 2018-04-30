/*
 * Canvas3D.h provides a window / canvas for all of the drawing operations in
 * the TSGL library. //TODO
 */

#define __DEBUG__

#ifndef CANVAS3D_H_
#define CANVAS3D_H_

#include "Canvas.h"

namespace tsgl {

/*! \class Canvas
 *  \brief A GL window with numerous built-in, thread-safe drawing operations.
 *  \details Canvas provides an easy-to-set-up, easy-to-use class for drawing
 * various shapes. \details Using STB, Canvas also supports the drawing of
 * images. \details On top of being easy to use, Canvas is also thread-safe, so
 * any number of images may be drawn at once. \note <b>OS X:</b> Due to the way
 * OS X handles I/O, either sleep() or handleIO() must be manually called
 *       whenever the user wants to handle any input/output events
 * (keyboard/mouse presses). Whenever a window is created using OpenGL, OS X
 * requires the main thread to handle I/O calls. \note <b>OS X:</b> OS X also
 * uses p_thread instead of std::thread for threading. \bug <b>Linux:</b> X
 * forwarding does not work properly with TSGL.
 */
class Canvas3D : public Canvas {

public:
  /*!
   * \brief Default Canvas constructor method.
   * \details This is the default constructor for the Canvas class.
   *   \param timerLength The minimum number of seconds between draw cycles for
   * the Canvas. A value less than or equal to 0 sets it to automatic. \return A
   * new Canvas in the middle of the screen with no title. The created Canvas
   * will take up approximately 90% of the monitor's height, and will have a 4:3
   * aspect ratio.
   */
  Canvas3D(double timerLength = 0.0f) : Canvas(timerLength) {}

  /*!
   * \brief Explicit Canvas constructor method.
   * \details This is the explicit constructor for the Canvas class.
   *   \param x The x position of the Canvas window.
   *   \param y The y position of the Canvas window.
   *   \param width The x dimension of the Canvas window.
   *   \param height The y dimension of the Canvas window.
   *   \param title The title of the window.
   *   \param timerLength The minimum number of seconds between draw cycles for
   * the Canvas. A value less than or equal to 0 sets it to automatic. \return A
   * new Canvas with the specified position, dimensions, title, and draw cycle
   * length.
   */
  Canvas3D(int x, int y, int width, int height, std::string title,
           double timerLength = 0.0f)
      : Canvas(x, y, width, height, title, timerLength) {}

  /*!
   * \brief Canvas destructor method.
   * \details This is the destructor for the Canvas class.
   * \details Frees up memory that was allocated to a Canvas instance.
   */
  // virtual ~Canvas();

  // Changes the camera position on the 3D canvas
  void setCameraPosition(float x, float y, float z) {

    // put the new values into the camera's variables
    eyeX = x;
    eyeY = y;
    eyeZ = z;

  }

  // set the point which the camera is pointed at
  void setCameraFocusPoint(float x, float y, float z) {

    // update the lookat position variables
    lookX = x;
    lookY = y;
    lookZ = z;

  }

  // Change the data for the camera perspective on the canvas
  void setCameraPerspective(float FieldOfView, float nearClipDist, float farClipDist) {
    FOV = FieldOfView;
    nearClip = nearClipDist;
    farClip = farClipDist;
  }

private:

  // Instance variables for the Canvas camera setup
  float eyeX, eyeY, eyeZ = 0.0;     // the eye position coordinates for the camera
  float lookX, lookY, lookZ = 0.0;  // the lookat point coordinates for the camera

  // FOV data
  float nearClip = .1f;
  float farClip = 100.f;
  float FOV = 45.0f;

  glm::mat4 getCameraMatrix() {

    // Calculate the projection matrix:
    glm::mat4 Projection = glm::perspective(
      glm::radians(FOV),                    // FOV
      (float) getWindowHeight() / (float) getWindowWidth(),  // Aspect ratio
      nearClip,                             // Near clipping plane
      farClip                               // Far clipping plane
    );

    // Calculate the camera matrix:
    glm::mat4 View = glm::lookAt(
        glm::vec3(eyeX, eyeY, eyeZ),    // Camera position
        glm::vec3(lookX, lookY, lookZ), // Lookat point
        glm::vec3(0,1,0)      // Head position, for now always up
    );

    // Multiply the matricies:
    return Projection * View; // Matrix mults always seem the other way around

  }

};

} // namespace tsgl

#endif /* CANVAS3D_H_ */
