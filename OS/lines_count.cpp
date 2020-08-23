// gcc version 10.1.0
// gcc -std=c++2a -fconcepts -O3 lines_count.cpp

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <algorithm>
using std::count;

auto main(int argc, char* argv[]) -> int
{
    if(argc != 2)
    {
        puts("Wrong usage");
        exit(1);
    }

    if(int file = open(argv[1], O_RDONLY); file >= 0)
    {
        const unsigned int buffersize = 128;
        char buffer[buffersize];
        int lines = 0;
        int nbytes = 0;
        while( (nbytes = read(file, buffer, buffersize)) > 0)
        {
            lines += count(buffer, buffer + nbytes, '\n');
        }
        printf("%d lines in %s\n", lines, argv[1]);

        close(file);
    }
    else
    {
        puts("Could not open that file...");
    }
    
    exit(0);
}
