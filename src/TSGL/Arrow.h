/*!
* Arrow.h extends Line and provides a class for drawing an arrow to a Canvas.
*/

#ifndef ARROW_H_
#define ARROW_H_

#include "Line.h"
#include "Triangle.h"

namespace tsgl {
  /*!
  * \class Arrow
  * \brief draw an arrow.
  * \details Arrow extends Line and includes an arrow head (triangle).
  */
  class Arrow : public Line {
  private:
    Triangle * arrowHead;   // Triangle of first arrow head
    Triangle * arrowHead2;  // Triangle of second arrow head
    bool isDoubleArrow = false;
    Triangle* makeArrowHead(float x, float y, float deltaX, float deltaY, const ColorFloat &color);

  public:
    /*!
    * \brief Explicitly constructs a new Arrow.
    * \details This is the constructor for the Arrow class.
    *      \param x1 The x coordinate of the first endpoint.
    *      \param y1 The y coordinate of the first endpoint.
    *      \param x2 The x coordinate of the second endpoint.
    *      \param y2 The y coordinate of the second endpoint.
    *      \param color The reference variable to the color of the Arrow.
    *      \param doubleArrow Determines whether Arrow has two heads.
    * \return A new Arrow with the specified endpoints and color.
    */
    Arrow(int x1, int y1, int x2, int y2, const ColorFloat &color = BLACK, bool doubleArrow = false);

    ~Arrow();

  };

}

#endif /* ARROW_H_ */