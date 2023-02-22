/*
 * SMART IRRIGATION SYS
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//#include <LiquidCrystal.h>

#define echoPin 2 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 3 //attach pin D3 Arduino to pin Trig of HC-SR04

const int sensorPin = A6;
int sensorValue;
const int moistLimit = 300; // Needed moisture
const int pump1 = 4;
const int pump2 = 5;
const int minLevel = 8; // Minimum water level that ISNT SUFFICIENT
const int maxLevel = 3; //Maximum water level that IS SUFFICIENT

float duration; // variable for the duration of sound wave travel
float distance; // variable for the distance measurement
const long cm;
//long length();

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // put your setup code here, to run once:
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  pinMode(sensorPin, INPUT);//Sets the soil moisture data pin as INPUT
  pinMode(pump1, OUTPUT);
  pinMode(pump2, OUTPUT);

  lcd.begin();
  lcd.backlight();

  lcd.setCursor(3, 0);
  lcd.print("PROJECT BY:");
  lcd.setCursor(3, 1);
  lcd.print("Jovine Mutelani");
  delay(2000);
  lcd.setCursor(0, 0);
  lcd.print("IRRIGATION SYSTEM");

}

void loop() {

  get_water_level();
  sensorValue = analogRead(sensorPin);

  if (sensorValue < moistLimit ) { // Moisture is insufficient
    if (distance >= maxLevel && distance < minLevel) { // Water level is sufficient to water the plant
      update_lcd("IRRIGATING", 1);
      while (sensorValue < moistLimit) {
        digitalWrite(pump1, HIGH); //Start pumping from the Main tank
        sensorValue = analogRead(sensorPin);
      }
      digitalWrite(pump1, LOW);
      digitalWrite(pump2, LOW); // Stop pumping from the reservoir tank
      lcd.clear();
      update_lcd("DONE IRRIGATING", 1);
      delay(1000);
      lcd.clear();
      update_lcd("", 0);

    }
    else { // Water is NOT sufficient to water the plant

      update_lcd("FILLING TANK", 2);
      while (distance > maxLevel) {
        digitalWrite(pump2, HIGH); // Pump from reservoir to the main tank
        get_water_level();
      }
      digitalWrite(pump2, LOW);
      lcd.clear();
      update_lcd("DONE FILLING", 2);
      delay(1000);
      lcd.clear();
      update_lcd("", 0);

    }
  }
  else { //Moisture is sufficient

    digitalWrite(pump1, LOW);
    update_lcd("FILLING TANK", 2);
    while (distance > maxLevel + 1) { // Water level is insufficient
      digitalWrite(pump2, HIGH);
      get_water_level();
    }
    digitalWrite(pump2, LOW);
    lcd.clear();
    update_lcd("DONE FILLING", 2);
    delay(1000);
    lcd.clear();
    update_lcd("", 0);
  }

}


float get_water_level() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  return distance;
}


void update_lcd(String message, int pos) {
  lcd.setCursor(0, 0);
  lcd.print("IRRIGATION SYSTEM");
  lcd.setCursor(pos, 1);
  lcd.print(message);

}
