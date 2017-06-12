/**
 * Writer.h contains the class necessary in order to create a Writer object for the Reader-Writer visualization.
 */
#ifndef WRITER_H_
#define WRITER_H_

//#include <omp.h>
//#include <tsgl.h>
#include "Vec.h"
#include "Thread.h"
#include "ColorItem.h"
#include "../../TSGL/tsgl.h"
#include "../../TSGL/Canvas.h"
#include "../../TSGL/Line.h"
using namespace tsgl;

/**
 * Writer class creates a Writer object and inherits from the Thread class.	
 * Inheritance: Thread class.
 * Implements the abstract run() method from the Thread class so that the pthread runs the write() method.
 */
class Writer : public Thread {
public:
	Writer(); //Default constructor
	Writer(Vec<ColorItem> & sharedData, unsigned long id, Canvas & can);  //Size of data to generate	and id of pthread
	void write(); //Generate color data
	void draw(int x, int y); //Draw the circles onto the Canvas
	void erase(); //Remove Writer from the the Canvas
	void run();  //Implementation of run() method for pthread

private:
	int myFirst, mySecond, myThird; //Ints for the ColorInt to use in generating a random color
	int myX, myY;   //Center coordinates for the Writer
	int recentX, recentY; //Coordinates most recently located at
	ColorInt recentColor; //Color to return to at erase
	int dataX, dataY; // Bottom left coordinates of the data display
	int numWritten; //Tally of number of colors written to Vec
	ColorItem myColorData;  //Generated color data
	Vec<ColorItem> * data;  //Handle to the shared buffer
	Canvas * myCan;   //Handle to the Canvas
};

#endif /* PRODUCER_H_ */
