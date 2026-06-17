#ifndef METAL_DETECTOR_H
#define METAL_DETECTOR_H

#include <Arduino.h>

class MetalDetector {
private:
    int sensorPin;
    float calibrationValue;
    float detectionThreshold;
    
    // Dynamic data collection
    float* signalBuffer;        // Dynamic array
    int bufferCapacity;         // Max size
    int bufferSize;             // Current size
    bool isCollecting;          // Collection state
    
    void expandBuffer();        // Helper to resize
    
public:
    MetalDetector(int pin = A0);
    ~MetalDetector();           // Destructor for cleanup
    
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
    
    // Data collection methods
    void processReading(float value);   // Main processing
    void getCollectedData(float*& data, int& size); // Return collected data
    void clearBuffer();                 // Reset buffer
    
    // Get collection status
    bool isCurrentlyCollecting();
};

#endif // METAL_DETECTOR_H