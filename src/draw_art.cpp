#include <iostream>
#include "canvas.h"

using namespace std;
using namespace agl;

int main(int argc, char **argv) {
    canvas drawer(100, 100);
    drawer.background(255, 255, 255);
    drawer.color(0, 0, 0);

    //draw circle
    drawer.draw_circle(50, 50, 45);
    drawer.save("circle.png");

    // draw rectangle
    drawer.draw_rectangle(50, 50, 70, 85);
    drawer.save("rectangle.png");

    // set width
    drawer.set_width(10);
    drawer.draw_circle(50, 50, 30);
    drawer.begin(LINES);
    drawer.vertex(0, 50);
    drawer.vertex(99, 50);
    drawer.vertex(50, 0);
    drawer.vertex(50, 99);
    drawer.end();
    drawer.save("width_10.png");

    // dotted line
    drawer.dotted(0.7);
    drawer.draw_circle(50, 50, 30);
    drawer.begin(LINES);
    drawer.vertex(0, 50);
    drawer.vertex(99, 50);
    drawer.vertex(50, 0);
    drawer.vertex(50, 99);
    drawer.end();
    drawer.save("dotted.png");

    // artwork: the Deathly Hallows
    canvas drawer2(200, 200);
    drawer2.background(206, 174, 148);
    drawer2.color(93, 72, 50);
    drawer2.set_width(10);
    drawer2.dotted(0.6);
    drawer2.begin(LINES);
    drawer2.vertex(90, 15);
    drawer2.vertex(10, 170);
    drawer2.vertex(10, 170);
    drawer2.vertex(180, 170);
    drawer2.vertex(180, 170);
    drawer2.vertex(90, 15);
    drawer2.end();
    drawer2.dotted(0.3);
    drawer2.draw_circle(98, 123, 38);
    drawer2.dotted(0.8);
    drawer2.begin(LINES);
    drawer2.vertex(90, 15);
    drawer2.vertex(90, 170);
    drawer2.end();
    drawer2.save("deathly-hallows.png");
}

