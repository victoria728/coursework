#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include "waveform.h"


int main() {

    int count = 0;  //stores how many rows were loaded

    //log the file and return pointer to rows
    WaveformSample *samples= int loadCSV("power_quality_log.csv",&count);


    if (samples == NULL) {

        printf("Error loading file\n");

        return 1;

    }

    printf("Loaded %d samples\n",count);



    free(samples); //release memory after use

    return 0;

}