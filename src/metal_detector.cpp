#include "metal_detector.h"

MetalDetector::MetalDetector(int pin) 
    : sensorPin(pin), calibrationValue(0), detectionThreshold(100),
      signalBuffer(nullptr), bufferCapacity(0), bufferSize(0), 
      isCollecting(false) {}

MetalDetector::~MetalDetector() {
    if (signalBuffer != nullptr) {
        delete[] signalBuffer;
    }
}

void MetalDetector::begin() {
    pinMode(sensorPin, INPUT);
    
    // Initialize buffer with starting capacity
    bufferCapacity = 50;
    signalBuffer = new float[bufferCapacity];
}

void MetalDetector::expandBuffer() {
    int newCapacity = bufferCapacity * 2;
    float* newBuffer = new float[newCapacity];
    
    // Copy existing data
    for (int i = 0; i < bufferSize; i++) {
        newBuffer[i] = signalBuffer[i];
    }
    
    delete[] signalBuffer;
    signalBuffer = newBuffer;
    bufferCapacity = newCapacity;
    
    Serial.print("Buffer expanded to: ");
    Serial.println(newCapacity);
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

void MetalDetector::processReading(float value) {
    float difference = abs(value - calibrationValue);
    
    // Threshold geçildi - başlat
    if (difference >= detectionThreshold && !isCollecting) {
        isCollecting = true;
        bufferSize = 0;
        Serial.println("\n>>> Metal detection started!");
    }
    
    // Veri topla
    if (isCollecting) {
        // Buffer genişletme gerekiyorsa
        if (bufferSize >= bufferCapacity) {
            expandBuffer();
        }
        
        signalBuffer[bufferSize++] = value;
    }
    
    // Threshold'un altına düştü - bitir
    if (isCollecting && difference < detectionThreshold && bufferSize > 0) {
        isCollecting = false;
        
        // Analiz yap
        Serial.print(">>> Metal detection ended! Collected ");
        Serial.print(bufferSize);
        Serial.println(" samples");
        
        // Min/Max hesapla
        float minVal = signalBuffer[0];
        float maxVal = signalBuffer[0];
        
        for (int i = 1; i < bufferSize; i++) {
            if (signalBuffer[i] < minVal) minVal = signalBuffer[i];
            if (signalBuffer[i] > maxVal) maxVal = signalBuffer[i];
        }
        
        Serial.print("Signal range: ");
        Serial.print(minVal);
        Serial.print(" - ");
        Serial.println(maxVal);
    }
}

void MetalDetector::getCollectedData(float*& data, int& size) {
    data = signalBuffer;
    size = bufferSize;
}

void MetalDetector::clearBuffer() {
    bufferSize = 0;
    isCollecting = false;
}

float MetalDetector::getWidth() {
    float sensorValue = readSensor();
    return (sensorValue - calibrationValue) * 0.1; // Convert to mm
}

float MetalDetector::getLength() {
    // Length calculation based on collected samples
    if (bufferSize > 0) {
        return bufferSize * 0.5; // Her sample ~0.5mm
    }
    return 0;
}

void MetalDetector::setThreshold(float value) {
    detectionThreshold = value;
}

bool MetalDetector::isCurrentlyCollecting() {
    return isCollecting;
}