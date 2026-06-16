#ifndef MEASUREMENT_PROCESSOR_H
#define MEASUREMENT_PROCESSOR_H

#include <Arduino.h>

struct MeasurementData {
    float width;
    float length;
    float sensorValue;
    unsigned long timestamp;
};

class MeasurementProcessor {
private:
    float lastWidth;
    float lastLength;
    float detectionStartTime;
    float detectionEndTime;
    static const float PIXELS_PER_MM = 0.1; // Conversion factor
    
public:
    MeasurementProcessor();
    
    // Process raw sensor data into measurements
    MeasurementData processMeasurement(float sensorValue);
    
    // Calculate width based on sensor readings
    float calculateWidth(float sensorValue);
    
    // Calculate length based on detection duration
    float calculateLength();
    
    // Get last measured width
    float getLastWidth();
    
    // Get last measured length
    float getLastLength();
    
    // Reset measurements
    void resetMeasurements();
};

#endif // MEASUREMENT_PROCESSOR_H
