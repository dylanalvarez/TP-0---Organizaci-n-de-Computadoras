#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define SUCCESS 0
#define ERROR 1
#define VERSION 1.0
#define MAX_RANGE 1000

void print_info(){

    char* help = "-h, --help Prints usage information";
    char* version = "-V, --version Prints version information";
    char* output = "-o, --output Path to output file";

    printf("%s\n%s\n%s\n%s\n", "Usage:", "erat -h", "erat -V", "erat [options] N");
    printf("%s\n%s\n%s\n%s\n", "Options:", help, version, output);
    printf("%s\n%s\n", "Examples:", "erat -o - 10");
}

void erat(int upper_limit, FILE* file) {
    for (int i = 2; i <= upper_limit; i++) {
        bool is_prime = true;
        for (int j = 2; j < i && is_prime; j++) {
            if (i % j == 0) {
                is_prime = false;
            }
        }
        if (is_prime) {
            fprintf(file, "%d\n", i);
        }
    }
}

bool is_in_range(int number){
    return (number > 0 && number < MAX_RANGE);
}

int main(int argc, char **argv) {

   if (argc == 2){
        // -h o -V
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0){
            print_info();
        } else if (strcmp(argv[1], "-V") == 0 || strcmp(argv[1], "--version") == 0){
            printf("%s%.2f\n", "Version: ", VERSION);
        } else{
            print_info();
        }
    } else if (argc == 4){
        // 4 argumentos: erat -o -output N
        if (strcmp(argv[1], "-o") == 0 || strcmp(argv[1], "--output") == 0){
            // check range
            int number = atoi(argv[3]);

            if (!is_in_range(number)){
                fprintf(stderr, "%s\n", "Error: Fuera de rango");
                return ERROR;
            }

            if (strcmp(argv[2], "-") == 0){
                //imprimir en stdout
                erat(number, stdout);
            } else {
                //imprimir en file
                FILE* file = fopen(argv[2], "wb");
                if (!file){
                    fprintf(stderr, "%s\n", "Error al abrir el archivo");
                    return ERROR;
                } else {
                    erat(number, file);
                }
            }
        }
    } else {
        print_info();
    }

    return SUCCESS;
}