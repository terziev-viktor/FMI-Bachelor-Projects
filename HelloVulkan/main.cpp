#include <iostream>

#include "HelloVulkan.h"
#include <assert.h>

int main()
{
    if (HelloVulkan app; app)
    {
        app.run();
    }
    else
    {
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}

