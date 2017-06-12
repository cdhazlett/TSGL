/**
 * Reader.h contains the class necessary in order to create a Reader object for the Reader-Writer visualization.
 */
#ifndef CONSUMER_H_
#define CONSUMER_H_

#include <omp.h>
//#include <tsgl.h>
#include "Vec.h"
#include "Thread.h"
#include "ColorItem.h"
#include "../../TSGL/tsgl.h"
#include "../../TSGL/Canvas.h"
#include "../../TSGL/Line.h"
using namespace tsgl;

/**
 * Reader class inherits from the Thread class in order to create a Reader object. 
 * Inheritance: Thread class.
 * Implements the run() method, which calls the read() method.
 */
class Reader : public Thread {
public:
	Reader(); //Default constructor
	Reader(Vec<ColorItem> & sharedVec, unsigned long id, Canvas & can);  //Explicit constructor
	void read(); //Consume method
	void draw(int x, int y);  //Draw the Reader onto the Canvas
	void erase(); //Remove Reader from Canvas
	void run();	//Inheirted from Thread class; function that the pthread should run.
private:
	int myX, myY; //Center coordinates for the reader
	int numRead; //Tally of number of colors read
	int recentX, recentY; //Coordinates of most recent location
	ColorInt recentColor; //Color to return to when erasing
	ColorItem myColor;  //Color data obtained from the shared vec
	Vec<ColorItem> * vec; //Handle to the current vec
	Canvas * myCan;  //Handle to the Canvas
};

#endif /* CONSUMER_H_ */
