#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>

// Application configuration
#define SERIAL_BAUD 115200
#define UPDATE_INTERVAL 500 // milliseconds

// Application states
enum AppState {
    INITIALIZING = 0,
    CALIBRATING = 1,
    DETECTING = 2,
    MEASURING = 3,
    ERROR = 4
};

// Function declarations
void initializeApplication();
void handleDetection();
void displayResults();
void calibrateSensor();

#endif // MAIN_H
