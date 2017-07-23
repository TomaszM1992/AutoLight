#include <EEPROM.h>
#include <Timer.h>


const int sensorPin = A0;
const int ledPin = 12;  //pin od diody
const int switchPin = 11; //pin od tactswitcha
const int lightPin = 10;  //pin od przełączania światła
const long readingPeriod = 60; // 60 sekund

boolean recording = false;
int readingIndex = 0;
Timer t;

void setup()
{
  pinMode(ledPin, OUTPUT);
  pinMode(lightPin, OUTPUT);
  pinMode(switchPin, INPUT_PULLUP);
  Serial.begin(9600);
  while (! Serial) {};
  Serial.println("p-pobierz w-wyczysc");
  t.every(readingPeriod * 1000L, takeReading);
  t.every(1000, updateLED);
}

void loop()
{
  if (Serial.available())
  {
    char ch = Serial.read();
  }
  if (digitalRead(switchPin) == LOW)
  {
    recording = true;
  }
  t.update();
}

void takeReading()
{
  byte intensity = 0;
  if (recording)
  {
    digitalWrite(ledPin, LOW);
    int raw = analogRead(sensorPin);
    // 0 do 1024 log10 0 do 3
    byte reading = byte(log10(float(raw)) * 80.0);
    intensity = reading;
    Serial.println(intensity);
    checkLight(intensity);
  }
}

void checkLight(byte value)
{
  if (value < 135)
  {
    digitalWrite(lightPin, HIGH);
  }else
  {
    digitalWrite(lightPin, LOW);
  }
}

void updateLED()
{
  if (recording)
  {
    digitalWrite(ledPin, HIGH);
    delay(50);
    digitalWrite(ledPin, LOW);
  }
}
