#include <Arduino.h>
#include "metal_detector.h"

// ============ ARDUINO PIN CONFIGURATION ============
#define SENSOR_PIN_1 A0        // First metal detector sensor
#define SENSOR_PIN_2 A1        // Second metal detector sensor
#define SENSOR_PIN_3 A2        // Third metal detector sensor

// ============ THRESHOLD CONFIGURATION ============
#define THRESHOLD_VALUE_1 100   // Threshold for detector 1
#define THRESHOLD_VALUE_2 100   // Threshold for detector 2
#define THRESHOLD_VALUE_3 100   // Threshold for detector 3

// ============ APPLICATION SETTINGS ============
#define SERIAL_BAUD 115200
#define UPDATE_INTERVAL 100     // milliseconds

// ============ GLOBAL VARIABLES ============
MetalDetector detector1(SENSOR_PIN_1);
MetalDetector detector2(SENSOR_PIN_2);
MetalDetector detector3(SENSOR_PIN_3);

unsigned long lastUpdateTime = 0;

void setup() {
    Serial.begin(SERIAL_BAUD);
    delay(2000);
    
    Serial.println("\n========================================");
    Serial.println("===    Metal Detector System v1.0    ===");
    Serial.println("========================================\n");
    
    // Initialize all detectors
    Serial.println("[*] Initializing detectors...");
    detector1.begin();
    detector2.begin();
    detector3.begin();
    Serial.println("[+] All sensors initialized\n");
    
    // Set threshold values
    Serial.println("[*] Setting threshold values...");
    detector1.setThreshold(THRESHOLD_VALUE_1);
    detector2.setThreshold(THRESHOLD_VALUE_2);
    detector3.setThreshold(THRESHOLD_VALUE_3);
    Serial.print("[+] Detector 1 threshold: ");
    Serial.println(THRESHOLD_VALUE_1);
    Serial.print("[+] Detector 2 threshold: ");
    Serial.println(THRESHOLD_VALUE_2);
    Serial.print("[+] Detector 3 threshold: ");
    Serial.println(THRESHOLD_VALUE_3);
    Serial.println();
    
    // Calibrate all detectors
    Serial.println("[*] Calibrating all detectors...");
    Serial.print("    Calibrating detector 1... ");
    detector1.calibrate();
    Serial.println("OK");
    
    Serial.print("    Calibrating detector 2... ");
    detector2.calibrate();
    Serial.println("OK");
    
    Serial.print("    Calibrating detector 3... ");
    detector3.calibrate();
    Serial.println("OK");
    
    Serial.println("\n[+] System ready!\n");
}

void loop() {
    unsigned long currentTime = millis();
    
    // Update sensor readings
    if (currentTime - lastUpdateTime >= UPDATE_INTERVAL) {
        lastUpdateTime = currentTime;
        
        // Process all detectors
        float reading1 = detector1.readSensor();
        float reading2 = detector2.readSensor();
        float reading3 = detector3.readSensor();
        
        detector1.processReading(reading1);
        detector2.processReading(reading2);
        detector3.processReading(reading3);
    }
    
    // Handle serial input for commands
    if (Serial.available()) {
        char cmd = Serial.read();
        
        switch(cmd) {
            case 'c':
            case 'C':
                Serial.println("\n[*] Recalibrating all detectors...");
                detector1.calibrate();
                detector2.calibrate();
                detector3.calibrate();
                Serial.println("[+] Calibration complete!\n");
                break;
                
            case 'r':
            case 'R':
                Serial.println("\n[*] System reset...");
                setup();
                break;
                
            case 's':
            case 'S':
                Serial.println("\n========== SYSTEM STATUS ==========");
                Serial.print("Detector 1 - Threshold: ");
                Serial.print(THRESHOLD_VALUE_1);
                Serial.print(", Collecting: ");
                Serial.println(detector1.isCurrentlyCollecting() ? "YES" : "NO");
                
                Serial.print("Detector 2 - Threshold: ");
                Serial.print(THRESHOLD_VALUE_2);
                Serial.print(", Collecting: ");
                Serial.println(detector2.isCurrentlyCollecting() ? "YES" : "NO");
                
                Serial.print("Detector 3 - Threshold: ");
                Serial.print(THRESHOLD_VALUE_3);
                Serial.print(", Collecting: ");
                Serial.println(detector3.isCurrentlyCollecting() ? "YES" : "NO");
                Serial.println("===================================\n");
                break;
                
            case 'h':
            case 'H':
                Serial.println("\n========== COMMANDS ==========");
                Serial.println("c/C - Recalibrate detectors");
                Serial.println("r/R - Reset system");
                Serial.println("s/S - Show status");
                Serial.println("h/H - Show help");
                Serial.println("=============================\n");
                break;
                
            default:
                break;
        }
    }
}