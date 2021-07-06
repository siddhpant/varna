#pragma once

// Import from standard library.
#include <iostream>
#include <vector>

// Import external third party headers.
#include "../third-party/nanosvg.h"


// It would be helpful to open line 165 of the nanosvg.h header file.


/*
We need to vectorize the linked lists for easy random access later.

For that, we first redefine the data types getting rid of the linked lists, and
then we make a vector of those.

See the nanosvg.h file for reference on the members of structs.
*/

typedef struct Point {
    float x, y;

    // Constructors
    Point(float a, float b): x(a), y(b) {}
    Point(const Point& pt): x(pt.x), y(pt.y) {}
} Point;

typedef struct CubicBeizerCurve {
    Point start, control_start, control_end, end;

    // Constructor
    CubicBeizerCurve(
        const Point& start, const Point& control_start,
        const Point& control_end, const Point& end
    ) :
        start(start), control_start(control_start),
        control_end(control_end), end(end)
    {}  // End of constructor
} CubicBeizerCurve;

typedef struct SvgPathUnlinked {
    std::vector<Point> pts;
    std::vector<CubicBeizerCurve> curves;
    int npts;
    char closed;
    float bounds[4];
} SvgPathUnlinked;

typedef struct SvgShapeUnlinked {
    char id[64];
    NSVGpaint fill;
    NSVGpaint stroke;
    float opacity;
    float strokeWidth;
    float strokeDashOffset;
    float strokeDashArray[8];
    char strokeDashCount;
    char strokeLineJoin;
    char strokeLineCap;
    float miterLimit;
    char fillRule;
    unsigned char flags;
    float bounds[4];
    std::vector<SvgPathUnlinked> paths;
} SvgShapeUnlinked;

typedef struct SvgImage {
    float width;
    float height;
    std::vector<SvgShapeUnlinked> shapes;
} SvgImage;


std::vector<SvgPathUnlinked> vectorize_paths(NSVGpath*);
std::vector<SvgShapeUnlinked> vectorize_shapes(NSVGshape*);
SvgImage get_parsed_svg(const char*, const char* = "px", float = 150.0F);


// End of file.
