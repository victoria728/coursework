//
// Created by av2-anyanwu on 30/04/2026.
//
#include <stddef.h>
#include <stdint.h>

#ifndef UNTITLED_WAVEFORM_H
#define UNTITLED_WAVEFORM_H

#define NOMINAL_RMS_VOLTAGE 230.0
#define RMS_TOLERANCE_PERCENT 10.0
#define CLIPPING_LIMIT 324.9 // If the average voltage is more than this, flag the DC offset issue
#define DC_OFFSET_LIMIT 5.0 // Bitwise status flags
 #define STATUS_CLIPPING 0x01 // bit 0
  #define STATUS_OUT_OF_TOLERANCE 0x02 // bit 1
  #define STATUS_DC_OFFSET 0x04 // bit 2

typedef struct {

    double timestamp;
    double phaseA;
    double phaseB;
    double phaseC;
    double current;
    double frequency;
    double powerFactor;
    double thd;

} WaveformSample;

typedef struct {
    double rms;
    double peakToPeak;
    double dcOffset;
    double minimum;
    double maximum;
    double variance;
    double standardDeviation;
    int clippedSamples;
    int withinTolerance;
    uint8_t statusFlags;
} PhaseMetrics;

typedef enum {
    PHASE_A,
    PHASE_B,
    PHASE_C
} PhaseSelector;

PhaseMetrics analysePhase(const WaveformSample *samples,
                          size_t count,
                          PhaseSelector phase);

void sortSamplesByVoltageMagnitude(WaveformSample *samples,
                                   size_t count,
                                   PhaseSelector phase);

int hasStatusFlag(uint8_t flags, uint8_t flag);
#endif //UNTITLED_WAVEFORM_H
