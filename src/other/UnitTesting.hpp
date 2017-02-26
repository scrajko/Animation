
#include <OpenGL_common_SFML.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "Common.hpp"
#include "Coordinate.hpp"
#include "GraphicArray.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Frame.hpp"
#include "Color.hpp"
#include "Brush.hpp"
#include "lodepng.h"
#include <random>
#include <ctime>

// Used just for this file:
#include <ctime>
#include <fstream>
#include <map>

/**
 * An unrefined way to implement a form of unit testing.
 * The functions tested must be no-arg bool functions.
 * To add a function to the list to be tested,
 * define it in this file, and add the name of the function
 * to the std::vector unit_test_functions in main()
 */

const std::string Unit_Test_Dir = "./unit_tests/";

bool test_success()
{
    return 1 == 1;
}
bool test_failure()
{
    return 1 == 0;
}

/**
 * Tests the brush created from the distance procedure
 * against the first brush that uses the linear "AlphaCircle" style,
 * to check against fencepost errors
 */
bool test_brush_mode1()
{
    unsigned Width = 500, Height = 500;
    std::vector<unsigned char> pixels1(4*Width*Height);
    std::vector<unsigned char> pixels2(4*Width*Height);

    Brush brush(1);
    brush.color  = Color::Red;
    brush.radius = 75;

    // First paint with AlphaCircle
    brush.mode   = BrushMode::AlphaCircle;
    brush.paint(pixels1, Coordinate(250, 250), Width, Height);

    // Then separately paint with CircleAlphaArray
    brush.distance_alpha_values = load_alpha_distance_array("default_brush_linear_255.brush");
    brush.mode = BrushMode::CircleAlphaArray;
    brush.paint(pixels2, Coordinate(250, 250), Width, Height);

    /*
       Check for exact equality:
       This reason this is failing is most likely
       due to the lack of rounding from BrushMode::CircleAlphaArray,
       or slightly off positioning
     */
    for (std::size_t i = 0; i < pixels1.size(); i++)
    {
        if (pixels1[i] != pixels2[i])
            return false;
    }
    return true;
}

bool test_brush_stroke()
{
    return false;
}
bool test_selection_frame()
{
    return false;
}

#if UNIT_TEST_MODE
typedef bool (*UnitTestFunction)();
int main()
{

    // Note for future: The parsing function depends that
    // all tokens here are separate, ex no '={', make it be '= {'
    /// ADD TEST FUNCTIONS HERE:
    std::vector<UnitTestFunction> unit_test_functions = {
        test_success,
        test_failure,
        test_brush_mode1
    };

    // Map is created by the program reading its own source code
    // at runtime...
    std::map<UnitTestFunction, std::string> func_str_map;
    std::ifstream source_code("UnitTesting.hpp");
    std::string token;

    while (source_code >> token)
    {
        if (token == "unit_test_functions")
            break;
    }
    source_code >> token; // skip Equals Sign
    source_code >> token; // skip opening brace
    std::size_t function_index = 0;
    while (source_code >> token)
    {
        if (token == "};")
            break;
        // remove comma:
        if (token.back() == ',')
            token.pop_back();

        // Add to function->string map
        func_str_map[unit_test_functions[function_index++]] = token;

    }

    std::ofstream f("unit_test_results.txt");
    for (std::size_t i = 0; i < unit_test_functions.size(); i++)
    {
        bool success = unit_test_functions[i]();
        f << func_str_map[unit_test_functions[i]] << " : " << success << '\n';
    }
    f << '\n';

    // Write time at end of tile:

    // http://www.cplusplus.com/reference/ctime/localtime/
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    f << asctime(timeinfo);
}

#endif // UNIT_TEST_MODE
