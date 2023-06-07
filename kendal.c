/*Takes as input a file with a random ranking. Assumes that the first ranking is in ascending order. Calculates the kendal*/
#include<stdio.h>
#include<stdlib.h>
#include"functions.h"

typedef struct data_arr {
    unsigned int* data;
    unsigned int size;
} Data_arr;

//Makes a one dimensional array with values read from the file
int read_arrangements ( char* const filename, Data_arr* arrPtr ) {

    FILE* fd;
    unsigned int num, size;
    unsigned int* tempPtr = NULL;

    if ( (fd = fopen( filename, "r" )) == NULL ) {
        return -1;
    }

    for ( size = 0; (fscanf( fd, " %d", &num) != EOF); size++ ) {
        tempPtr = (unsigned int*) realloc( tempPtr, sizeof(unsigned int)*(size+1) );
        if ( tempPtr == NULL ) {
            return -2;
        }
        tempPtr[size] = num;
        arrPtr->data = tempPtr;
    }
    arrPtr->size = size;
    
    fclose(fd);
    remove(filename);
    return 1;
}

int find_discpairs ( const Data_arr arrPtr ) {

    unsigned int counter1, counter2, val, disc_pairs, size = arrPtr.size;  

    disc_pairs = 0;
    for ( counter1 = 0; counter1 < (size - 1); counter1++ ) {
        val = arrPtr.data[counter1];
        for ( counter2 = (counter1 + 1); counter2 < size; counter2++ ) {
            if ( val > arrPtr.data[counter2] ) {
                disc_pairs++;
            }
        }
    }
    return disc_pairs;
}

int kendal ( char* filename ) {

    Data_arr in_arrPtr = {0};
    int result, in_disc_pairs, numpairs;
    double kendal;
    
    result = read_arrangements( filename, &in_arrPtr );
    switch(result) {
        case -1:
            fprintf(stderr, "Failed to open file %s.\n", filename);
            return -1;
        case -2:
            fprintf(stderr, "Not enough memory.\n");
            return -2;
        default:
            ;
    }

    in_disc_pairs = find_discpairs( in_arrPtr ), numpairs = (in_arrPtr.size*(in_arrPtr.size - 1))/2;
    kendal =  1 - ((2* (double) in_disc_pairs)/numpairs);
    printf("Kendal coefficient is: %.3lf\n", kendal);
    
    free( in_arrPtr.data );
    return 0;
}
