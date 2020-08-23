// gcc version 10.1.0
// gcc -std=c++2a -fconcepts -O3 forksandpipes.cpp

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

auto main() -> int
{
    int the_pipe[2];
    const int MSGSIZE = 16;
    const char* msg1 = "hello, world #1"; 
    const char* msg2 = "hello, world #2"; 
    const char* msg3 = "hello, world #3";

    // Create a pipe to use for communication
    if(pipe(the_pipe) < 0)
    {
        puts("Could not create pipe ;(");
        return 2;
    }

    auto [pipe_read, pipe_write] = the_pipe;

    if(pid_t pid = fork(); pid == 0) // we are the child process
    {
        printf("Im the child process man! My id is %d \n", pid);
        write(pipe_write, msg1, MSGSIZE);
        write(pipe_write, msg2, MSGSIZE);
        write(pipe_write, msg3, MSGSIZE);
    }
    else // we are the parent process
    {
        puts("Im the parent process man!");
        int nbytes = 0;
        char inbuffer[MSGSIZE];

        while(nbytes = read(pipe_read, inbuffer, MSGSIZE))
        {
            printf("%s \n", inbuffer);
        }
        if(nbytes != 0)
        {
            return 2;
        }
        puts("Finised reading!");
    }

    return 0;
}   
