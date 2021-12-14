#include<stdio.h>
#include<stdlib.h>

void save_to_file(const char* filename, double t_seq, double t_par) {
    const char DELIMITER = ';';

    FILE *fptr;
    fptr = fopen(filename, "a");

    if (fptr == NULL) {
        printf("Error!");
        exit(1);
    }

    fprintf(fptr, "%.15lf%c%.15lf\n", t_seq, DELIMITER, t_par);
}