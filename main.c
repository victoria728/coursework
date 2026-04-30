/*
 main.c
 This is the entry point of the program.
 It should only control the flow:
 - check command-line argument
 - load CSV data
 - analyse phases
 - write report
 - free memory
 It should NOT contain detailed maths logic.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"
#include "waveform.h"
int main(int argc, char *argv[]) {
    /*
    argc = number of command-line arguments
    argv = actual argument text
    Example:
    ./PowerQualityAnalyser power_quality_log.csv
    argc = 2
    argv[0] = program name
    argv[1] = CSV filename
    */
    if (argc != 2) {
        printf("Usage: %s <csv_file>\n", argv[0]);
        return 1;
    }
    const char *inputFilename = argv[1];
    WaveformSample *samples = NULL;
    size_t count = 0;
    printf("Loading file: %s\n", inputFilename);
    int loaded = loadCSV(inputFilename, &samples, &count);
    if (!loaded) {
        printf("Error: could not load CSV file.\n");
        return 1;
    }
    printf("Loaded %zu samples successfully.\n", count);
    /*
    Analyse each phase.
    metrics[0] = Phase A
    metrics[1] = Phase B
    metrics[2] = Phase C
    */
    PhaseMetrics metrics[3];
    metrics[0] = analysePhase(samples, count, PHASE_A);
    metrics[1] = analysePhase(samples, count, PHASE_B);
    metrics[2] = analysePhase(samples, count, PHASE_C);
    /*
    Extra feature:
    sorting samples by Phase A voltage magnitude.
    We copy samples first so the original data stays unchanged.
    */
    WaveformSample *sortedSamples =
            malloc(count * sizeof(WaveformSample));
    if (sortedSamples == NULL) {
        printf("Error: could not allocate memory for sorting.\n");
        free(samples);
        return 1;
    }
    memcpy(sortedSamples, samples, count * sizeof(WaveformSample));
    sortSamplesByVoltageMagnitude(sortedSamples, count, PHASE_A);
    /*
    Write final report.
    */
    int reportWritten = writeReport("results.txt",
                                    inputFilename,
                                    samples,
                                    count,
                                    metrics,
                                    sortedSamples,
                                    count);
    if (!reportWritten) {
        printf("Error: could not write results.txt\n");
        free(sortedSamples);
        free(samples);
        return 1;
    }
    printf("Analysis complete.\n");
    printf("Report written to results.txt\n");
    /*
    Free all dynamic memory.
    This prevents memory leaks.
    */
    free(sortedSamples);
    free(samples);
    return 0;
}
