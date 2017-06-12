/**
 * testReaderWriter.cpp contains the code to run the Readers-Writers Problem animation using TSGL and POSIX threads.
 * The program utilizes Reader and Writer classes as well as a custom Vec class to hold the shared data.
 * Usage: ./testReaderWriter [numberOfReaders] [numberOfWriters] [preference]
 * [preference] can be 'm' for monitor, 'w' for writer priority, or 'r' for reader priority
 */

//#include <omp.h>
//#include <tsgl.h>
#include <unistd.h>
#include <cassert> // assert
#include "ReaderWriter/Reader.h"
#include "ReaderWriter/Writer.h"
#include "ReaderWriter/ColorItem.h"
#include "../TSGL/tsgl.h"
#include "../TSGL/Canvas.h"
#include "../TSGL/Line.h"
using namespace tsgl;

// constants
const int WINDOW_WIDTH = 600, WINDOW_HEIGHT = 800, DATAX = WINDOW_WIDTH/3, DATAY = WINDOW_HEIGHT*5/6, MARGIN = 45; //Size of Canvas
const ColorFloat dataColor = GRAY; // color of the data rectangle

int main(int argc, char* argv[]) {
	
	// Start Reader-Writer visualization
	Canvas can(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, "Reader-Writer", 1.0f/2);  //Canvas to draw on
	Vec<ColorItem>::LockMethod lockType = Vec<ColorItem>::monitors; // choose from readerPref, writerPref, and monitors

	// number of readers is the first argument or defaults to 6
	int numReaders  = ( (argc > 1) && (atoi(argv[1]) > 0) && (atoi(argv[1]) <= 8) ) ? atoi(argv[1]) : 6;
	// number of writers is the second argument or defaults to 2
	int numWriters = ( (argc > 2) && (atoi(argv[2]) > 0) && (atoi(argv[2]) <= 8) ) ? atoi(argv[2]) : 2;
	
	std::string str = "a";
	if(argc > 3)
		str = argv[3];
	char c = str[0];
	switch(c) {
		case 'w':
			lockType = Vec<ColorItem>::writerPref;
			break;
		case 'r':
			lockType = Vec<ColorItem>::readerPref;
			break;
		case 'm':
			lockType = Vec<ColorItem>::monitors;
			break;
		default:
			lockType = Vec<ColorItem>::monitors;
			break;
	};

	Vec<ColorItem> sharedVec(20, floor(200 / ColorItem::width) * floor(600 / ColorItem::width), lockType);

	// seed the random number generator for colors and wait times
	srand(time(NULL));

	Reader * readers = new Reader[numReaders]; // Array of Readers
	Writer * writers = new Writer[numWriters]; // Array of Writers

	// Start the visualization
	can.start();
	can.setBackgroundColor(WHITE);

	// Label Canvas with the lock type being used
	can.drawRectangle(DATAX-MARGIN, WINDOW_HEIGHT/12, WINDOW_WIDTH*2/3+MARGIN, DATAY, dataColor, true); // draw data area
	std::string lockString;
	switch(lockType) {
		case Vec<ColorItem>::readerPref:
			lockString = "Reader priority";
			break;
		case Vec<ColorItem>::writerPref:
			lockString = "Writer priority";
			break;
		case Vec<ColorItem>::monitors:
			lockString = "Monitors";
			break;
		default:
			lockString = "";
			break;
	};
	can.drawText(lockString, 50, WINDOW_HEIGHT-50, 20, BLACK);
	can.drawText("Numbers indicate", WINDOW_WIDTH-225, WINDOW_HEIGHT-50, 20, BLACK);
	can.drawText("counts of reads/writes", WINDOW_WIDTH-225, WINDOW_HEIGHT-30, 20, BLACK);

	// Label Readers and Writers
	can.drawText("Writers", 20, 20, 20, BLACK);
	can.drawText("Readers", WINDOW_WIDTH-150, 20, 20, BLACK);

	// Fill the Reader and Writer arrays with their objects
	for(int i = 0; i < numReaders; i++) {
		readers[i] = Reader(sharedVec, i, can); // Reader is created and draws itself
	}
	
	for(int i = 0; i < numWriters; i++) {
		writers[i] = Writer(sharedVec, i, can); // Writer is created and draws itself
	}
	
	// start up the Reader and Writer pthreads
	for(int i = 0; i < numWriters; i++) {
		writers[i].start();
		sleep(0.1);
	}
	sleep(1);
	for(int i = 0; i < numReaders; i++) {
		readers[i].start();
		sleep(0.1);
	}
	
	can.wait(); // wait for the main Canvas to be closed
	
	// end threads once the canvas has been closed
	for(int i = 0; i < numReaders; i++) {
		readers[i].join();
	}
	
	for(int i = 0; i < numWriters; i++) {
		writers[i].join();
	}
	
	// Cleanup
	delete [] readers;
	delete [] writers;
	readers = NULL;
	writers = NULL;
	
	return 0;
}
