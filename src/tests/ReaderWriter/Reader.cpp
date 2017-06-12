#include "Reader.h"

/**
 * Default-constructor for the Reader class.
 * @return: The constructed Reader object.
 */
Reader::Reader() : Thread() {
	myColor = ColorItem();
	vec = NULL;
	myCan = NULL;
	numRead = 0;
	recentX = recentY = myX = myY = 0;
}

/**
 * Explicit-constructor for the Reader class.
 * @param: sharedVec, a reference to the Vec object that is shared between the Reader and Writer.
 * @param: id, an unsigned long that will be passed to the Thread() constructor that will act as the id for the Thread object.
 * @param: can, a handle to the Canvas that will be drawn on and will determine whether or not to continue consuming object from the Queue. 
 * @return: The constructed Reader object.
 */
Reader::Reader(Vec<ColorItem> & sharedVec, unsigned long id, Canvas & can) : Thread(id) {
	numRead = recentX = recentY = 0;
	myColor = ColorItem();
	recentColor = can.getBackgroundColor();
	vec = &sharedVec;   //Get the address of that buffer and have the handle point to it
	myCan = &can;	//Get the handle to the Canvas
	int windowWidth = myCan->getWindowWidth();
	myX = windowWidth-50;
	myY = 50 * (id + 1) + 60;
	draw(myX, myY);
}

/**
 * read() method takes something from the vector and read it.
 */
void Reader::read() {
	while(myCan->isOpen()) { //While the Canvas is still open...
		myCan->sleepFor( (rand()%20+2)/10 ); //Wait for random time
		
		// want to get in Vec to read (move to space next to the data)
		draw(myX-75, myY);

		// get in Vec to read
		vec->beginRead();
		numRead++;
		draw(myX-130, myY);
		
		// do the reading
		myColor = vec->read();
		int beginX = myX-150, beginY = myY, endX = myColor.getX()+(ColorItem::width/2), endY = myColor.getY()+ColorItem::width/2;
		myCan->drawLine(beginX, beginY, endX, endY, BLACK, false, true); // draw arrow to item drawn
		myCan->sleep();
		myCan->drawLine(beginX, beginY, endX, endY, recentColor, false, true); // erase the arrow
		// tell all the ColorItems we may have overlapped to redraw themselves
		if( getId()%2 == 0 ) {
			for( int i = vec->getItems()-1; i >= 0; i--) {
				vec->getItem(i).draw();
			}
		} else {
			for( unsigned i = 0; i < vec->getItems(); i++) {
				vec->getItem(i).draw();
			}
		}

		// release lock
		draw(myX, myY); //Return to base location
		//std::cout << "Y" << std::flush;
		vec->endRead(); //Unlock the Vec
	}
}

/**
 * draw() method draws the Reader onto the Canvas.
 * @param: can, a reference to the Canvas object to draw on.
 * @param: windowWidth, an int representing the window width of the Canvas.
 * (in order to draw the Reader on the right side of the Canvas, I had to take the max width of the Canvas and subtract it by an offset.)
 */
void Reader::draw(int x, int y) {
	myCan->sleep();
	erase();
	recentColor = myCan->getPoint(x, y);
	myCan->drawCircle(x, y, 20, 32, myColor.getColor());
	int textSize = 20;
	if( numRead > 99 ) textSize = 15;
	myCan->drawText( to_string(numRead), x-15, y+5, textSize, BLACK);
	recentX = x;
	recentY = y;
}

/**
 * erase() method erases the Reader on the Canvas
 */
void Reader::erase() {
	myCan->drawCircle(recentX, recentY, 20, 32, recentColor);
}

/**
 * run() method is the implemented abstract method inherited from the Thread class. 
 * Calls the read() method. 
 */
void Reader::run() {
	read();
}
