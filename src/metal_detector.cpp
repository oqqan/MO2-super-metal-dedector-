#include "metal_detector.h"

MetalDetector::MetalDetector() 
    : detectionThreshold(100), eventCount(0), isDetecting(false), 
      metalLength(0), conveyorSpeed(0.1) {
    
    for (int i = 0; i < NUM_SENSORS; i++) {
        sensors[i].pin = -1;
        sensors[i].position = 0;
        sensors[i].isActive = false;
        calibrationValues[i] = 0;
    }
}

MetalDetector::~MetalDetector() {}

void MetalDetector::begin() {
    for (int i = 0; i < NUM_SENSORS; i++) {
        if (sensors[i].pin != -1) {
            pinMode(sensors[i].pin, INPUT);
            sensors[i].isActive = true;
        }
    }
}

void MetalDetector::setSensorPin(int sensorId, int pin, float position) {
    if (sensorId >= 0 && sensorId < NUM_SENSORS) {
        sensors[sensorId].pin = pin;
        sensors[sensorId].position = position;
    }
}

void MetalDetector::calibrate() {
    Serial.println("\n[*] Calibrating all sensors...");
    
    for (int sensorId = 0; sensorId < NUM_SENSORS; sensorId++) {
        if (sensors[sensorId].pin == -1) continue;
        
        int sum = 0;
        for (int i = 0; i < 100; i++) {
            sum += analogRead(sensors[sensorId].pin);
            delay(5);
        }
        calibrationValues[sensorId] = sum / 100.0;
        
        Serial.print("  [+] Sensor ");
        Serial.print(sensorId + 1);
        Serial.print(" calibrated - Base value: ");
        Serial.println(calibrationValues[sensorId]);
    }
    Serial.println("[+] Calibration complete!\n");
}

void MetalDetector::setThreshold(float value) {
    detectionThreshold = value;
}

void MetalDetector::setConveyorSpeed(float speed) {
    conveyorSpeed = speed;  // mm per millisecond
}

void MetalDetector::readAllSensors() {
    for (int i = 0; i < NUM_SENSORS; i++) {
        if (sensors[i].pin != -1) {
            float value = analogRead(sensors[i].pin);
            processReading(i, value);
        }
    }
}

void MetalDetector::processReading(int sensorId, float value) {
    if (sensorId < 0 || sensorId >= NUM_SENSORS) return;
    
    float difference = abs(value - calibrationValues[sensorId]);
    
    // Metal geçmeye başladı
    if (difference >= detectionThreshold && !isDetecting) {
        isDetecting = true;
        eventCount = 0;
        detectionStartTime = millis();
        
        Serial.println("\n>>> METAL DETECTION STARTED!");
    }
    
    // Algılama sırasında sensör tetiklendi
    if (isDetecting && difference >= detectionThreshold) {
        // Eğer bu sensör daha önceden kaydedilmemişse ekle
        bool sensorAlreadyRecorded = false;
        for (int i = 0; i < eventCount; i++) {
            if (events[i].sensorId == sensorId) {
                sensorAlreadyRecorded = true;
                break;
            }
        }
        
        if (!sensorAlreadyRecorded && eventCount < NUM_SENSORS) {
            events[eventCount].sensorId = sensorId;
            events[eventCount].timestamp = millis();
            events[eventCount].sensorValue = value;
            eventCount++;
            
            Serial.print("  -> Sensor ");
            Serial.print(sensorId + 1);
            Serial.print(" triggered at ");
            Serial.print(events[eventCount - 1].timestamp - detectionStartTime);
            Serial.println(" ms");
        }
    }
    
    // Metal geçmeyi bitirdi
    if (isDetecting && difference < detectionThreshold && eventCount > 0) {
        unsigned long detectionEndTime = millis();
        unsigned long detectionDuration = detectionEndTime - detectionStartTime;
        
        Serial.print(">>> METAL DETECTION ENDED! Duration: ");
        Serial.print(detectionDuration);
        Serial.println(" ms\n");
        
        isDetecting = false;
        analyzeDetection();
    }
}

void MetalDetector::analyzeDetection() {
    if (eventCount < 2) {
        Serial.println("[!] Not enough sensor data to calculate length");
        return;
    }
    
    // En erken ve en geç tetikleme bulma
    int firstSensorId = events[0].sensorId;
    int lastSensorId = events[0].sensorId;
    unsigned long firstTime = events[0].timestamp;
    unsigned long lastTime = events[0].timestamp;
    
    for (int i = 1; i < eventCount; i++) {
        if (events[i].timestamp < firstTime) {
            firstTime = events[i].timestamp;
            firstSensorId = events[i].sensorId;
        }
        if (events[i].timestamp > lastTime) {
            lastTime = events[i].timestamp;
            lastSensorId = events[i].sensorId;
        }
    }
    
    // Sensörler arasındaki mesafe
    float distanceBetweenSensors = abs(sensors[lastSensorId].position - sensors[firstSensorId].position);
    
    // Geçiş süresi
    unsigned long passageDuration = lastTime - firstTime;
    
    // Metalin uzunluğu hesabı
    // Metalin uzunluğu = sensörler arası mesafe + metalin genişliği
    // Metal ilk sensörü tetikleyip son sensörü tetikleyene kadar geçen sürede hareket eder
    
    if (passageDuration > 0) {
        // Aktual hız = sensörler arası mesafe / geçiş süresi
        float actualSpeed = (float)distanceBetweenSensors / (float)passageDuration;
        
        // Metal genişliği = ilk sensör tetiklenme zamanından sensör bulunduğu konum mesafesi
        // + son sensörün tetiklendiği zamandan sonraki mesafe
        
        // Basit hesap: Metal uzunluğu ≈ conveyor hızı × algılama süresi
        unsigned long totalDetectionTime = lastTime - (events[0].timestamp - detectionStartTime) - 50; // 50ms tolerance
        metalLength = actualSpeed * totalDetectionTime;
    } else {
        metalLength = distanceBetweenSensors;
    }
    
    printDetectionData();
}

float MetalDetector::getMetalLength() {
    return metalLength;
}

int MetalDetector::getDetectedSensorCount() {
    return eventCount;
}

void MetalDetector::printDetectionData() {
    Serial.println("\n========== DETECTION ANALYSIS ==========");
    Serial.print("Sensors triggered: ");
    Serial.println(eventCount);
    
    Serial.println("\nTimeline:");
    for (int i = 0; i < eventCount; i++) {
        Serial.print("  Sensor ");
        Serial.print(events[i].sensorId + 1);
        Serial.print(" - Position: ");
        Serial.print(sensors[events[i].sensorId].position);
        Serial.print(" mm - Time: ");
        Serial.print(events[i].timestamp - detectionStartTime);
        Serial.println(" ms");
    }
    
    Serial.print("\n[RESULT] Metal Length: ");
    Serial.print(metalLength);
    Serial.println(" mm");
    Serial.println("=========================================\n");
}

bool MetalDetector::isCurrentlyDetecting() {
    return isDetecting;
}

void MetalDetector::resetDetection() {
    isDetecting = false;
    eventCount = 0;
    metalLength = 0;
}