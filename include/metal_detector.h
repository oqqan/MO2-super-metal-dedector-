#ifndef METAL_DETECTOR_H
#define METAL_DETECTOR_H

#include <Arduino.h>

// Number of sensors
#define NUM_SENSORS 6

// Sensor configuration
struct SensorConfig {
    int pin;
    float position;      // Position on conveyor in mm
    bool isActive;
};

// Detection event structure
struct DetectionEvent {
    int sensorId;
    unsigned long timestamp;
    float sensorValue;
};

class MetalDetector {
private:
    SensorConfig sensors[NUM_SENSORS];
    float calibrationValues[NUM_SENSORS];
    float detectionThreshold;
    
    // Detection tracking
    DetectionEvent events[NUM_SENSORS];
    int eventCount;
    bool isDetecting;
    unsigned long detectionStartTime;
    
    // Results
    float metalLength;
    float conveyorSpeed;  // mm per millisecond
    
    void analyzeDetection();
    
public:
    MetalDetector();
    ~MetalDetector();
    
    // Setup and calibration
    void begin();
    void setSensorPin(int sensorId, int pin, float position);
    void calibrate();
    void setThreshold(float value);
    void setConveyorSpeed(float speed);
    
    // Read and process
    void readAllSensors();
    void processReading(int sensorId, float value);
    
    // Get results
    float getMetalLength();
    int getDetectedSensorCount();
    void printDetectionData();
    
    // Utility
    bool isCurrentlyDetecting();
    void resetDetection();
};

#endif // METAL_DETECTOR_H