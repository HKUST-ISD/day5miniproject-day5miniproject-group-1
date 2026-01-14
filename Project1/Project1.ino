/* replace ? and add code in ???*/
#include "pitches.h"
#include "Arduino.h"
#include "NewPing.h"

// define ultrasonic pins
#define TRIG_PIN 5
#define ECHO_PIN 4

// define buzzer pin ？
#define BUZZER_PIN  35

// Define Built-in LED pin
#define LED_PIN 1

// Maximum distance we want to ping for (in centimeters).
#define MAX_DISTANCE 400

// NewPing setup of pins and maximum distance.
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

//*** Define distance as a variable ***
float distance;

// jingle bells high_tone

int melody_high[] = {

    NOTE_E4, NOTE_E4, NOTE_E4,
    NOTE_E4, NOTE_E4, NOTE_E4,
    NOTE_E4, NOTE_G4, NOTE_C4, NOTE_D4, NOTE_E4,
    NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4,
    NOTE_F4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4,
    NOTE_E4, NOTE_D4, NOTE_D4, NOTE_E4,
    NOTE_D4, NOTE_G4};

int noteDurations_high[] = {
    8, 8, 4,
    8, 8, 4,
    8, 8, 8, 8, 2,
    8, 8, 8, 8,
    8, 8, 8, 16, 16,
    8, 8, 8, 8,
    4, 4};
// jingle bells LOW
int melody_low[] = {

    NOTE_E3, NOTE_E3, NOTE_E3,
    NOTE_E3, NOTE_E3, NOTE_E3,
    NOTE_E3, NOTE_G3, NOTE_C3, NOTE_D3, NOTE_E3,
    NOTE_F3, NOTE_F3, NOTE_F3, NOTE_F3,
    NOTE_F3, NOTE_E3, NOTE_E3, NOTE_E3, NOTE_E3,
    NOTE_E3, NOTE_D3, NOTE_D3, NOTE_E3,
    NOTE_D3, NOTE_G3};

int noteDurations_low[] = {
    8, 8, 4,
    8, 8, 4,
    8, 8, 8, 8, 2,
    8, 8, 8, 8,
    8, 8, 8, 16, 16,
    8, 8, 8, 8,
    4, 4};

void setup()
{

  Serial.begin(115200);

  // set pins mode
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
}

void loop()
{
  // Get the distance from the Ultrasonic Sensor
  float distance = sonar.ping_cm();

  // -------- DISTANCE > 10 cm --------
  if (distance > 10)
  {
    digitalWrite(LED_PIN, HIGH); // turn on built-in LED
    noTone(BUZZER_PIN);
  }

  // -------- 5 cm ≤ DISTANCE ≤ 10 cm --------
  else if (distance <= 10 && distance >= 5)
  {
    digitalWrite(LED_PIN, LOW); // turn off built-in LED

    // play HIGH tone Jingle Bells
    for (int i = 0; i < 26; i++)
    {
      int noteDuration = 1000 / noteDurations_high[i];
      tone(BUZZER_PIN, melody_high[i], noteDuration);
      delay(noteDuration * 1.30);
      noTone(BUZZER_PIN);
    }
  }

  // -------- DISTANCE < 5 cm --------
  else if (distance < 5 && distance > 0)
  {
    digitalWrite(LED_PIN, LOW); // turn off built-in LED

    // play LOW tone Jingle Bells
    for (int i = 0; i < 26; i++)
    {
      int noteDuration = 1000 / noteDurations_low[i];
      tone(BUZZER_PIN, melody_low[i], noteDuration);
      delay(noteDuration * 1.30);
      noTone(BUZZER_PIN);
    }
  }

  delay(100);
}
