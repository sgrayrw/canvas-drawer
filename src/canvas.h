#pragma once

#include <string>
#include <vector>
#include "ppm_image.h"

using namespace std;

namespace agl {
    enum PrimitiveType {
        UNDEFINED, LINES, TRIANGLES
    };

    struct point {
        int x;
        int y;
        ppm_pixel color;
    };

    class canvas {
    public:
        canvas(int w, int h);

        virtual ~canvas();

        // Save to file
        void save(const std::string &filename);

        // Draw primitives with a given type (either LINES or TRIANGLES)
        // For example, the following draws a red line followed by a green line
        // begin(LINES);
        //    color(255,0,0);
        //    vertex(0,0);
        //    vertex(100,0);
        //    color(0,255,0);
        //    vertex(0, 0);
        //    vertex(0,100);
        // end();
        void begin(PrimitiveType type);

        void end();

        // Specifiy a vertex at raster position (x,y)
        // x corresponds to the column; y to the row
        void vertex(int x, int y);

        // Specify a color. Color components are in range [0,255]
        void color(unsigned char r, unsigned char g, unsigned char b);

        // Fill the canvas with the given background color
        void background(unsigned char r, unsigned char g, unsigned char b);

        //******************* additional features below *******************

        // draw a circle with given center (x, y) and radius
        void draw_circle(int x, int y, int radius);

        // draw a rectangel with given center (x, y), width, and height
        void draw_rectangle(int x, int y, int width, int height);

        // set the width of shape borders
        void set_width(int width);

        // Resemble dotted lines by randomly missing some pixels, and expect
        // to only draw *ratio* amount of pixels in total
        void dotted(float ratio);

    private:
        ppm_image _canvas;
        ppm_pixel curr_color = {0, 0, 0};
        ppm_pixel curr_background = {0, 0, 0};
        PrimitiveType curr_type = UNDEFINED;
        vector<point> curr_points;
        int curr_width = 1;
        float curr_ratio = 1;

        static ppm_pixel linear_interpolate(const ppm_pixel &c1, const ppm_pixel &c2, float t);

        void draw_line(const point& start, const point& end);
        void draw_line_vertical(int ax, int ay, int bx, int by, const ppm_pixel &cx, const ppm_pixel &cy);
        void draw_line_horizontal(int ax, int ay, int bx, int by, const ppm_pixel &cx, const ppm_pixel &cy);
        void draw_line_flat(int ax, int ay, int bx, int by, const ppm_pixel& cx, const ppm_pixel& cy);
        void draw_line_steep(int ax, int ay, int bx, int by, const ppm_pixel& cx, const ppm_pixel& cy);

        void draw_triangle(const point &a, const point &b, const point &c);
        static int line_distance(const point &start, const point &end, const point &p);

        void draw_circle_internal(int x, int y, int radius);
        void draw_rectangle_internal(int x, int y, int width, int height);
    };
}
