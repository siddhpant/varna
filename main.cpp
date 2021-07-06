// Import from standard library.
#include <iostream>
#include <vector>

// Include helper functions.
#include "svg-path/svg_path_parser.hpp"


int main(void) {
    // Currently just used for debugging tbh

    char path[] = "drawing.svg";
    SvgImage parsed_svg = get_parsed_svg(path);

    std::cout << "Width = " << parsed_svg.width << "\n";
    std::cout << "Height = " << parsed_svg.height << "\n";

    std::vector<SvgShapeUnlinked> shapes = parsed_svg.shapes;

    for(SvgShapeUnlinked shape : shapes) {
        std::cout << "\nID = " << shape.id;
        std::cout << "\nopacity = " << shape.opacity;

        for(SvgPathUnlinked path : shape.paths) {
            std::cout << "\nNum points = " << path.npts;
            std::cout << "\nPoints:\n";
            for(Point pt : path.pts){
                std::cout << "x = " << pt.x << ", y = " << pt.y << "\n";
            }

            std::cout << "\n----------------\n" << std::endl;

            for(CubicBeizerCurve curve : path.curves) {
                std::cout << "Starting = (" << curve.start.x << ", "
                          << curve.start.y << ")\n";

                std::cout << "Starting Control Point = ("
                          << curve.control_start.x << ", "
                          <<curve.control_start.y << ")\n";

                std::cout << "Ending Control Point = ("
                          << curve.control_end.x << ", "
                          << curve.control_end.y << ")\n";

                std::cout << "Ending = (" << curve.end.x << ", "
                          << curve.end.y << ")\n";

                std::cout << "~~~~~~~~~~~~~~~~~~~~~\n\n";

            }
        }

        std::cout << "\n=============================================\n\n\n\n";
    }
    return 0;
}
