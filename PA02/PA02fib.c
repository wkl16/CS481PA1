#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

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

    printf("n = %d, m = %d\n", n, m);
}
