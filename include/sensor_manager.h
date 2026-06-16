#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <Arduino.h>

class SensorManager {
private:
    static const int SENSOR_PIN = A0;
    static const int NUM_SAMPLES = 10;
    float calibrationValue;
    float threshold;
    
public:
    SensorManager();
    
    // Initialize sensor
    void begin();
    
    // Read raw sensor value
    float readSensor();
    
    // Read averaged sensor value
    float readSensorAveraged();
    
    // Calibrate sensor
    void calibrate();
    
    // Check if metal is detected
    bool isMetalDetected();
    
    // Get calibration value
    float getCalibrationValue();
    
    // Set threshold
    void setThreshold(float value);
    
    // Get threshold
    float getThreshold();
};

#endif // SENSOR_MANAGER_H
