#include <stdio.h>
#include <stdbool.h>

#define SUCCESS 0
#define ERROR 1

void erat(int upper_limit) {
    for (int i = 2; i <= upper_limit; i++) {
        bool is_prime = true;
        for (int j = 2; j < i && is_prime; j++) {
            if (i % j == 0) {
                is_prime = false;
            }
        }
        if (is_prime) {
            printf("%d\n", i);
        }
    }
}

int main(int argc, char **argv) {
    int upper_limit = 0;
    sscanf(argv[1], "%d", &upper_limit);
    if (argc > 1 && upper_limit > 0) {
        erat(upper_limit);
        return SUCCESS;
    } else {
        return ERROR;
    }
}