#include "Line.h"

namespace tsgl {

Line::Line(int x1, int y1, int x2, int y2, const ColorFloat &color, bool dashed, bool endArrowHead, bool beginArrowHead) {
    vertices[0] = x1;
    vertices[1] = y1;
    vertices[6] = x2;
    vertices[7] = y2;
    vertices[2] = vertices[8] = color.R;
    vertices[3] = vertices[9] = color.G;
    vertices[4] = vertices[10] = color.B;
    vertices[5] = vertices[11] = color.A;
    
    isDashed = dashed;
    hasEndArrowHead = endArrowHead;
    hasBeginArrowHead = beginArrowHead;
    
    myColor = color;
}

void Line::draw() {
    
    if (isDashed) { // recursively draw a dashed line using shorter lines
        float x1 = vertices[0], y1 = vertices[1], x2 = vertices[6], y2 = vertices[7];
        // calculate the length
        float deltaX = x2-x1, deltaY = y2-y1;
        float length = sqrt(pow(deltaX, 2) + pow(deltaY, 2));
        
        if( length < (SEG_LENGTH + GAP_LENGTH) ) { // base case
            Line l(x1, y1, x2, y2, myColor);
            l.draw();
        } else { // recursively draw Lines
            // draw a short line
            Line l1(x1, y1, x1+(deltaX/length*SEG_LENGTH), y1+(deltaY/length*SEG_LENGTH), myColor, false);
            l1.draw();

            // recursively draw the rest of the line
            Line l2(x1+ deltaX/length * (SEG_LENGTH+GAP_LENGTH) , y1+ deltaY/length * (SEG_LENGTH+GAP_LENGTH), x2, y2, myColor, true);
            l2.draw();
        }
    } else {
        glBufferData(GL_ARRAY_BUFFER, 2 * 6 * sizeof(float), vertices, GL_DYNAMIC_DRAW);
        glDrawArrays(GL_LINES, 0, 2);
    }

    if (hasEndArrowHead) { // draw the triangle segment of the arrow
	    // calculate the angle of the line
	    float x = vertices[6], y = vertices[7];
    	float deltaX = vertices[6]-vertices[0], deltaY = vertices[7]-vertices[1];
	    drawArrowHead(x, y, deltaX, deltaY);
    }
    
    if (hasBeginArrowHead) { // draw the triangle segment of the arrow
	    // calculate the angle of the line
	    float x = vertices[0], y = vertices[1];
    	float deltaX = vertices[0]-vertices[6], deltaY = vertices[1]-vertices[7];
	    drawArrowHead(x, y, deltaX, deltaY);
    }
}

/**
 * \brief private method helping draw() for the arrow heads
 */
void Line::drawArrowHead(float x, float y, float deltaX, float deltaY) {
    double angle = atan( deltaY / deltaX );
    double s = sin( angle ), c = cos( angle );
	int x1 = -10*c-(-5)*s, x2 = 2*c - 0*s, x3 = -10*c-5*s;
	int y1 = -10*s+(-5)*c, y2 = 2*s + 0*c, y3 = -10*s+5*c;
    // if we have an arrow pointing left, rotate it pi radians ( sin = 0, cos = -1)
	if (deltaX < 0) {
        x1 = -x1, x2 = -x2, x3 = -x3;
		y1 = -y1, y2 = -y2, y3 = -y3;
    }
    // transpose the triangle to the end of the line
	x1 += x, x2 += x, x3 += x;
	y1 += y, y2 += y, y3 += y;
	
    Triangle t(x1, y1, x2, y2, x3, y3, myColor);
    t.draw();
}

}
