#include "Writer.h"

/**
 * Default-constructor for the Writer class.
 * @return: The constructed Writer object.
 */
Writer::Writer() : Thread() {
	myFirst = mySecond = myThird = myX = myY = dataX = dataY = numWritten = recentX = recentY = 0;
	data = NULL;
	myCan = NULL;
	myColorData = ColorItem();
}

/**
 * Explicit-constructor for the Writer class.
 * @param: shareddata, a reference to the Vec object that is shared between the Consumer and Writer.
 * @param: id, an unsigned long that will be passed to the Thread() constructor that will act as the id for the Thread object.
 * @param: can, a handle to the Canvas that will be drawn on and will determine whether or not to continue writing object into the Vec. 
 * @return: The constructed Writer object.
 */
Writer::Writer(Vec<ColorItem> & shareddata, unsigned long id, Canvas & can) : Thread(id) {
	myFirst = mySecond = myThird = myY = numWritten = recentX = recentY = 0;
	recentColor = can.getBackgroundColor();
	myX = 50; //Set the x-coordinate to 50
	myY = myX * (id + 1) + 60; //Multiply the center y-coordinate by the id of the pthread and add 1	
	dataX = can.getWindowWidth()/3;
	dataY = can.getWindowHeight()*5/6;
	data = &shareddata;  //Get the handle to the data and have it point to the data
	myCan = &can;  //Get the handle to the Canvas
	myColorData = ColorItem();
	draw(myX, myY);
}

/**
 * write() method generates a random color and attempts to append it to the shared data.
 */
void Writer::write() {
	while (myCan->isOpen()) {  //While the Canvas is still open...
		myCan->sleepFor( (rand()%10+2)/10 ); // wait for a random time

		// generate a color
		myFirst = rand() % 255;  //Generate your data and try to stick it in the Vec
		mySecond = rand() % 255;
		myThird = rand() % 255;
		ColorInt color = ColorInt(myFirst, mySecond, myThird);
		
		// show intention to get in Vec to write (move to space next to the data)
		draw(myX+75, myY);

		// get in Vec to write
		data->beginWrite(); // lock the Vec for writing
		numWritten++;
		draw(myX+130, myY); // display presence in Vec
		
		// do the writing
		unsigned itemNum = rand()%(data->getMax()); // random index between 0 and the number of items in data
		if( itemNum > data->getItems() )
			itemNum = data->getItems();
		int dataRow = itemNum/(200/ColorItem::width);
		int dataCol = itemNum%(200/ColorItem::width);
		int leftX  = dataX+ColorItem::width*dataCol, topY = dataY-ColorItem::width*(dataRow+1);

		myColorData = ColorItem(*myCan, color, leftX, topY, itemNum);
		data->write(myColorData, itemNum);  // Write the item to the Vec

		myColorData.draw();
		
		// draw an arrow down to the item
		int endX = leftX+(ColorItem::width/2), endY = topY+ColorItem::width/2, beginX = myX+150, beginY = myY;
		myCan->drawLine(beginX, beginY, endX, endY, BLACK, false, true); // draw arrow to item drawn
		myCan->sleep();
		myCan->drawLine(beginX, beginY, endX, endY, recentColor, false, true); // erase the arrow
		// tell all the ColorItems we may have overlapped to redraw themselves
		for( unsigned i = 0; i < data->getItems(); i++) {
			data->getItem(i).draw();
		}
		
		// release lock
		draw(myX, myY); // return to starting location
		data->endWrite(); // unlock the Vec for writing
	} 
}

/**
 * draw() method draws the Writer onto the Canvas.
 */
void Writer::draw(int x, int y) {
	unsigned long id = getId();	//Get the Id of the pthread (the id will act as an offset)
	ColorFloat color = Colors::highContrastColor(id);  //Get the color based off of the id
	myCan->sleep();
	erase();
	recentColor = myCan->getPoint(x, y);
	myCan->drawCircle(x, y, 20, 32, color);
	int textSize = 20;
	if( numWritten > 99 ) textSize = 15;
	myCan->drawText( to_string(numWritten), x-15, y+5, textSize, BLACK);
	recentX = x;
	recentY = y;
}

/**
 * erase() method erases the Writer on the Canvas
 */
void Writer::erase() {
	myCan->drawCircle(recentX, recentY, 20, 32, recentColor);
}

/**
 * run() method is the implemented abstract method inherited from the Thread class. 
 * Calls the write() method. 
 */
void Writer::run() {
	write();
}
