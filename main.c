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
#define NUMBER_OF_DIGITS_IN_LONG_MAX 20

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

void print_out_of_range_error() {
    fprintf(stderr, "The number is out of range (%d to %li)\n", 2, LONG_MAX);
}

int main(int argc, char **argv) {
    int argument;
    char input[NUMBER_OF_DIGITS_IN_LONG_MAX];
    FILE *output = NULL;
    char *lastChar;
    long number;
    bool readFromStdin = false;

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
                            "The file '%s' could not be created/opened\n",
                            optarg);
                    return ERROR;
                }
            }
            if (argc < 4) {
                fprintf(stderr,
                        "The number is missing\n");
                return ERROR;
            }
        default:
            if (argc < 4) {
                if (!fgets(input, NUMBER_OF_DIGITS_IN_LONG_MAX, stdin)) {
                    print_out_of_range_error();
                    return ERROR;
                }
                readFromStdin = true;
            }
            number = strtol(readFromStdin ? input : argv[3], &lastChar, 10);
            if (errno == ERANGE && (number == LONG_MIN || number == LONG_MAX)) {
                print_out_of_range_error();
                return ERROR;
            }
            if ((readFromStdin ? input : argv[3]) == lastChar) {
                fprintf(stderr,
                        "The number is invalid\n");
                return ERROR;
            }
            if (number < 2) {
                print_out_of_range_error();
                return ERROR;
            }
            erat(number, output ? output : stdout);
            if (output) { fclose(output); }
            return SUCCESS;
    }
}
