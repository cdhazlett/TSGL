#include "ColorItem.h"

const int ColorItem::width = 20;

/**
 * Default constructor for the ColorItem class.
 * \return: the constructed ColorItem
 */
ColorItem::ColorItem() {
	myColor = ColorInt(0, 0, 0);
	myX = myY = myId = 0;
	myCan = NULL;
}

/**
 * Explicit constructor for the ColorItem class.
 * \param can, a handle to the Canvas
 * \param color, a ColorInt to store this ColorItem's color
 * \param x, the x coordinate
 * \param y, the y coordinate
 * \return: the constructed ColorItem
 */
ColorItem::ColorItem(Canvas & can, ColorInt color, int x, int y, int index) {
	myCan = &can;
	myColor = color;
	myX = x, myY = y, myId = index;
}

ColorItem& ColorItem::operator=(const ColorItem& original) {
	myX = original.myX;
	myY = original.myY;
	myColor = original.myColor;
	myCan = original.myCan;
	return *this;
}

bool ColorItem::operator==(ColorItem& other) {
	if( myCan != other.myCan )
		return false;
	if( myX != other.myX )
		return false;
	if( myY != other.myY )
		return false;
	return true;
}

/**
 * \brief Draws the ColorItem to the Canvas
 */
void ColorItem::draw() {
	myCan->drawRectangle(myX, myY, myX+width, myY+width, myColor, true);
}
