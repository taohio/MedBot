#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

const int emgPin = A0;
const int sunetPin = A1;

// Praguri pentru servo-uri
int servoTresholdMin = 500;
int servoTresholdMax = 2400;

// Stări pentru senzorii de sunet și EMG
int valSunet = 73;
int valEMG = 400;

unsigned long openTime = 2000;
unsigned long lastVoiceDetectedTime = 0;

bool voiceDetected = false;
bool muscleFlexed = false;

// Pin assignment pentru servo-urile de la degete
const int fingerServos[] = {0, 1, 2, 3, 4};
const int fingerPosOpened[] = {1000, 1000, 1000, 1000, 1000};
const int fingerPosClosed[] = {2000, 2000, 2000, 2000, 2000};

// Pin pentru servo-ul de la încheietură
const int wristServo = 5;
const int wristOpened = 1000;
const int wristClosed = 2000;

void setup() {
  Serial.begin(9600);
  pwm.begin();
  // Frecvența de 50 Hz pentru servomotoare
  pwm.setPWMFreq(50);

  for (int i = 0; i < 5; i++) {
    pwm.writeMicroseconds(fingerServos[i], 1500);
  }

  Serial.println("Setup complet!");
}

void loop() {
  int sunetValue = analogRead(sunetPin);
  int emgValue = analogRead(emgPin);
  
  Serial.print("Sunet Sensor Value: ");
  Serial.println(sunetValue);
  Serial.print("EMG Sensor Value: ");
  Serial.println(emgValue);

  // Verificare dacă senzorul de voce detectează un sunet
  if (sunetValue == valSunet) {
    voiceDetected = true;
   lastVoiceDetectedTime = millis();
   Serial.println("Sunet detectat!");
 } else {
    voiceDetected = false;
 }
  // Verificare dacă mușchiul este flexat
  if (emgValue > valEMG) {
    muscleFlexed = true;
    closeHand();
    Serial.println("Mușchi flexat! Închidem mâna.");
  } else {
    muscleFlexed = false;
    openHand();
    Serial.println("Mușchi relaxat! Deschidem mâna.");
  }

   //Verificare dacă sunetul a fost detectat și timpul de deschidere a expirat
  if (voiceDetected && (millis() - lastVoiceDetectedTime > openTime)) {
    if (muscleFlexed) {
     closeHand();
     Serial.println("Revenim la poziția flexată.");
    } else {
      openHand();
      Serial.println("Revenim la poziția deschisă.");
    }
    voiceDetected = false; // Resetare flag voiceDetected
  }

  delay(10);
}

void openHand() {
  for (int i = 0; i < 5; i++) {
    pwm.writeMicroseconds(fingerServos[i], fingerPosOpened[i]);
  }
}

void closeHand() {
  for (int i = 0; i < 5; i++) {
    pwm.writeMicroseconds(fingerServos[i], fingerPosClosed[i]);
  }
}
