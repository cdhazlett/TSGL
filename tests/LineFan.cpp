/*
 * LineFan.cpp
 *
 * Usage: ./LineFan <width> <height> <numThreads>
 */

#include <omp.h>
#include <tsgl.h>
#include <vector>
#include "Util.h"

using namespace tsgl;

/*!
 * \brief Draws a fan of randomly colored lines at the target framerate and a dynamic number of threads using OMP.
 * \details
 * - The internal timer of the Canvas is set up to go off every \b FRAME seconds ( \b FRAME == 1 / \b FPS ).
 * - The spacing in between the arcs of the fan is stored in the constant: \b ARC .
 *  - The number of threads to use is recalculated, and the process is forked based off of the passed number of threads: \b t.
 *  - The internal timer sleeps on each thread until the next frame is ready to be drawn.
 *  - An offset is calculated based on the thread's ID and a predefined arc-length.
 *  - An angle is then calculated using the offset and the Canvas' current lifespan ( as calculated by \b can.getReps() ).
 *  - The vertices of the lines to be drawn are chosen using the sines and cosines of the predetermined angle.
 * - While the canvas is open:
 *  - The lines change to new random colors in order of creation.
 *  .
 * .
 * \param can Reference to the Canvas being drawn to.
 * \param t The number of threads to use in the function.
 */
void lineFanFunction(Canvas& can, int t) {
  const double ARC = 7.11;  //(Arbitrary) spacing between arcs of the fan
  can.setBackgroundColor(WHITE);
  srand(time(NULL));
  #pragma omp parallel num_threads(t)
  {
    int a, b, c, d;
    double angle, offset = omp_get_thread_num() * ARC;
    std::vector<Line*> lines;
    //Create the lines that for the ellipse
    for(int i = 0; i < 180; i++) {
      can.sleep();
      angle = offset + i * RAD;
      a = can.getWindowWidth() / 2 * (1 + sin(angle));  //First x
      b = can.getWindowHeight() / 2 * (1 + cos(angle)); //First y
      c = can.getWindowWidth() / 2 * (1 - sin(angle));  //Second x
      d = can.getWindowHeight() / 2 * (1 - cos(angle)); //Second y
      Line* nextLine = new Line(a, b, c, d, Colors::randomColor()); //Create line
      lines.push_back(nextLine); //Store line pointer for changes later
      can.add(nextLine); //Add to Canvas
    }
    int index = 0;
    while (can.isOpen()) {
      can.sleep();
      lines[index]->setColor(Colors::randomColor()); //Change line to a random color
      index++; index = index % lines.size();
    }

    //Delete pointers after Canvas is closed
    for(unsigned i = 0; i < lines.size(); i++) {
      delete lines[i];
    }
  }
}

//Takes command-line arguments for the width and height of the screen as well as for the
//number of threads to use in the function
int main(int argc, char** argv) {
  int w = (argc > 1) ? atoi(argv[1]) : 1.2*Canvas::getDisplayHeight();
  int h = (argc > 2) ? atoi(argv[2]) : 0.75*w;
  if (w <= 0 || h <= 0) {    //Checked the passed width and height if they are valid
    w = 1200;
    h = 900;            //If not, set the width and height to a default value
  }
  unsigned t = (argc > 3) ? atoi(argv[3]) : omp_get_num_procs();   //Get the number of threads to use
  if (t == 0)
    t = omp_get_num_procs();
  Canvas c(-1,-1,w,h,"Line Fan");
  c.run(lineFanFunction,t);
}