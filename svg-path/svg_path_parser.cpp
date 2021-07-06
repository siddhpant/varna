// Include the associated header file.
#include "svg_path_parser.hpp"

// Import from standard library.
#include <iostream>
#include <vector>

// Includes and definitions needed for using nanosvg.h.
#include <cstdio>
#include <cstring>
#include <cmath>
#define NANOSVG_ALL_COLOR_KEYWORDS
#define NANOSVG_IMPLEMENTATION

// Import external third party headers.
#include "../third-party/nanosvg.h"

// It would be helpful to open line 165 of the nanosvg.h header file.


std::vector<SvgPathUnlinked> vectorize_paths(NSVGpath* linked_paths) {
    // Vectorize NSVGpath linked list.
    std::vector<SvgPathUnlinked> vectorized_paths;

    while(linked_paths != NULL) {
        // Copy all the memebrs to our new struct, and then push to vector.
        SvgPathUnlinked path;

        path.npts = linked_paths->npts;

        for (int i = 0; i < path.npts - 1; i += 3) {
            float* p = &linked_paths->pts[i*2];

            Point start(p[0], p[1]);
            Point control_start(p[2], p[3]);
            Point control_end(p[4], p[5]);
            Point end(p[6], p[7]);

            CubicBeizerCurve curve(start, control_start, control_end, end);
            path.curves.push_back(curve);
        }

        float* first_point_x = linked_paths->pts;  // x coord. of the 1st point
        for(int i = 0; i < path.npts; i++) {
            // Make a Point object by giving x and y coordinate to the ctor.
            Point pt(*(first_point_x + 2*i), *(first_point_x + 2*i + 1));
            path.pts.push_back(pt);
        }

        path.closed = linked_paths->closed;
        memmove(path.bounds, linked_paths->bounds, 4);

        vectorized_paths.push_back(path);
        linked_paths = linked_paths->next;
    }

    return vectorized_paths;
} // End of vectorize_paths()


std::vector<SvgShapeUnlinked> vectorize_shapes(NSVGshape* linked_shapes) {
    // Vectorizes NSGshape linked list.
    std::vector<SvgShapeUnlinked> vectorized_shapes;

    while(linked_shapes != NULL){
        // Copy all the memebrs to our new struct, and then push to vector.
        SvgShapeUnlinked shape;

        memmove(shape.id, linked_shapes->id, 64);
        shape.fill = linked_shapes->fill;
        shape.stroke = linked_shapes->stroke;
        shape.opacity = linked_shapes->opacity;
        shape.strokeWidth = linked_shapes->strokeWidth;
        shape.strokeDashOffset = linked_shapes->strokeDashOffset;
        memmove(shape.strokeDashArray, linked_shapes->strokeDashArray, 8);
        shape.strokeDashCount = linked_shapes->strokeDashCount;
        shape.strokeLineJoin = linked_shapes->strokeLineJoin;
        shape.strokeLineCap = linked_shapes->strokeLineCap;
        shape.miterLimit = linked_shapes->miterLimit;
        shape.fillRule = linked_shapes->fillRule;
        shape.flags = linked_shapes->flags;
        memmove(shape.bounds, linked_shapes->bounds, 4);
        shape.paths = vectorize_paths(linked_shapes->paths);

        vectorized_shapes.push_back(shape);
        linked_shapes = linked_shapes->next;
    }

    return vectorized_shapes;
}  // End of vectorize_shapes()


/*
nsvgParseFromFile returns a pointer to a NSVGimage struct, which has the
dimensions of the SVG, and a linked list of the shapes in SVG.

The linked list is composed of NSVGshape, which has information about a
particular shape and in turn also consists of another linked list containing
the cubic beizer paths (NSVGpath) that make up that shape.

We will parse the SVG, vectorize the shapes and paths, and then return the
entire thing.
*/

SvgImage get_parsed_svg(const char* file_path, const char* units, float dpi) {
    // Get parsed svg from nanosvg, and then vectorize the components.
    NSVGimage* image = nsvgParseFromFile(file_path, units, dpi);

    SvgImage parsed_svg;
    parsed_svg.width = image->width;
    parsed_svg.height = image->height;
    parsed_svg.shapes = vectorize_shapes(image->shapes);

    nsvgDelete(image);  // free() the memory since it uses malloc() wizardry.
    return parsed_svg;  // This returns an object which doesn't need free().
}  // End of get_parsed_svg()


// End of file
