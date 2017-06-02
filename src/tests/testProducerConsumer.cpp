/**
 * testProducerConsumer.cpp contains the code that shows the visualization for the Producer-Consumer problem using TSGL and pthreads.
 * It utilizes a custom Queue class to make the shared buffer.
 * Producer and Consumer classes have been made in order to make the Producers and Consumers
 * A Thread class has been made in order to have an encapsulated pthread (which the Producer and Consumer class both inherit from).
 * Usage: ./testProducerConsumer [numberOfProducers] [numberOfConsumers]
 * (Update NOTE: The Canvases may be blank at startup sometimes. If this happens, kill the process in the terminal (using ^C), and 
 *	 	         rerun testProducerConsumer.) 
 */

#include <stdlib.h>
#include <unistd.h>
#include "ProducerConsumer/Producer.h"
#include "ProducerConsumer/Consumer.h"

	
// constants for drawing
const int INNERRAD = 75;  // radius of the inner circle
const int OUTERRAD = 150; // radius of the outercircle
const int CAPACITY = 8;

/**
 * display() method draws the color data that is currently in the shared buffer.
 * @param: can, a reference to the Canvas to draw on.
 * @param: centerX, an int representing the center x-coordinate of the colored "object" to draw. 
 * @param: sharedBuffer, a reference to the Queue shared between the Producers and Consumers.
 */
void display(Canvas & can, int centerX, Queue<ColorInt> & sharedBuffer) {
// partial representation of a circular Queue
	int centerY = can.getWindowHeight()/2;

	for(int i = 0; i < CAPACITY; i++) {
		
		ColorFloat c = sharedBuffer.getArray()[i]; // get the array of all colors in the sharedBuffer
		
		float itAngle = (i*2*PI + PI)/CAPACITY; // angle of item

		can.drawCircle(100*cos(itAngle)+centerX, -100*sin(itAngle)+centerY, 20, 50, c, true); // draw the item as a circle
	}
}

void preDisplay(Canvas & can, int centerX) {
	int centerY = can.getWindowHeight()/2;
	for(int i = 0; i < CAPACITY; i++) {
		float langle = (i*2*PI)/CAPACITY; // line angle
		can.drawLine(-INNERRAD*sin(langle)+centerX, INNERRAD*cos(langle)+centerY, -OUTERRAD*sin(langle)+centerX, OUTERRAD*cos(langle)+centerY);
	}
	can.drawCircle(centerX, centerY, INNERRAD, CAPACITY, BLACK, false);
	can.drawCircle(centerX, centerY, OUTERRAD, CAPACITY, BLACK, false);

}

//Global constants
const int WINDOW_WIDTH = 600, WINDOW_HEIGHT = 500, MAX_DATA = 8; //Size of Canvas and limit on amount of data to be stored in Queue 
Canvas queueDisplay(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, "Producer-Consumer", FRAME * 13);  //Canvas to draw on
Canvas legendDisplay(700, 400, 400, 400, "Legend");  //Legend
Queue<ColorInt> sharedBuffer(MAX_DATA, queueDisplay);  //Shared buffer (has colored data)

//Main method
int main(int argc, char * argv[]) {
	int numProducers, numConsumers;  //Number of producers, consumers
	bool paused = false;   //Flag that determines whether to pause the animation or not

	//Check the command line
	numProducers = (argc > 1) ? atoi(argv[1]) : 5; 
	numConsumers = (argc > 2) ? atoi(argv[2]) : 5;

	//Set to max number of producers && consumers if negative, zero values
	if(numProducers <= 0 || numConsumers <= 0) { 
		std::cout << "Invalid input! Now changing to max default values..." << std::endl; 
		numProducers = numConsumers = 8;	
	} else if(numProducers > 8 || numConsumers > 8) {
		std::cout << "Too many producers/consumers! Now changing to max default values..." << std::endl;
		numProducers = numConsumers = 8;	
	}
	
	srand(time(NULL)); // seed the random number generator
	
	Producer * pro = new Producer[numProducers]; //Array of Producers
	Consumer * con = new Consumer[numConsumers];  //Array of Consumers
		
	srand(time(NULL));	//Seed the random number generator (to make random colors)

	//Pause the animation if space bar is pressed
	queueDisplay.bindToButton(TSGL_SPACE, TSGL_PRESS, [&paused]() {
		paused = !paused;
	});
	
	//Fire up the visualization
    queueDisplay.start();

	//Fire up the Legend
	legendDisplay.start(); 	
	
	queueDisplay.setBackgroundColor(WHITE);	
	legendDisplay.setBackgroundColor(WHITE);

		
	//Fill the arrays of Producers and Consumers with Producer and Consumer objects
	for(int i = 0; i < numProducers; i++) {
		pro[i] = Producer(sharedBuffer, i, queueDisplay);	
	}

	for(int j = 0; j < numConsumers; j++) {
		con[j] = Consumer(sharedBuffer, j, queueDisplay);	
	}

	int colorChanger = 0; //Changes color on Legend
	//Draw the Legend	
	for(int i = 0; i < 8; i++) {
		legendDisplay.drawCircle(30, (40 * (i + 1) - 10), 15, 32, Colors::highContrastColor(i));	
		legendDisplay.drawText("is Producer " + to_string(i), 60, (40 * (i + 1)), 15, BLACK);				
	}
	legendDisplay.drawText("Consumers are on the right side ", 60, 360, 15, BLACK);	
		
	
	//Start up the pthreads for Producers and Consumers
	for(int k = 0; k < numProducers; k++) {
		pro[k].start();
		sleep(0.3);
	}
	
	for(int l = 0; l < numConsumers; l++) {
		con[l].start();
		sleep(0.3);
	}
	
	preDisplay(queueDisplay, WINDOW_WIDTH/2);
	for(int m = 0; m < numProducers; m++) {  //Draw the Producers onto the Canvas				
		pro[m].draw(queueDisplay);
	}
	
	//Drawing loop	
 	while(queueDisplay.isOpen()) {
			legendDisplay.sleep();
			legendDisplay.sleepFor(0.5);
			legendDisplay.drawCircle(30, 360, 15, 32, Colors::highContrastColor(colorChanger));  //Consumer on Legend
			colorChanger++;
	}
		
	//Now join them
	for(int o = 0; o < numProducers; o++) {	//Join the pthreads for the Producers	
		pro[o].join();
	}	
	
	for(int p = 0; p < numConsumers; p++) {   //Join the pthreads for the Consumers
		con[p].join();
	}  

	//Cleanup the Canvas and free up memory
	delete [] pro;
	delete [] con;
	pro = NULL;
	con = NULL;

	queueDisplay.wait();
	if(legendDisplay.isOpen()) {  //Close up the Legend Canvas if it's still open, else call wait().
		legendDisplay.stop();
	} else {
		legendDisplay.wait();
	}
	return 0;
}
