#ifndef METAL_DETECTOR_H
#define METAL_DETECTOR_H

#include <Arduino.h>

class MetalDetector {
private:
    int sensorPin;
    float calibrationValue;
    float detectionThreshold;
    
public:
    MetalDetector(int pin = A0);
    
    // Initialize detector
    void begin();
    
    // Read sensor value
    float readSensor();
    
    // Calibrate the detector
    void calibrate();
    
    // Check if metal is detected
    bool isMetalDetected();
    
    // Get width measurement
    float getWidth();
    
    // Get length measurement
    float getLength();
    
    // Set detection threshold
    void setThreshold(float value);
};

#endif // METAL_DETECTOR_H
