// gcc version 10.1.0
// gcc -std=c++2a -fconcepts -O3 forksandpipes.cpp
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

auto main() -> int
{
    int the_pipe[2];
    const int MSGSIZE = 16;
    const char* msg1 = "hello, world #1"; 
    const char* msg2 = "hello, world #2"; 
    const char* msg3 = "hello, world #3";
    char buffer[MSGSIZE];

    // Create a pipe to use for communication
    if(pipe(the_pipe) < 0)
    {
        puts("Could not create pipe ;(");
        return 2;
    }

    auto [pipe_read, pipe_write] = the_pipe;

    if(pid_t pid = fork(); pid == 0) // we are the child process
    {
        printf("Im the child process man! Writing some shit to the pipe!\n");
        write(pipe_write, msg1, MSGSIZE);
        sleep(1); // doing some hard, hard work
        write(pipe_write, msg2, MSGSIZE);
        sleep(1); // doint some hard, hard work
        write(pipe_write, msg3, MSGSIZE);

        exit(0);
    }
    else // we are the parent process
    {
        //int status = -1;
        //pid_t dead_pid = wait(&status); // wait for a process to die
        printf("Child id is %d \n", pid);
        
        int nbytes = 0;
        int messages_to_read = 3; // we know we expect 3 messages in the pipe
        while(messages_to_read-- && (nbytes = read(pipe_read, buffer, MSGSIZE)) > 0)
        {
            printf("%s \n", buffer);
        }
        
        puts("Finised reading!");

        exit(0);
    }
}   
