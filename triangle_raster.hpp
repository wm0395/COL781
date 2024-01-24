#ifndef TRIANGLE_RASTER_H
#define TRIANGLE_RASTER_H

#include <vector>
#include <utility>
#include <iostream>

using namespace std;

class line {
public:
    pair<float, float> a, b;
    line();
    line(pair<float, float> a, pair<float, float> b);

    bool isLeft(pair<float, float> &p);

    float intercept_x(float y);

    float intercept_y(float x);
};

class triangle {
public:
    pair<float, float> a, b, c;
    line A, B, C;

    triangle();
    triangle(pair<float, float> a, pair<float, float> b, pair<float, float> c);

    bool isInside(pair<float, float> p);

private:
    void make_anti_clockwise();
};

class screen {
public:
    int width, height;
    vector<vector<float>> pixels;

    screen(int w, int h);

    const vector<char> colors;

    void print();

    void clear();

    void rasterize(triangle &T, string cmp = "inline", float color = 1, int sr = 1);

private:
    void set_pixel(int x, int y, float val);

    void rasterize_inline(triangle &T, float color = 1);

    void rasterize_tile(triangle &T, float color = 1);

    void rasterize_incremental(triangle &T, float color = 1);

    float sample_aa(int i, int j, int sr, float &color, triangle &T);

    void rasterize_aa(triangle &T, int sr = 1, float color = 1);
};

#endif
