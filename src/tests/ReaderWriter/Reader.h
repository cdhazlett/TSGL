/**
 * Reader.h contains the class necessary in order to create a Reader object for the Producer-Reader visualization.
 */
#ifndef CONSUMER_H_
#define CONSUMER_H_

#include <omp.h>
#include <tsgl.h>
#include "Vec.h"
#include "Thread.h"
using namespace tsgl;

/**
 * Reader class inherits from the Thread class in order to create a Reader object. 
 * Inheritance: Thread class.
 * Implements the run() method, which calls the read() method.
 */
class Reader : public Thread {
public:
	Reader(); //Default constructor
	Reader(Vec<ColorInt> & sharedVec, unsigned long id, Canvas & can);  //Explicit constructor
	void read(); //Consume method
	void draw();  //Draw the Reader onto the Canvas
	void run();	//Inheirted from Thread class; function that the pthread should run.
private:
	int myX, myY; //Center coordinates for the reader
	int numRead; //Tally of number of colors read
	ColorInt myColor;  //Color data obtained from the shared vec
	Vec<ColorInt> * vec; //Handle to the current vec
	Canvas * myCan;  //Handle to the Canvas
};

#endif /* CONSUMER_H_ */
