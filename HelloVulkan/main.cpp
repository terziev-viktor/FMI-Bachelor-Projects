#include <iostream>

#include "HelloVulkan.h"

int main()
{
    HelloVulkan app;
    if (app)
    {
        app.run();
    }
    else
    {
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}

