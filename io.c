//
// Created by av2-anyanwu on 30/04/2026.
//

/*
 io.c
 This file handles file input and output.
 It contains:
 - CSV loading
 - report writing
*/
#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
 countDataRows()
 Counts how many data rows are in the CSV file.
 Why?
 The coursework requires malloc sized to the number of rows.
 So first we count rows, then allocate exact memory.
*/
static size_t countDataRows(FILE *file) {
    char line[512];
    size_t count = 0;
    // Skip header line
    fgets(line, sizeof(line), file);
    // Count each remaining non-empty line
    while (fgets(line, sizeof(line), file)) {
        if (strlen(line) > 1) {
            count++;
        }
    }
    // Return file pointer to start
    rewind(file);
    return count;
}
/*
 loadCSV()
 Opens the CSV file, counts rows, allocates memory,
 reads each row, and stores it in WaveformSample array.
*/
int loadCSV(const char *filename,
            WaveformSample **samplesOut,
            size_t *countOut) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return 0;
    }
    size_t rowCount = countDataRows(file);
    if (rowCount == 0) {
        fclose(file);
        return 0;
    }
    WaveformSample *samples =
            malloc(rowCount * sizeof(WaveformSample));
    if (samples == NULL) {
        fclose(file);
        return 0;
    }
    char line[512];
    // Skip header row
    fgets(line, sizeof(line), file);
    size_t index = 0;
    while (fgets(line, sizeof(line), file) && index < rowCount) {
        WaveformSample sample;
        int valuesRead = sscanf(line,
                                "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf",
                                &sample.timestamp,
                                &sample.phaseA,
                                &sample.phaseB,
                                &sample.phaseC,
                                &sample.current,
                                &sample.frequency,
                                &sample.powerFactor,
                                &sample.thd);
        if (valuesRead == 8) {
            samples[index] = sample;
            index++;
        }
    }
    fclose(file);
    *samplesOut = samples;
    *countOut = index;
    return 1;
}
/*
 writePhaseReport()
 Helper function that writes one phase section.
*/
static void writePhaseReport(FILE *file,
                             const char *phaseName,
                             PhaseMetrics metrics) {
    fprintf(file, "%s\n", phaseName);
    fprintf(file, "-----------------------------\n");
    fprintf(file, "RMS Voltage: %.2f V\n", metrics.rms);
    fprintf(file, "Peak-to-Peak Voltage: %.2f V\n", metrics.peakToPeak);
    fprintf(file, "DC Offset: %.2f V\n", metrics.dcOffset);
    fprintf(file, "Minimum Voltage: %.2f V\n", metrics.minimum);
    fprintf(file, "Maximum Voltage: %.2f V\n", metrics.maximum);
    fprintf(file, "Variance: %.2f\n", metrics.variance);
    fprintf(file, "Standard Deviation: %.2f\n", metrics.standardDeviation);
    fprintf(file, "Clipped Samples: %d\n", metrics.clippedSamples);
    fprintf(file, "Tolerance Check: %s\n",
            metrics.withinTolerance ? "PASS" : "FAIL");
    fprintf(file, "Status Flags: 0x%02X\n", metrics.statusFlags);
    if (metrics.statusFlags == 0) {
        fprintf(file, "Status Meaning: Healthy\n");
    } else {
        fprintf(file, "Status Meaning:\n");
        if (hasStatusFlag(metrics.statusFlags, STATUS_CLIPPING)) {
            fprintf(file, "- Clipping detected\n");
        }
        if (hasStatusFlag(metrics.statusFlags, STATUS_OUT_OF_TOLERANCE)) {
            fprintf(file, "- RMS voltage out of tolerance\n");
        }
        if (hasStatusFlag(metrics.statusFlags, STATUS_DC_OFFSET)) {
            fprintf(file, "- DC offset issue detected\n");
        }
    }
    fprintf(file, "\n");
}
/*
 writeReport()
 Writes the final results.txt file.
*/
int writeReport(const char *filename,
                const char *inputFilename,
                const WaveformSample *samples,
                size_t count,
                const PhaseMetrics metrics[3],
                const WaveformSample *sortedSamples,
                size_t sortedCount) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        return 0;
    }
    fprintf(file, "========================================\n");
    fprintf(file, "POWER QUALITY ANALYSIS REPORT\n");
    fprintf(file, "========================================\n\n");
    fprintf(file, "Input file: %s\n", inputFilename);
    fprintf(file, "Samples loaded: %zu\n\n", count);
    writePhaseReport(file, "PHASE A", metrics[0]);
    writePhaseReport(file, "PHASE B", metrics[1]);
    writePhaseReport(file, "PHASE C", metrics[2]);
    int systemHealthy =
            metrics[0].statusFlags == 0 &&
            metrics[1].statusFlags == 0 &&
            metrics[2].statusFlags == 0;
    fprintf(file, "SYSTEM STATUS\n");
    fprintf(file, "-----------------------------\n");
    fprintf(file, "%s\n\n", systemHealthy ? "HEALTHY" : "WARNING / FAULT");
    fprintf(file, "TOP VOLTAGE MAGNITUDES FOR PHASE A\n");
    fprintf(file, "-----------------------------\n");
    size_t limit = sortedCount < 5 ? sortedCount : 5;
    for (size_t i = 0; i < limit; i++) {
        fprintf(file,
                "%zu) Time: %.6f s, Phase A: %.2f V\n",
                i + 1,
                sortedSamples[i].timestamp,
                sortedSamples[i].phaseA);
    }
    fclose(file);
    return 1;
}