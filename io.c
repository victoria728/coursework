//
// Created by av2-anyanwu on 30/04/2026.
//

#include <stdio.h>
#include <stdlib.h>
#include <io.h>



WaveformSample* loadCSV(const char *filename, int *count) { FILE *file = fopen(filename, "r"); if (file == NULL) return NULL;

    int maxRows = 1000;
    WaveformSample *data = malloc(maxRows * sizeof(WaveformSample));

    char line[256];
    fgets(line, sizeof(line), file); // skip header

    while (*count < maxRows && fscanf(file, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf",
                                      &data[*count].timestamp,
                                      &data[*count].phaseA,
                                      &data[*count].phaseB,
                                      &data[*count].phaseC,
                                      &data[*count].current,
                                      &data[*count].frequency,
                                      &data[*count].powerFactor,
                                      &data[*count].thd) == 8) {

        (*count)++;
    }

    fclose(file);
    return data;


}
