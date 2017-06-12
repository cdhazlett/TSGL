/*
 * Line.h extends Shape and provides a class for drawing a single line to a Canvas.
 */

#ifndef LINE_H_
#define LINE_H_

#include "Shape.h"  // For extending our Shape object
#include "Triangle.h"

namespace tsgl {

/*! \class Line
 *  \brief Draw a simple line.
 *  \details Line is a class for holding vertex data for a simple line.
 */
class Line : public Shape {
 private:
    float vertices[12];
    bool isDashed, hasEndArrowHead, hasBeginArrowHead;
    ColorFloat myColor;
    const int SEG_LENGTH = 10, GAP_LENGTH = 5;
    void drawArrowHead(float x, float y, float deltaX, float deltaY);
 public:

    /*!
     * \brief Explicitly constructs a new Line.
     * \details This is the constructor for the Line class.
     *      \param x1 The x coordinate of the first endpoint.
     *      \param y1 The y coordinate of the first endpoint.
     *      \param x2 The x coordinate of the second endpoint.
     *      \param y2 The y coordinate of the second endpoint.
     *      \param color The reference variable to the color of the Line.
     *      \param dashed The boolean value determining if the Line is dashed.
     *      \param endArrowHead The boolean value determining if the Line has an arrow head at (x2, y2).
     *      \param beginArrowHead The boolean value determining if the Line has an arrow head at (x1, y1).
     * \return A new Line with the specified endpoints and color.
     */
    Line(int x1, int y1, int x2, int y2, const ColorFloat &color, bool dashed = false, bool endArrowHead = false, bool beginArrowHead = false);

    /*!
     * \brief Draw the Line.
     * \details This function actually draws the Line to the Canvas.
     */
    void draw();
};

}

#endif /* LINE_H_ */
