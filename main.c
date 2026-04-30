#include <stdio.h>

#include <stdlib.h>

#include "io.h"

#include "waveform.h"



int main() {



    WaveformSample *samples = NULL;

    size_t count = 0;



    int loaded = loadCSV("power_quality_log.csv", &samples, &count);



    if (!loaded) {

        printf("Error loading file\n");

        return 1;

    }



    printf("Loaded %zu samples\n", count);





    free(samples);



    return 0;

}