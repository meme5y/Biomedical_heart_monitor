// pulse_detection.h
// Header file for pulse detection algorithms

#ifndef PULSE_DETECTION_H
#define PULSE_DETECTION_H

#include <Arduino.h>

// Function declarations
void setupPulseDetection();
int calculateBPM(int pulses, unsigned long interval);
bool isValidPulse(unsigned long lastPulseTime);

#endif
