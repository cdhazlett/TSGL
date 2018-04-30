#define RANDOMCOLORS

#include <tsgl.h>
using namespace tsgl;
#include <thread> // std::this_thread::sleep_for
#include <chrono> // std::chrono::seconds
using namespace std;

Canvas *canvasPtr;

#define PI 3.14159265


int degrees = 0;
int distanceX, distanceY = 0;
float zoomLevel = 10.0;
int startX, stopX, startY, stopY = 0;
float camX, camZ = 10.f;
void updateCameraTrack()
{
    camX = sin(degrees * PI / 180) * zoomLevel;
    camZ = cos(degrees * PI / 180) * zoomLevel;

    degrees = degrees % 360;
}
void mouseDown()
{
    startX = canvasPtr->getMouseX();
    startY = canvasPtr->getMouseY();
}
void mouseUp()
{
    stopX = canvasPtr->getMouseX();
    stopY = canvasPtr->getMouseY();
    distanceX = (stopX - startX) / 10;
    distanceY = (stopY - startY) / 10;
}

void cube_demo_function(Canvas &can, int numProcs)
{
    const int WW = can.getWindowWidth(), WH = can.getWindowHeight();
    int a, b, c, d;

    can.setCameraPosition(10, 0, 10);
    can.setCameraFocusPoint(0, 0, 0);
    can.setCameraPerspective(90.0f, 0.1f, 100.0f);

    int numCubes = 9*9*9;
    Object3D *cubeArr[numCubes];


    int k;
    for (k = 0; k < numCubes; k++)
    {
        cubeArr[k] = new Object3D("/Users/christiaanhazlett/TSGL/assets/models/cube.dae");
        cubeArr[k]->scale(.5, .5, .5);
        cubeArr[k]->translate(k%9 * 1.5, -(float)(int)((k/81)%81) * 1.5, -(float)(int)((k/9)%9) * 1.5);
        cubeArr[k]->translate(-8, 0, 0);

        // can.add(cubeArr[k]);
    }


    // int numProcs = 9;
    k = 0;
    while (can.isOpen())
    {

        int z = 0;
        int perProc = (numCubes / numProcs);
        for (z=0; z<numProcs; z++) {
            int cur = perProc*z + k;
            if (cur < numCubes) can.add(cubeArr[cur]);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        k++;

        // if (k<numCubes) {
        //     can.add(cubeArr[k]);
        //     k++;
        // }



        // for (i = 0; i<numCubes; i++) {

        // }


        can.sleep();
        // for (k = 0; k < numCubes; k++)
        // {
        //     cubeArr[k]->rotate(-5 * rotArr[k * 3], rotArr[k * 3], rotArr[k * 3 + 1], rotArr[k * 3 + 2]);
        // }
    }
}

//Takes command-line arguments for the width and height of the screen
int main(int argc, char *argv[])
{
    int numProcs =  (argc > 1) ? atoi(argv[1]) : 1;
    int w = 0.9 * Canvas::getDisplayHeight();
    int h = w;
    w = h = 700; //If not, set the width and height to a default value
    Canvas c(-1, -1, w, h, "TSGL Demo: Cool Cubes");
    canvasPtr = &c;

    c.run(&cube_demo_function, numProcs);
}
