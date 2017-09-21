#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>
#include <limits.h>

#define SUCCESS 0
#define ERROR 1
#define VERSION 1.0

void print_info() {
    printf("%s\n%s\n%s\n%s\n",
           "Usage:",
           "erat -h",
           "erat -V",
           "erat [options] N");
    printf("%s\n%s\n%s\n%s\n",
           "Options:",
           "-h, --help Prints usage information",
           "-V, --version Prints version information",
           "-o, --output Path to output file");
    printf("%s\n%s\n",
           "Examples:",
           "erat -o - 10");
}

void print_version() {
    printf("%s%.2f\n", "Version: ", VERSION);
}

void erat(long upper_limit, FILE *file) {
    int i, j;
    for (i = 2; i <= upper_limit; i++) {
        bool is_prime = true;
        for (j = 2; j < i && is_prime; j++) {
            if (i % j == 0) {
                is_prime = false;
            }
        }
        if (is_prime) {
            fprintf(file, "%d\n", i);
        }
    }
}

int main(int argc, char **argv) {
    int argument;
    char *input = NULL;
    FILE *output = NULL;
    char *lastChar;
    long number;
    size_t size;

    static struct option options[] =
        {
            {"help",    no_argument,       NULL, 'h'},
            {"version", no_argument,       NULL, 'V'},
            {"output",  required_argument, NULL, 'o'},
            {NULL,      no_argument,       NULL, 0}
        };

    argument = getopt_long(argc, argv, "hVo:", options, NULL);

    switch (argument) {
        case 'h':
            print_info();
            return SUCCESS;
        case 'V':
            print_version();
            return SUCCESS;
        case 'o':
            if (strcmp(optarg, "-") != 0) {
                output = fopen(optarg, "wb");
                if (!output) {
                    fprintf(stderr,
                            "The file '%s' could not be created/opened",
                            optarg);
                    return ERROR;
                }
            }
            if (argc < 4) {
                fprintf(stderr,
                        "The number is missing");
                return ERROR;
            }
        default:
            if (argc < 4) {
                if (getline(&input, &size, stdin) == -1) { input = NULL; }
            }
            number = strtol(input ? input : argv[3], &lastChar, 10);
            if (errno == ERANGE && (number == LONG_MIN || number == LONG_MAX)) {
                fprintf(stderr,
                        "The number is out of range (%d to %li)",
                        2, LONG_MAX);
                return ERROR;
            }
            if ((input ? input : argv[3]) == lastChar) {
                fprintf(stderr,
                        "The number is invalid");
                return ERROR;
            }
            if (number < 2) {
                fprintf(stderr,
                        "The number is out of range (%d to %li)",
                        2, LONG_MAX);
                return ERROR;
            }
            erat(number, output ? output : stdout);
            if (output) { fclose(output); }
            if (input) { free(input); }
            return SUCCESS;
    }
}
