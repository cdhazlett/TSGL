/**
 * testReaderWriter.cpp contains the code to run the Readers-Writers Problem animation using TSGL and POSIX threads.
 * The program utilizes Reader and Writer classes as well as a custom Vec class to hold the shared data.
 * Usage: ./testReaderWriter [numberOfReaders] [numberOfWriters]
 */

#include <omp.h>
#include <tsgl.h>
#include <unistd.h>
#include "ReaderWriter/Reader.h"
#include "ReaderWriter/Writer.h"
using namespace tsgl;

// constants
const int WINDOW_WIDTH = 600, WINDOW_HEIGHT = 500, DATAX = WINDOW_WIDTH/3, DATAY = WINDOW_HEIGHT*5/6; //Size of Canvas
Canvas c(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, "Reader-Writer", 1.0f/2);  //Canvas to draw on
const ColorFloat dataColor = GRAY; // color of the data rectangle
const Vec<ColorInt>::LockMethod lockType = Vec<ColorInt>::monitors; // choose from readerPref, writerPref, and monitors
Vec<ColorInt> sharedVec(20, lockType);

void readWriteFunction(Canvas & can, int numReaders, int numWriters) {	

	// seed the random number generator for colors and wait times
	srand(time(NULL));

	Reader * readers = new Reader[numReaders]; // Array of Readers
	Writer * writers = new Writer[numWriters]; // Array of Writers

	// Start the visualization
	can.start();
	can.setBackgroundColor(WHITE);

	// Label Canvas with the lock type being used
	can.drawRectangle(DATAX, WINDOW_HEIGHT/6, WINDOW_WIDTH*2/3, DATAY, dataColor, true); // draw data area
	std::string lockString;
	switch(lockType) {
		case Vec<ColorInt>::readerPref:
			lockString = "Reader priority";
			break;
		case Vec<ColorInt>::writerPref:
			lockString = "Writer priority";
			break;
		case Vec<ColorInt>::monitors:
			lockString = "Monitors";
			break;
		default:
			lockString = "";
			break;
	};
	can.drawText(lockString, 50, WINDOW_HEIGHT-50, 20, BLACK);

	// Label Readers and Writers
	can.drawText("Writers", 20, 20, 20, BLACK);
	can.drawText("Readers", WINDOW_HEIGHT-20, 20, 20, BLACK);

	// Fill the Reader and Writer arrays with their objects
	for(int i = 0; i < numReaders; i++) {
		readers[i] = Reader(sharedVec, i, can); // Reader is created and draws itself
	}
	
	for(int i = 0; i < numWriters; i++) {
		writers[i] = Writer(sharedVec, i, can); // Writer is created and draws itself
	}
	
	// wait to be notified that the main Canvas is closed
	//pthread_cond_wait();
	
	// start up the Reader and Writer pthreads
	for(int i = 0; i < numWriters; i++) {
		writers[i].start();
		sleep(0.3);
	}
	for(int i = 0; i < numReaders; i++) {
		readers[i].start();
		sleep(0.3);
	}
	
	// wait for the main Canvas to be closed
	while(can.isOpen()) {
		can.sleep();
	}
	
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
}

int main(int argc, char* argv[]) {
	// number of readers defaults to 6
	int nread  = ( (argc > 1) && (atoi(argv[1]) > 0) && (atoi(argv[1]) <= 8) ) ? atoi(argv[1]) : 6;
	// number of writers defaults to 2
	int nwrite = ( (argc > 2) && (atoi(argv[1]) > 0) && (atoi(argv[1]) <= 8) ) ? atoi(argv[2]) : 2;
	c.run(readWriteFunction,nread,nwrite);
	return 0;
}