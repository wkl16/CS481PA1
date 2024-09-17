#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <stdio.h>
#include <signal.h>
#include <stdbool.h>

pid_t pid;
bool running = true;


void handle_sigint(int sig) {
    if (sig == SIGINT) {
        printf("\n");
        printf("ctrl-c caught\n");
        printf("terminating the child process\n");
        kill(pid, SIGKILL);
        wait(0);
        printf("child process terminated\n");
        printf("terminating the parent process\n");
        exit(EXIT_SUCCESS);
    } else if (sig == SIGTSTP) {
        printf("\n");
        printf("ctrl-z caught\n");
        if (running) {
            printf("the child process is currently running => stopping the child process.\n");
            kill(pid, SIGTSTP);
            printf("the child process is now suspended\n");
        } else {
            printf("the child process is currently stopped => resuming the child process.\n");
            kill(pid, SIGCONT);
            printf("the child process is now running\n");
        }

        running = !running;
    }
}

int main(void) {
    pid = fork();
    if (pid > 0) {
        // Parent
        // Register SIGINT handler
        struct sigaction action;
        action.sa_handler = handle_sigint;
        sigemptyset(&action.sa_mask);
        action.sa_flags = 0;
        sigaction(SIGINT, &action, 0);
        sigaction(SIGTSTP, &action, 0);

        while (true) ; // Infinite loop
    } else if (pid == 0) {
        char *args[] = {"/bin/yes", NULL};
        char *envs[] = {NULL};
        // Child
        execve("/bin/yes", args, envs);
    } else {
        // Error
    }
}
