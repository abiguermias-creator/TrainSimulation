#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Buttons
const int forwardBtn = 2;
const int backwardBtn = 3;
const int delayBtn = 4;

// LEDs
const int greenLED = 5;
const int yellowLED = 6;
const int redLED = 7;

// Buzzer
const int buzzer = 8;

// Stations
String stations[] = {
  "Addis Ababa",
  "Bishoftu",
  "Adama",
  "Mojo",
  "Dire Dawa"
};

int distances[] = {
  0,
  50,
  100,
  150,
  200
};

const int totalStations = 5;
const int trainSpeed = 60;

int currentStation = 0;
bool delayMode = false;
unsigned long previousMillis = 0;
const long autoInterval = 10000;

void startupSound() {
  tone(buzzer, 1000, 200);
  delay(250);
  tone(buzzer, 1500, 200);
  delay(250);
}

void arrivalSound() {
  tone(buzzer, 2000, 300);
  delay(350);
  tone(buzzer, 2500, 500);
}

void movingSound() {
  tone(buzzer, 1200, 150);
}

void delaySound() {
  for (int i = 0; i < 3; i++) {
    tone(buzzer, 500, 300);
    delay(350);
  }
}

void updateLEDsNormal() {
  digitalWrite(greenLED, HIGH);
  digitalWrite(yellowLED, LOW);
  digitalWrite(redLED, LOW);
}

void updateLEDsMoving() {
  digitalWrite(greenLED, LOW);
  digitalWrite(yellowLED, HIGH);
  digitalWrite(redLED, LOW);
}

void updateLEDsDelay() {
  digitalWrite(greenLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(redLED, HIGH);
}

void displayInfo() {
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print(stations[currentStation]);

  int distanceTravelled = distances[currentStation];
  int distanceLeft = 200 - distanceTravelled;

  float etaHours = (float)distanceLeft / trainSpeed;
  int hours = (int)etaHours;
  int minutes = (etaHours - hours) * 60;

  lcd.setCursor(0, 1);
  lcd.print(distanceTravelled);
  lcd.print("km ");

  lcd.print(hours);
  lcd.print("h");
  lcd.print(minutes);
  lcd.print("m");
}

void displayArrival() {
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("ARRIVED");

  lcd.setCursor(0, 1);
  lcd.print(stations[currentStation]);

  digitalWrite(greenLED, HIGH);
  digitalWrite(yellowLED, LOW);
  digitalWrite(redLED, LOW);

  arrivalSound();
}

void moveForward() {

  if (delayMode)
    return;

  if (currentStation < totalStations - 1) {

    updateLEDsMoving();

    movingSound();

    currentStation++;

    displayInfo();

    delay(1000);

    updateLEDsNormal();

    if (currentStation == totalStations - 1) {
      displayArrival();
    }
  }
}

void moveBackward() {

  if (delayMode)
    return;

  if (currentStation > 0) {

    updateLEDsMoving();

    movingSound();

    currentStation--;

    displayInfo();

    delay(1000);

    updateLEDsNormal();
  }
}