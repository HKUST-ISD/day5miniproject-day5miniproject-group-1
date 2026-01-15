/* replace ? and add code in ???*/
#include <Arduino.h>
#include <DHT.h>
#include "NewPing.h"
#include <ESP32Servo.h>

//define pins
#define LED_PIN 1
#define DHT_PIN 4
#define TRIG_PIN 15
#define ECHO_PIN 16
#define BUZZER_PIN 35
#define Servo_PIN 5

Servo servo;

#define DHT_TYPE DHT11
DHT dht11(DHT_PIN, DHT_TYPE);

// Maximum distance we want to ping for (in centimeters).
#define MAX_DISTANCE 400
// NewPing setup of pins and maximum distance.
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  // initialize the DHT11 sensor
  dht11.begin();
  //attach servo pin
  servo.attach(Servo_PIN);
  // config the pin mode of each pin
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  //
}

void loop() {
  // Read humidity and temperature
  float humi = dht11.readHumidity();
  float tempC = dht11.readTemperature();

  // Check if sensor reading failed
  if (isnan(humi) || isnan(tempC)) {
    Serial.println("Failed to read from DHT11 sensor!");
    delay(2000);
    return;
  }

  // Always print current readings
  Serial.print("Temp: ");
  Serial.print(tempC);
  Serial.print(" °C | Humidity: ");
  Serial.print(humi);
  Serial.println(" %");

  // ───────────────────────────────────────────────
  //              Main decision tree
  // ───────────────────────────────────────────────
  if (tempC <= 30.0) {
    // ================= SAFE =================
    servo.write(95);     // servo/fan stopped
    noTone(BUZZER_PIN);  // buzzer off
    Serial.println("Status: SAFE");
  }

  else {
    // Temperature > 30 °C ──► now humidity decides

    if (humi >= 30.0) {
      // ================ CAUTION ================
      servo.write(95);  // servo/fan stopped

      tone(BUZZER_PIN, 1000);  // continuous warning tone

      // Measure distance to heat source
      float distance = sonar.ping_cm();
      if (distance == 0) distance = MAX_DISTANCE;  // handle out-of-range

      Serial.print("Heat source distance: ");
      Serial.print(distance);
      Serial.println(" cm");

      Serial.println("Status: CAUTION!!!");
    }

    else {
      // =============== DANGEROUS ===============
      servo.write(70);  // fan on / servo to d°

      // Beep pattern (non-blocking style would be better, but this is simple)
      tone(BUZZER_PIN, 2000);
      delay(300);
      noTone(BUZZER_PIN);
      delay(300);

      Serial.println("Status: DANGEROUS!!!");
    }
  }

  // Wait before next reading
  delay(2000);
}
