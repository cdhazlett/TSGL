// #define RANDOMCOLORS

#include <tsgl.h>
using namespace tsgl;
#include <thread> // std::this_thread::sleep_for
#include <chrono> // std::chrono::seconds
using namespace std;

Canvas *canvasPtr;


void cube_demo_function(Canvas &can, int numProcs)
{
    const int WW = can.getWindowWidth(), WH = can.getWindowHeight();
    int a, b, c, d;

    can.setCameraPosition(0, 0, 10);
    can.setCameraFocusPoint(0, 0, 0);
    can.setCameraPerspective(90.0f, 0.1f, 1000.0f);

    // can.enable2D();


    // Rectangle* testRect = new Rectangle(0, 0, 100, 100, BLUE, BLUE);
    // // testRect->scale(100,100,1);
    // // Rectangle* testRect2 = new Rectangle(100, 100, 1, 1, GREEN, GREEN);
    // // can.add(testRect);
    // // can.add(testRect2);

    // Ellipse* eltest = new Ellipse(20, 20, 20, 40, RED, RED);
    // can.add(eltest);

    // Circle* cltest = new Circle(100,100,50,GREEN,GREEN);
    // can.add(cltest);

    // // Triangle* tatest = new Triangle(0,0,20,20,20,10,RED, PURPLE);
    // Triangle* tatest = new Triangle(10, 10, 50, 50, .5, RED, RED);
    // can.add(tatest);


    Cube* cubetest = new Cube(2,.2,.2,1,1,.5,RED);

    // Object3D* cubetest = new Object3D("assets/models/cube.dae");
    // cubetest->scale(100,100,100);
    // cubetest->rotate(45, 0,1,0);
    // cubetest->scale(200,200,200);
    // cubetest->translate(200,200,0);
    can.add(cubetest);

    while (can.isOpen())
    {
        can.sleep();
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
