#include "Reader.h"

/**
 * Default-constructor for the Reader class.
 * @return: The constructed Reader object.
 */
Reader::Reader() : Thread() {
	myColor = ColorInt(0,0,0);
	vec = NULL;
	myCan = NULL;
	numRead = 0;
}

/**
 * Explicit-constructor for the Reader class.
 * @param: sharedVec, a reference to the Vec object that is shared between the Reader and Writer.
 * @param: id, an unsigned long that will be passed to the Thread() constructor that will act as the id for the Thread object.
 * @param: can, a handle to the Canvas that will be drawn on and will determine whether or not to continue consuming object from the Queue. 
 * @return: The constructed Reader object.
 */
Reader::Reader(Vec<ColorInt> & sharedVec, unsigned long id, Canvas & can) : Thread(id) {
	numRead = 0;
	myColor = ColorInt(0, 0, 0);
	vec = &sharedVec;   //Get the address of that buffer and have the handle point to it
	myCan = &can;	//Get the handle to the Canvas
	int windowWidth = myCan->getWindowWidth();
	unsigned long offset = getId();   //Get the id of the pthread (the id will act as an offset)
	myX = windowWidth-50;
	myY = 50 * (offset + 1);
	draw();
}

/**
 * read() method takes something from the vector and read it.
 */
void Reader::read() {
	while(myCan->isOpen()) { //While the Canvas is still open...
		myCan->sleepFor( (rand()%10+3)/10 );
		vec->beginRead();
		myColor = vec->read();
		myCan->sleep();
		draw();
		numRead++;
		myCan->drawText( to_string(numRead), myX-5-10*floor( log10(numRead) ), myY+5, 20, BLACK);
		vec->endRead();
	}
}

/**
 * draw() method draws the Reader onto the Canvas.
 * @param: can, a reference to the Canvas object to draw on.
 * @param: windowWidth, an int representing the window width of the Canvas.
 * (in order to draw the Reader on the right side of the Canvas, I had to take the max width of the Canvas and subtract it by an offset.)
 */
void Reader::draw() {
	ColorInt white = ColorInt(255, 255, 255);	
	if(myColor == white) {
		myColor = ColorInt(0, 0, 0);
	}	
	myCan->drawCircle(myX, myY, 20, 32, myColor);
}

/**
 * run() method is the implemented abstract method inherited from the Thread class. 
 * Calls the read() method. 
 */
void Reader::run() {
	read();
}
