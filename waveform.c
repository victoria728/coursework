//
// Created by av2-anyanwu on 30/04/2026.
//
#include <stdio.h>
#include "waveform.h"
#include <math.h>

typedef struct {
    double phaseA;
} WaveformSample;

double calculate_RMS(WaveformSample *samples, int count){
    double sumSquares = 0.0;

    for (int i = 0; i < count; i++){
        double voltage = samples[i].phaseA;
        sumSquares += voltage * voltage;
    }

    return sqrt(sumSquares/count);
}
