/* replace ? and add code in ???*/
#include <Arduino.h>
#include <DHT.h>
#include "NewPing.h"
#include <ESP32Servo.h>

//define pins
#define LED_PIN 1
#define DHT_PIN   4
#define TRIG_PIN 16        
#define ECHO_PIN  17       
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

  // read humidity
  float humi  = dht11.readHumidity();
  // read temperature in Celsius
  float tempC = dht11.readTemperature();
  // read temperature in Fahrenheit
  float tempF = dht11.readTemperature(true);

  // check whether the reading is successful or not
  if ( isnan(tempC) || isnan(tempF) || isnan(humi)) {
    Serial.println("Failed to read from DHT11 sensor!");
    return;
  }
  
    Serial.print("Temp: ");
  Serial.print(tempC);
  Serial.print(" °C | Humidity: ");
  Serial.print(humi);
  Serial.println(" %");

  // ---------------- SAFE ----------------
  if (tempC <= 30) {

    // Servo stop
    servo.write(0);

    // Serial message
    Serial.println("Status: SAFE");

    // Buzzer off
    noTone(BUZZER_PIN);
  }

  // ---------------- CAUTION ----------------
  else if (humi >= 30) {

    // Servo stop
    servo.write(0);

    // Serial message
    Serial.println("Status: CAUTION!!!");

    // Buzzer continuous alarm
    tone(BUZZER_PIN, 1000);

    // Report distance of heat source
    float distance = sonar.ping_cm();
    Serial.print("Heat source distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  }

  // ---------------- DANGEROUS ----------------
  else if (humi < 30) {

    // Servo run (fan on)
    servo.write(90);

    // Serial message
    Serial.println("Status: DANGEROUS!!!");

    // Buzzer beeping
    tone(BUZZER_PIN, 2000);
    delay(300);
    noTone(BUZZER_PIN);
    delay(300);
  }

  // wait a 2 seconds between readings
  delay(2000);
}
