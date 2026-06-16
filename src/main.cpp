#include <Arduino.h>
#include "main.h"
#include "sensor_manager.h"
#include "measurement_processor.h"
#include "data_logger.h"

// Global instances
SensorManager sensorManager;
MeasurementProcessor measurementProcessor;
DataLogger dataLogger;

// Application variables
AppState currentState = INITIALIZING;
unsigned long lastUpdateTime = 0;

void setup() {
    // Initialize serial communication
    Serial.begin(SERIAL_BAUD);
    delay(2000);
    
    // Initialize application
    initializeApplication();
}

void loop() {
    unsigned long currentTime = millis();
    
    // Handle detection and measurement
    if (currentTime - lastUpdateTime >= UPDATE_INTERVAL) {
        lastUpdateTime = currentTime;
        handleDetection();
        displayResults();
    }
    
    // Handle serial commands
    if (Serial.available()) {
        char command = Serial.read();
        
        switch (command) {
            case 'c':
            case 'C':
                calibrateSensor();
                break;
            case 'r':
            case 'R':
                dataLogger.resetLog();
                Serial.println("Data log reset");
                break;
            case 'p':
            case 'P':
                dataLogger.printLog();
                break;
            default:
                break;
        }
    }
}

void initializeApplication() {
    Serial.println("\n=== Metal Detector System ===\n");
    Serial.println("Initializing...\n");
    
    // Initialize sensor manager
    sensorManager.begin();
    
    // Initialize data logger
    dataLogger.begin();
    
    // Calibrate sensor
    calibrateSensor();
    
    currentState = DETECTING;
    Serial.println("System ready!\n");
}

void handleDetection() {
    // Read sensor data
    float sensorValue = sensorManager.readSensor();
    
    // Check if metal is detected
    if (sensorManager.isMetalDetected()) {
        currentState = MEASURING;
        
        // Process measurements
        MeasurementData measurement = measurementProcessor.processMeasurement(sensorValue);
        
        // Log the measurement
        dataLogger.logMeasurement(measurement);
        
        // Return to detecting state
        currentState = DETECTING;
    }
}

void displayResults() {
    if (currentState == MEASURING || dataLogger.hasNewData()) {
        Serial.print("Sensor Value: ");
        Serial.print(sensorManager.readSensor());
        Serial.print(" | Width: ");
        Serial.print(measurementProcessor.getLastWidth());
        Serial.print("mm | Length: ");
        Serial.print(measurementProcessor.getLastLength());
        Serial.println("mm");
    }
}

void calibrateSensor() {
    Serial.println("Calibrating sensor...");
    sensorManager.calibrate();
    Serial.println("Calibration complete!\n");
}
