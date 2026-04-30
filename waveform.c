//
// Created by av2-anyanwu on 30/04/2026.
//
#include <stdio.h>
#include "waveform.h"
#include <math.h>

getPhaseVoltage();
 double getPhaseVoltage(const WaveformSample *sample,
                        PhaseSelector phase) {
    if (phase == PHASE_A) { return sample->phaseA;
    }
    if (phase == PHASE_B) { return sample->phaseB;
    }
    return sample->phaseC;
}
getPhaseName()
 const char* getPhaseName(PhaseSelector phase) {
    if (phase == PHASE_A) {
        return "Phase A";
    }
    if (phase == PHASE_B) {
        return "Phase B";
    }
    return "Phase C";
}
int hasStatusFlag(uint8_t flags, uint8_t flag) {
    return (flags & flag) != 0;
}
PhaseMetrics analysePhase(const WaveformSample *samples,
                          size_t count,
                          PhaseSelector phase) {

}PhaseMetrics metrics;
metrics.rms = 0.0;
metrics.peakToPeak = 0.0;
metrics.dcOffset = 0.0;
metrics.minimum = 0.0;
metrics.maximum = 0.0;
metrics.variance = 0.0;
metrics.standardDeviation = 0.0;
metrics.clippedSamples = 0;
metrics.withinTolerance = 0;
metrics.statusFlags = 0;

if (samples == NULL || count == 0) {
    return metrics;
} double sum = 0.0;
double sumSquares = 0.0;

 double firstVoltage = getPhaseVoltage(samples, phase);
 double minVoltage = firstVoltage;
 double maxVoltage = firstVoltage;

  for (size_t i = 0; i < count; i++) {
      const WaveformSample *currentSample = samples + i;

      double voltage = getPhaseVoltage(currentSample, phase);
      sum += voltage;
      sumSquares += voltage * voltage;

      if (voltage < minVoltage) {
          minVoltage = voltage;
      }

      if (voltage > maxVoltage) {
          maxVoltage = voltage;
      }

      if (fabs(voltage) >= CLIPPING_LIMIT) {
          metrics.clippedSamples++;
      }
  }

  metrics.dcOffset = sum / count;
  metrics.rms = sqrt(sumSquares / count);
  metrics.minimum = minVoltage;
  metrics.maximum = maxVoltage;
  metrics.peakToPeak = maxVoltage - minVoltage;

   Second pass: variance needs the mean first.

   double varianceSum = 0.0;
   for (size_t i = 0; i < count; i++) {
       const WaveformSample *currentSample = samples + i;
       double voltage = getPhaseVoltage(currentSample, phase);
       double difference = voltage - metrics.dcOffset;
       varianceSum += difference * difference;
   }

   metrics.variance = varianceSum / count;
   metrics.standardDeviation = sqrt(metrics.variance);

    double lowerLimit = NOMINAL_RMS_VOLTAGE * 0.90;
    double upperLimit = NOMINAL_RMS_VOLTAGE * 1.10;

    if (metrics.rms >= lowerLimit && metrics.rms <= upperLimit) {
        metrics.withinTolerance = 1;
    } else {
        metrics.withinTolerance = 0;
        metrics.statusFlags |= STATUS_OUT_OF_TOLERANCE;
    }

    if (metrics.clippedSamples > 0) {
        metrics.statusFlags |= STATUS_CLIPPING;
    }
    if (fabs(metrics.dcOffset) > DC_OFFSET_LIMIT) {
metrics.statusFlags |= STATUS_DC_OFFSET;
}
 sortSamplesByVoltageMagnitude()


  void sortSamplesByVoltageMagnitude(WaveformSample *samples,
                                     size_t count,
                                     PhaseSelector phase) {

        if (samples == NULL || count == 0) {
            return;
        } for (size_t i = 1; i < count; i++) {

            WaveformSample key = samples[i];
            double keyMagnitude = fabs(getPhaseVoltage(&key, phase));
            size_t j = i;
            while (j > 0) {
                double previousMagnitude =
                        fabs(getPhaseVoltage(&samples[j - 1], phase));
                if (previousMagnitude >= keyMagnitude) {
                    break;
                }

                samples[j] = samples[j - 1];
                j--;
            }
            samples[j] = key;
        }
    }