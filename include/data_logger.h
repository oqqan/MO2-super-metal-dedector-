#ifndef DATA_LOGGER_H
#define DATA_LOGGER_H

#include <Arduino.h>
#include "measurement_processor.h"

class DataLogger {
private:
    static const int MAX_ENTRIES = 100;
    MeasurementData logBuffer[MAX_ENTRIES];
    int currentIndex;
    bool hasNewData;
    
public:
    DataLogger();
    
    // Initialize data logger
    void begin();
    
    // Log a measurement
    void logMeasurement(MeasurementData measurement);
    
    // Print all logged measurements
    void printLog();
    
    // Reset log buffer
    void resetLog();
    
    // Get number of logged entries
    int getEntryCount();
    
    // Check if new data has been logged
    bool hasNewData;
    
    // Get specific measurement
    MeasurementData getMeasurement(int index);
    
    // Export log to SD card (if available)
    void exportLog();
};

#endif // DATA_LOGGER_H
