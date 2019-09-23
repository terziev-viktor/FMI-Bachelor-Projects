#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <vector>
#include <string.h>
#include <map>
#include <set>

#include "HelloVulkan.h"

int main()
{
    try
    {
        HelloVulkan app;
        if (app)
        {
            app.run();
        }
    }
    catch(const std::runtime_error & e)
    {
        std::cerr << "Runtime error:\n";
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    catch(const std::exception& e)
    {
        std::cerr << "exception:\n";
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

