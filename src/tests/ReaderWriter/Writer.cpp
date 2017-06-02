#include "Writer.h"

/**
 * Default-constructor for the Writer class.
 * @return: The constructed Writer object.
 */
Writer::Writer() : Thread() {
	myFirst = mySecond = myThird = myX = myY = dataX = dataY = numWritten = 0;
	data = NULL;
	myCan = NULL;
}	

/**
 * Explicit-constructor for the Writer class.
 * @param: shareddata, a reference to the Vec object that is shared between the Consumer and Writer.
 * @param: id, an unsigned long that will be passed to the Thread() constructor that will act as the id for the Thread object.
 * @param: can, a handle to the Canvas that will be drawn on and will determine whether or not to continue writing object into the Vec. 
 * @return: The constructed Writer object.
 */
Writer::Writer(Vec<ColorInt> & shareddata, unsigned long id, Canvas & can) : Thread(id) {
	myFirst = mySecond = myThird = myY = numWritten = 0;
	myX = 50; //Set the x-coordinate to 50
	myY = myX * (id + 1); //Multiply the center y-coordinate by the id of the pthread and add 1	
	dataX = can.getWindowWidth()/3;
	dataY = can.getWindowHeight()*5/6;
	data = &shareddata;  //Get the handle to the data and have it point to the data
	myCan = &can;  //Get the handle to the Canvas
	draw();
}

/**
 * write() method generates a random color and attempts to append it to the shared data.
 */
void Writer::write() {
	while (myCan->isOpen()) {  //While the Canvas is still open...
		myCan->sleepFor( (rand()%10+3)/10 );
		myFirst = rand() % 255;  //Generate your data and try to stick it in the Vec
		mySecond = rand() % 255;
		myThird = rand() % 255;
		myColorData = ColorInt(myFirst, mySecond, myThird);
		data->beginWrite();
		int itemNum = data->getItems();
		int dataRow = itemNum/20;
		int dataCol = itemNum%20;
		myCan->sleep();
		int leftX  = dataX+10*dataCol, botY = dataY-10*(dataRow+1);
		int rightX = leftX+10,         topY = botY+10;
		myCan->drawRectangle(leftX, topY, rightX, botY, myColorData, true); // draw the data item
		data->write(myColorData);  // Write the item
		numWritten++;
		draw();
		myCan->drawText( to_string(numWritten), myX-5-10*floor( log10(numWritten) ), myY+5, 20, BLACK);
		data->endWrite();
	} 
}

/**
 * draw() method draws the Writer onto the Canvas.
 */
void Writer::draw() {
	unsigned long id = getId();	//Get the Id of the pthread (the id will act as an offset)
	ColorFloat color = Colors::highContrastColor(id);  //Get the color based off of the id
	myCan->sleep();
	myCan->drawCircle(myX, myY, 20, 32, color);
}

/**
 * run() method is the implemented abstract method inherited from the Thread class. 
 * Calls the write() method. 
 */
void Writer::run() {
	write();
}
