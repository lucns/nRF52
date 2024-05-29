#include <Arduino.h>
#include <Adafruit_TinyUSB.h> // for Serial

#define LED_PIN 24 // RED_PIN

int value = 0;
bool go = true;

void setup() {
  Serial.begin(9600);
  while (!Serial || millis() < 1000);

  Serial.println("Ready");
  delay(100);

  
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, value);
}

void loop() {
  if (go) value++;
  else value--;
  if (value == 0) go = true;
  else if (value == 255) go = false;
  analogWrite(LED_PIN, value);
  delayMicroseconds(go ? 3000 : 5000);
  
  //Serial.println(value);
}
