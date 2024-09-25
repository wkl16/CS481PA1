#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

int fib_seq(int x);

int fib_fork(int n, int m) {
    // Base Cases
    if (n == 0) return 0;
    if (n == 1) return 1;

    // Use fib_seq if enough computing resources
    if ((n-1) <= m && (n-2) <= m) {
        return fib_seq(n);
    }


    int pipe1[2];
    if (pipe(pipe1) != 0) {
        fprintf(stderr, "Failed to create pipe\n");
        exit(EXIT_FAILURE);
    }
    pid_t pid1 = fork();
    if (pid1 > 0) {
        // Parent
        close(pipe1[1]); // Close the write end of pipe
        int pipe2[2];
        if (pipe(pipe2) != 0) {
            fprintf(stderr, "Failed to create pipe\n");
            exit(EXIT_FAILURE);
        }
        pid_t pid2 = fork();
        if (pid2 > 0) {
            // Parent
            close(pipe2[1]); // Close the write end of pipe
            int r1, r2;
            read(pipe1[0], &r1, sizeof(int)); 
            read(pipe2[0], &r2, sizeof(int)); 
            close(pipe1[0]); // Close the read end of pipe
            close(pipe2[0]); // Close the read end of pipe
            
            int stat1, stat2;
            waitpid(pid1, &stat1, 0);
            waitpid(pid2, &stat2, 0);

            if (WIFEXITED(stat1)) {
                if (WEXITSTATUS(stat1) != EXIT_SUCCESS) {
                    exit(EXIT_FAILURE);
                }
            }

            if (WIFEXITED(stat2)) {
                if (WEXITSTATUS(stat2) != EXIT_SUCCESS) {
                    exit(EXIT_FAILURE);
                }
            }

            return r1 + r2;
        } else if (pid2 == 0) {
            // Child
            close(pipe2[0]); // Close the read end of pipe
            int rec2 = fib_fork(n-2, m);
            write(pipe2[1], &rec2, sizeof(int));
            close(pipe2[1]); // Close the write end of pipe
            exit(EXIT_SUCCESS);
        } else {
            // Error
            fprintf(stderr, "Failed to fork (inner).\n");
            kill(pid1, SIGKILL);
            waitpid(pid1, NULL, 0);
            exit(EXIT_FAILURE);
        }
    } else if (pid1 == 0) {
        // Child
        close(pipe1[0]); // Close the read end of pipe
        int rec1 = fib_fork(n-1, m);
        write(pipe1[1], &rec1, sizeof(int));
        close(pipe1[1]); // Close the write end of pipe
        exit(EXIT_SUCCESS);
    } else {
        // Error
        fprintf(stderr, "Failed to fork (outer).\n");
        exit(EXIT_FAILURE);
    }
}

/* Slow/Recursive impl of Fib */
int fib_seq(int x) {
    int rint = (rand() % 30);
    double dummy;

    for (int i = 0; i < rint*100; i++) {
        dummy = 2.345 * i * 8.765/1.234;
    }

    if (x == 0) return 0;
    if (x == 1) return 1;

    return fib_seq(x-1) + fib_seq(x-2);
}


int main(int argc, char ** argv) {
    int n = -1;
    int m = -1;
    int c;

    opterr = 0;

    while ((c = getopt(argc, argv, "F:S:")) != -1) {
        switch (c) {
            case 'F':
                n = atoi(optarg);
                if (n <= 0) {
                    fprintf(stderr, "Invalid argument for option -F: %s.\n", optarg);
                    exit(EXIT_FAILURE);
                }
                break;
            case 'S':
                m = atoi(optarg);
                if (m < 0) {
                    fprintf(stderr, "Invalid argument for option -S: %s.\n", optarg);
                    exit(EXIT_FAILURE);
                }
                break;
            case '?':
                if (optopt == 'F' || optopt == 'S') {
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                } else {
                    fprintf(stderr, "Unknown option -%c.\n", optopt);
                }

                exit(EXIT_FAILURE);

            default:
                exit(EXIT_FAILURE);

        }
    }

    for (int i = optind; i < argc; i++) {
        printf("Non-option argument %s.\n", argv[i]);
    }

    if (n < 0) {
        fprintf(stderr, "-F is a required.\n");
        exit(EXIT_FAILURE);
    }
    if (m < 0) {
        fprintf(stderr, "-S is a required.\n");
        exit(EXIT_FAILURE);
    }

    int result = fib_fork(n, m);
    printf("%d\n", result);
}
