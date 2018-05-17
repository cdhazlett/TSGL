#define RANDOMCOLORS

#include <tsgl.h>
#include <thread> // std::this_thread::sleep_for
#include <chrono> // std::chrono::seconds
using namespace tsgl;

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

void cube_demo_function(Canvas &can)
{
    const int WW = can.getWindowWidth(), WH = can.getWindowHeight();
    int a, b, c, d;

    can.setCameraPosition(0, 0, 10);
    can.setCameraFocusPoint(0, 0, 0);
    can.setCameraPerspective(90.0f, 0.1f, 100.0f);

    int numCubes = 9;
    Object3D *cubeArr[numCubes];
    float rotArr[numCubes * 3];

    srand(static_cast<unsigned>(time(0)));

    int j;
    for (j = 0; j < numCubes * 3; j++)
    {
        rotArr[j] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    }

    int k;
    for (k = 0; k < numCubes; k++)
    {
        cubeArr[k] = new Object3D("/Users/christiaanhazlett/TSGL/assets/models/cube.dae");
        cubeArr[k]->scale(.5, .5, .5);
        cubeArr[k]->translate(k * 2, 0, 0);
        cubeArr[k]->translate(-8, 0, 0);

        can.add(cubeArr[k]);
    }

    while (can.isOpen())
    {
        can.sleep();
        for (k = 0; k < numCubes; k++)
        {
            cubeArr[k]->rotate(-5 * rotArr[k * 3], rotArr[k * 3], rotArr[k * 3 + 1], rotArr[k * 3 + 2]);
        }
    }
}

//Takes command-line arguments for the width and height of the screen
int main(int argc, char *argv[])
{
    int w = (argc > 1) ? atoi(argv[1]) : 0.9 * Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : w;
    w = h = 700; //If not, set the width and height to a default value
    Canvas c(-1, -1, w, h, "TSGL Demo: Cool Cubes");
    canvasPtr = &c;

    c.run(&cube_demo_function);
}