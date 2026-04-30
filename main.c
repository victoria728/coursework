#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include "waveform.h"


int main() {

    int count = 0;  //stores how many rows were loaded

    //losd the file snd return pointer to rows
    WaveformSample *data = loadCSV("power_quality_log.csv", &count);


    if (data == NULL) {

        printf("Error loading file\n");

        return 1;

    }

    printf("Loaded %d rows\n", count);



    free(data);

    return 0;

} 