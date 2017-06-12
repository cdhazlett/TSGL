/**
 * ColorItem.h provides a ColorItem class for the Reader-Writer visualization.
 */

#ifndef COLORITEM_H_
#define COLORITEM_H_

//#include <tsgl.h>
//using namespace tsgl;

#include "../../TSGL/tsgl.h"
using namespace tsgl;


class ColorItem {
public:
	ColorItem(); //Default constructor
	ColorItem(Canvas & can, ColorInt color, int x, int y, int index);
	ColorItem& operator=(const ColorItem& original);
	bool operator==(ColorItem& original);
	int getX() { return myX; }
	int getY() { return myY; }
	int getId() { return myId; }
	void draw(); //Draw ColorItem onto the Canvas
	ColorInt getColor() { return myColor; } //get the ColorInt stored in the ColorItem
	//static int getWidth() { return myWidth; }
	static const int width;

private:
	ColorInt myColor;
	int myX, myY; //Top left coorindates for the ColorItem
	int myId; // id number, also index in Vec
	Canvas * myCan; //Handle to the Canvas
	//static const int myWidth = 40;
};

#endif /*COLORITEM_H_*/