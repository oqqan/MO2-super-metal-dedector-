#include <Arduino.h>
#include "metal_detector.h"

// Global metal detector instance
MetalDetector detector;

// Application settings
#define SERIAL_BAUD 115200
#define UPDATE_INTERVAL 500

// Variables
unsigned long lastUpdateTime = 0;
float currentWidth = 0;
float currentLength = 0;

void setup() {
    Serial.begin(SERIAL_BAUD);
    delay(2000);
    
    Serial.println("\n=== Metal Detector ===");
    Serial.println("\nInitializing sensor...");
    
    // Initialize detector
    detector.begin();
    detector.calibrate();
    
    Serial.println("System ready!\n");
}

void loop() {
    unsigned long currentTime = millis();
    
    // Update sensor readings
    if (currentTime - lastUpdateTime >= UPDATE_INTERVAL) {
        lastUpdateTime = currentTime;
        
        // Check if metal is detected
        if (detector.isMetalDetected()) {
            currentWidth = detector.getWidth();
            currentLength = detector.getLength();
            
            Serial.print("METAL DETECTED! ");
            Serial.print("Width: ");
            Serial.print(currentWidth);
            Serial.print("mm | Length: ");
            Serial.print(currentLength);
            Serial.println("mm");
        }
    }
    
    // Handle serial input
    if (Serial.available()) {
        char cmd = Serial.read();
        
        switch(cmd) {
            case 'c':
            case 'C':
                Serial.println("Calibrating...");
                detector.calibrate();
                Serial.println("Calibration complete!");
                break;
            case 'r':
            case 'R':
                Serial.println("System reset");
                setup();
                break;
            default:
                break;
        }
    }
}

// MetalDetector class implementation
MetalDetector::MetalDetector(int pin) : sensorPin(pin), calibrationValue(0), detectionThreshold(100) {}

void MetalDetector::begin() {
    pinMode(sensorPin, INPUT);
}

float MetalDetector::readSensor() {
    return analogRead(sensorPin);
}

void MetalDetector::calibrate() {
    int sum = 0;
    for (int i = 0; i < 100; i++) {
        sum += analogRead(sensorPin);
        delay(10);
    }
    calibrationValue = sum / 100.0;
}

bool MetalDetector::isMetalDetected() {
    float currentValue = readSensor();
    float difference = abs(currentValue - calibrationValue);
    return difference > detectionThreshold;
}

float MetalDetector::getWidth() {
    float sensorValue = readSensor();
    return (sensorValue - calibrationValue) * 0.1; // Convert to mm
}

float MetalDetector::getLength() {
    // Length calculation based on detection duration
    // This is a placeholder implementation
    return 25.5;
}

void MetalDetector::setThreshold(float value) {
    detectionThreshold = value;
}