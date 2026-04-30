//
// Created by av2-anyanwu on 30/04/2026.
//

#ifndef IO_H
#define IO_H
/*
 io.h
 This header file declares functions for:
 - loading CSV data
 - writing the final report
*/
#include <stddef.h>
#include "waveform.h"
// Loads CSV file into dynamic memory
int loadCSV(const char *filename,
            WaveformSample **samplesOut,
            size_t *countOut);
// Writes final analysis report
int writeReport(const char *filename,
                const char *inputFilename,
                const WaveformSample *samples,
                size_t count,
                const PhaseMetrics metrics[3],
                const WaveformSample *sortedSamples,
                size_t sortedCount);
#endif