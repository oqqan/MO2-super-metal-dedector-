#include <Arduino.h>
#include "metal_detector.h"

// ============ SENSOR PIN CONFIGURATION ============
// Sensors arranged on conveyor belt from left to right
#define SENSOR_1_PIN A0   // Position: 0 mm
#define SENSOR_2_PIN A1   // Position: 50 mm
#define SENSOR_3_PIN A2   // Position: 100 mm
#define SENSOR_4_PIN A3   // Position: 150 mm
#define SENSOR_5_PIN A4   // Position: 200 mm
#define SENSOR_6_PIN A5   // Position: 250 mm

// ============ DETECTION CONFIGURATION ============
#define DETECTION_THRESHOLD 100        // Sensitivity level
#define CONVEYOR_SPEED 0.1            // mm per millisecond (adjust based on actual speed)

// ============ APPLICATION SETTINGS ============
#define SERIAL_BAUD 115200
#define UPDATE_INTERVAL 50            // milliseconds

// ============ GLOBAL VARIABLES ============
MetalDetector detector;
unsigned long lastUpdateTime = 0;

void setup() {
    Serial.begin(SERIAL_BAUD);
    delay(2000);
    
    Serial.println("\n========================================");
    Serial.println("===  Metal Length Detection System   ===");
    Serial.println("===         Conveyor Belt Mode       ===");
    Serial.println("========================================\n");
    
    // Configure sensors with positions
    Serial.println("[*] Configuring sensors...");
    detector.setSensorPin(0, SENSOR_1_PIN, 0);
    detector.setSensorPin(1, SENSOR_2_PIN, 50);
    detector.setSensorPin(2, SENSOR_3_PIN, 100);
    detector.setSensorPin(3, SENSOR_4_PIN, 150);
    detector.setSensorPin(4, SENSOR_5_PIN, 200);
    detector.setSensorPin(5, SENSOR_6_PIN, 250);
    Serial.println("[+] Sensor positions configured (0-250mm)\n");
    
    // Initialize sensors
    Serial.println("[*] Initializing all sensors...");
    detector.begin();
    Serial.println("[+] All sensors initialized\n");
    
    // Set detection parameters
    detector.setThreshold(DETECTION_THRESHOLD);
    detector.setConveyorSpeed(CONVEYOR_SPEED);
    Serial.print("[+] Detection threshold: ");
    Serial.println(DETECTION_THRESHOLD);
    Serial.print("[+] Conveyor speed: ");
    Serial.print(CONVEYOR_SPEED);
    Serial.println(" mm/ms\n");
    
    // Calibrate all sensors
    detector.calibrate();
    
    Serial.println("[+] System ready!\n");
    Serial.println("Place metal on conveyor belt to measure length...");
    Serial.println("Commands: c=calibrate, r=reset, s=status, h=help\n");
}

void loop() {
    unsigned long currentTime = millis();
    
    // Read all sensors continuously
    if (currentTime - lastUpdateTime >= UPDATE_INTERVAL) {
        lastUpdateTime = currentTime;
        detector.readAllSensors();
    }
    
    // Handle serial commands
    if (Serial.available()) {
        char cmd = Serial.read();
        
        switch(cmd) {
            case 'c':
            case 'C':
                detector.calibrate();
                break;
                
            case 'r':
            case 'R':
                Serial.println("\n[*] System reset...");
                detector.resetDetection();
                setup();
                break;
                
            case 's':
            case 'S':
                Serial.println("\n========== SYSTEM STATUS ==========");
                Serial.print("Detecting: ");
                Serial.println(detector.isCurrentlyDetecting() ? "YES" : "NO");
                Serial.print("Last measured length: ");
                Serial.print(detector.getMetalLength());
                Serial.println(" mm");
                Serial.println("===================================\n");
                break;
                
            case 'h':
            case 'H':
                Serial.println("\n========== COMMANDS ==========");
                Serial.println("c/C - Recalibrate all sensors");
                Serial.println("r/R - Reset system");
                Serial.println("s/S - Show status");
                Serial.println("h/H - Show help");
                Serial.println("==============================\n");
                break;
                
            default:
                break;
        }
    }
}