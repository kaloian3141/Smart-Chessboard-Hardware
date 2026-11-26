#include <Arduino.h>
#include <Wire.h>
#include "ALS31300.h"

ALS31300 sensor(0x60);

void setup() {
    Serial.begin(115200);
    delay(500);
    
    Wire.begin();
    
    Serial.println("\n=== ALS31300 test ===");
    
    if (sensor.begin()) {
        Serial.println("Sensor responds at 0x60");
        
        uint8_t stored;
        if (sensor.readStoredAddress(stored)) {
            Serial.print("Stored address: 0x");
            Serial.println(stored, HEX);
        } else {
            Serial.println("readStoredAddress failed");
        }
    } else {
        Serial.println("No sensor at 0x60");
    }
}

void loop() {
    delay(1000);
}