#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <stdio.h>
#include <signal.h>
#include <stdbool.h>

pid_t pid;
bool running = true;


void handle_sigint() {
    printf("\n");
    printf("ctrl-c caught\n");
    printf("terminating the child process\n");
    kill(pid, SIGKILL);
    wait(0);
    printf("child process terminated\n");
    printf("terminating the parent process\n");
    exit(EXIT_SUCCESS);
}

void handle_sigtstp() {
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

int main(void) {
    pid = fork();
    if (pid > 0) {
        // Parent
        // Register SIGINT handler
        struct sigaction action_int;
        action_int.sa_handler = handle_sigint;
        sigemptyset(&action_int.sa_mask);
        action_int.sa_flags = 0;
        sigaction(SIGINT, &action_int, 0);

        struct sigaction action_tstp;
        action_tstp.sa_handler = handle_sigtstp;
        sigemptyset(&action_tstp.sa_mask);
        action_tstp.sa_flags = 0;
        sigaction(SIGTSTP, &action_tstp, 0);

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
