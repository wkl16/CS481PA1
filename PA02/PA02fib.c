#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int fib_seq(int x) {
  int i, rint = (rand() % 30);
  double dummy;
  for (i = 0; i < rint * 100; i++) {
    dummy = 2.345 * i * 8.765 / 1.234;
  }
  if (x == 0)
    return (0);
  else if (x == 1)
    return (1);
  else
    return (fib_seq(x - 1) + fib_seq(x - 2));
}

int fib_fork(int n, int m) {
  if ((n - 1) <= m && (n - 2) <= m) {
    return fib_seq(n);
  } else {
    int pipefd1[2], pipefd2[2];
    pid_t pid1, pid2;

    // create pipes for IPC
    pipe(pipefd1);
    pipe(pipefd2);

    // Fork child process for fib(n-1)
    pid1 = fork();
    if (pid1 == 0) {
      close(pipefd1[0]);  // close read end
      int result = fib_fork(n - 1, m);
      write(pipefd1[1], &result, sizeof(result));
      close(pipefd1[1]);  // close write end
      exit(0);
    }
    // Fork child process for fib(n-2)
    pid2 = fork();
    if (pid2 == 0) {
      close(pipefd2[0]);  // close read end
      int result = fib_fork(n - 2, m);
      write(pipefd2[1], &result, sizeof(result));
      close(pipefd2[1]);  // close write end
      exit(0);
    }

    // Parent process
    close(pipefd1[1]);  // close write end
    close(pipefd2[1]);  // close write end
    int fib_n1, fib_n2;

    // read results from child processes
    read(pipefd1[0], &fib_n1, sizeof(fib_n1));
    read(pipefd2[0], &fib_n2, sizeof(fib_n2));

    // close pipes
    close(pipefd1[0]);
    close(pipefd2[0]);

    // wait for both children to terminate
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    return fib_n1 + fib_n2;
  }
}

int main(int argc, char **argv) {
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
        if (m <= 0) {
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

  if (n <= 0) {
    fprintf(stderr, "-F is a required.\n");
    exit(EXIT_FAILURE);
  }
  if (m <= 0) {
    fprintf(stderr, "-S is a required.\n");
    exit(EXIT_FAILURE);
  }

  // printf("n = %d, m = %d\n", n, m);
  int result = fib_fork(n, m);
  printf("%d\n", result);

  return 0;
}
