/*
 * testAlphaLangton.cpp
 *
 *  Created on: May 27, 2015
 *      Author: cpd5
 */

#include <cmath>
#include <complex>
#include <iostream>
#include <omp.h>
#include <queue>
#include <tsgl.h>

#ifdef _WIN32
const double PI = 3.1415926535;
#else
const double PI = M_PI;
#endif
const double RAD = PI / 180;  // One radian in degrees

// Some constants that get used a lot
const int NUM_COLORS = 256, MAX_COLOR = 255;

// Shared values between langton functions
enum direction {
    UP = 0,
    RIGHT = 1,
    DOWN = 2,
    LEFT = 3
};

typedef CartesianCanvas Cart;
typedef std::complex<long double> complex;

const int WINDOW_W = 400*3, WINDOW_H = 300*3, BUFFER = WINDOW_W * WINDOW_H * 2;

const int IPF = 1000;  //For those functions that need it

float randfloat(int divisor = 10000) {
    return (rand() % divisor) / (float) divisor;
}

/*!
 * \brief Simulates 4 Langton's Ants with alpha transparency used for drawing and a couple
 * other neat features.
 * \details Mostly the same as langtonColonyFunction(), but with a couple key differences:
 * - Each ant is drawn with an alpha (transparency) value of 16 (~0.06)
 * - We set up an additional timer \b pulse to keep track of intervals between clearing the screen
 * - We set up a function \b tempo, which resets the \b pulse timer, sets its interval to the
 * time since the last reset, and makes the Canvas clear itself at that interval.
 * - We bind the left mouse button and the enter button to the described \b tempo function.
 * - We bind the space button to clearing the Canvas.
 * - After all the ants are moved on a given frame, if the \b pulse timer is expired, we clear
 * the screen.
 * .
 * \param can Reference to the Canvas being drawn to
 */
void alphaLangtonFunction(Canvas& can) {
    const int IPF = 5000,                         // Iterations per frame
              WINDOW_W = can.getWindowWidth(),    // Set the window sizes
              WINDOW_H = can.getWindowHeight(),
              RADIUS = WINDOW_H / 6;              // How far apart to space the ants
    bool* filled = new bool[WINDOW_W * WINDOW_H]();  // Create an empty bitmap for the window
    int xx[4], yy[4], dir[4], red[4], green[4], blue[4];
    xx[0] = WINDOW_W / 2 - RADIUS;
    yy[0] = WINDOW_H / 2;
    red[0] = MAX_COLOR;
    green[0] = 0;
    blue[0] = 0;
    xx[1] = WINDOW_W / 2;
    yy[1] = WINDOW_H / 2 - RADIUS;
    red[1] = 0;
    green[1] = 0;
    blue[1] = MAX_COLOR;
    xx[2] = WINDOW_W / 2 + RADIUS;
    yy[2] = WINDOW_H / 2;
    red[2] = 0;
    green[2] = MAX_COLOR;
    blue[2] = 0;
    xx[3] = WINDOW_W / 2;
    yy[3] = WINDOW_H / 2 + RADIUS;
    red[3] = MAX_COLOR;
    green[3] = 0;
    blue[3] = MAX_COLOR;

    for (int i = 0; i < 4; i++) {
        dir[i] = i;
    }
    Timer pulse(28.72 / 60);
    double time = pulse.getTime();

    auto tempo = [&can, &pulse, &time]() {
        std::cout << (pulse.getTime() - time) << std::endl;
        pulse.reset(pulse.getTime() - time);
        time = pulse.getTime();
        can.clear();
    };
    can.bindToButton(TSGL_MOUSE_LEFT, TSGL_PRESS, tempo);
    can.bindToButton(TSGL_ENTER, TSGL_PRESS, tempo);
    can.bindToButton(TSGL_SPACE, TSGL_PRESS, [&can]() {
        can.clear();
    });

    while (can.getIsOpen()) {
        can.sleep();  //Removed the timer and replaced it with an internal timer in the Canvas class
        for (int i = 0; i < IPF; i++) {
            for (int j = 0; j < 4; j++) {
                if (filled[xx[j] + WINDOW_W * yy[j]]) {
                    dir[j] = (dir[j] + 1) % 4;
                    can.drawPoint(xx[j], yy[j], ColorInt(MAX_COLOR / 2, MAX_COLOR / 2, MAX_COLOR / 2, 16));
                } else {
                    dir[j] = (dir[j] + 3) % 4;
                    can.drawPoint(xx[j], yy[j], ColorInt(red[j], green[j], blue[j], 16));
                }
            }
            for (int j = 0; j < 4; j++) {
                filled[xx[j] + WINDOW_W * yy[j]] ^= true;
                switch (dir[j]) {
                    case UP:
                        yy[j] = (yy[j] > 0) ? yy[j] - 1 : WINDOW_H - 1;
                        break;
                    case RIGHT:
                        xx[j] = (xx[j] < WINDOW_W - 1) ? xx[j] + 1 : 0;
                        break;
                    case DOWN:
                        yy[j] = (yy[j] < WINDOW_H - 1) ? yy[j] + 1 : 0;
                        break;
                    case LEFT:
                        xx[j] = (xx[j] > 0) ? xx[j] - 1 : WINDOW_W - 1;
                        break;
                    default:
                        break;
                }
            }
            if (pulse.pastPeriod()) {
                can.clear();
            }
        }
    }
    delete filled;
}

int main() {
    glfwInit();  // Initialize GLFW
    Canvas::setDrawBuffer(GL_FRONT_AND_BACK);	// For Patrick's laptop
    Canvas c15(0, 0, 960, 960, 30000, "", FRAME);
    c15.setBackgroundColor(BLACK);
    c15.start();
    alphaLangtonFunction(c15);
    c15.close();
    glfwTerminate();  // Release GLFW
}
