#include "canvas.h"
#include <cassert>

using namespace std;
using namespace agl;

canvas::canvas(int w, int h) : _canvas(w, h) {
    // default background is black
    background(0, 0, 0);
}

canvas::~canvas() = default;

void canvas::save(const std::string &filename) {
    _canvas.save(filename);

    // clear the canvas and vertices
    curr_points.clear();
    background(curr_background.r, curr_background.g, curr_background.b);
}

void canvas::begin(PrimitiveType type) {
    curr_type = type;
}

void canvas::end() {
    switch (curr_type) {
        case LINES:
            // lines should be drawn in pairs
            if (curr_points.size() % 2 != 0) {
                cout << "warn: odd number of vertices, the last vertex will be ignored" << endl;
            }
            for (int i = 0; i < curr_points.size() - 1; ++i) {
                draw_line(curr_points[i], curr_points[i + 1]);
            }
            break;
        case TRIANGLES:
            // triangles should be drawn in triple
            if (curr_points.size() % 3 != 0) {
                cout << "warn: number of vertices is not multiple of 3" << endl;
            }
            for (int i = 0; i < curr_points.size() - 2; ++i) {
                draw_triangle(curr_points[i], curr_points[i + 1], curr_points[i + 2]);
            }
            break;
        case UNDEFINED:
            break;
    }
}

void canvas::vertex(int x, int y) {
    assert(curr_type != UNDEFINED);
    x = min(max(x, 0), _canvas.width() - 1);
    y = min(max(y, 0), _canvas.height() - 1);
    curr_points.push_back({x, y, curr_color});
}

void canvas::color(unsigned char r, unsigned char g, unsigned char b) {
    curr_color = {r, g, b};
}

void canvas::background(unsigned char r, unsigned char g, unsigned char b) {
    curr_background = {r, g, b};

    for (int i = 0; i < _canvas.height(); ++i) {
        for (int j = 0; j < _canvas.width(); ++j) {
            _canvas.set(i, j, {r, g, b});
        }
    }
}

ppm_pixel canvas::linear_interpolate(const ppm_pixel& c1, const ppm_pixel& c2, float t) {
    return c1 * (1 - t) + c2 * t;
}

void canvas::draw_line(const point &start, const point &end) {
    int ax = start.x;
    int ay = start.y;
    int bx = end.x;
    int by = end.y;
    ppm_pixel cx = start.color;
    ppm_pixel cy = end.color;
    int W = bx - ax;
    int H = by - ay;

    if (W == 0) {
        draw_line_vertical(ax, ay, bx, by, cx, cy);
    } else if (H == 0) {
        draw_line_horizontal(ax, ay, bx, by, cx, cy);
    } else if (abs(W) > abs(H)) {
        (ax < bx) ? draw_line_flat(ax, ay, bx, by, cx, cy) : draw_line_flat(bx, by, ax, ay, cx, cy);
    } else {
        (ay < by) ? draw_line_steep(ax, ay, bx, by, cx, cy) : draw_line_steep(bx, by, ax, ay, cx, cy);
    }
}

void canvas::draw_line_vertical(int ax, int ay, int bx, int by, const ppm_pixel& cx, const ppm_pixel& cy) {
    assert(ax == bx);
    int miny = min(ay, by);
    int maxy = max(ay, by);

    for (int y = miny; y <= maxy; ++y) {
        float t = ((float) y - miny) / (maxy - miny);
        ppm_pixel c = linear_interpolate(cx, cy, t);
        _canvas.set(y, ax, c);
    }
}

void canvas::draw_line_horizontal(int ax, int ay, int bx, int by, const ppm_pixel& cx, const ppm_pixel& cy) {
    assert(ay == by);
    int minx = min(ax, bx);
    int maxx = max(ax, bx);

    for (int x = minx; x <= maxx; ++x) {
        float t = ((float) x - minx) / (maxx - minx);
        ppm_pixel c = linear_interpolate(cx, cy, t);
        _canvas.set(ay, x, c);
    }
}

// where |W| < |H|
void canvas::draw_line_flat(int ax, int ay, int bx, int by, const ppm_pixel& cx, const ppm_pixel& cy) {
    int y = ay;
    int W = bx - ax;
    int H = by - ay;
    int inc = 1;
    if (H < 0) {
        inc = -1;
        H = -H;
    }
    int F = 2 * H - W;

    for (int x = ax; x <= bx; ++x) {
        float t = ((float) x - ax) / (bx - ax);
        ppm_pixel c = linear_interpolate(cx, cy, t);
        _canvas.set(y, x, c);
        if (F > 0) {
            y += inc;
            F += 2 * (H - W);
        } else {
            F += 2 * H;
        }
    }
}

// where |W| > |H|
void canvas::draw_line_steep(int ax, int ay, int bx, int by, const ppm_pixel& cx, const ppm_pixel& cy) {
    int x = ax;
    int W = bx - ax;
    int inc = 1;
    if (W < 0) {
        inc = -1;
        W = -W;
    }
    int H = by - ay;
    int F = 2 * W - H;

    for (int y = ay; y <= by; ++y) {
        float t = ((float) y - ay) / (by - ay);
        ppm_pixel c = linear_interpolate(cx, cy, t);
        _canvas.set(y, x, c);
        if (F > 0) {
            x += inc;
            F += 2 * (W - H);
        } else {
            F += 2 * W;
        }
    }
}

void canvas::draw_triangle(const point &a, const point &b, const point &c) {
    int xmin = min(a.x, min(b.x, c.x));
    int ymin = min(a.y, min(b.y, c.y));
    int xmax = max(a.x, max(b.x, c.x));
    int ymax = max(a.y, max(b.y, c.y));

    int f_alpha = line_distance(b, c, a);
    int f_beta = line_distance(a, c, b);
    int f_gamma = line_distance(a, b, c);

    point hidden = {-1, -1, ppm_pixel()};

    for (int y = ymin; y <= ymax; ++y) {
        for (int x = xmin; x <= xmax; ++x) {
            point p = {x, y, ppm_pixel()};
            float alpha = (float) line_distance(b, c, p) / f_alpha;
            float beta = (float) line_distance(a, c, p) / f_beta;
            float gamma = (float) line_distance(a, b, p) / f_gamma;
            if (alpha >= 0 && beta >= 0 && gamma >= 0) {
                ppm_pixel color = a.color * alpha + b.color * beta + c.color * gamma;
                if (alpha > 0 || f_alpha * line_distance(b, c, hidden) > 0 ||
                        beta > 0 || f_beta * line_distance(a, c, hidden) > 0 ||
                        gamma > 0 || f_gamma * line_distance(a, b, hidden) > 0) {
                    _canvas.set(y, x, color);
                }
            }
        }
    }
}

// "distance" from point to line using canonical line equation
int canvas::line_distance(const point &start, const point &end, const point &p) {
    int ax = start.x;
    int ay = start.y;
    int bx = end.x;
    int by = end.y;
    int W = bx - ax;
    int H = by - ay;

    return H * (p.x - ax) - W * (p.y - ay);
}


