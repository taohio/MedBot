#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Definirea pinului analogic pentru senzorul EMG
const int emgPin = A0;

// Definirea pinilor pentru servomotoare pe modulul PCA9685
const int fingerServos[] = {0, 1, 2, 3, 4};  // Pin assignments pt degete
const int fingerPosOpened[] = {1000, 1000, 1000, 1000, 1000};
const int fingerPosClosed[] = {2000, 2000, 2000, 2000, 2000};

const int delay_ = 2500;
const int nrVariante = 2;
const int variante[7][5] = {{2200,2200,2200,2200,2200}, 
                            {700, 700, 700, 700, 700}, 
                            {1500, 1000, 1200, 1000, 1500}, 
                            {1800, 1400, 1800, 1400, 1800}, 
                            {900, 750, 900, 750, 900},  
                            {1800, 1000, 1800, 1500, 800},  
                            {1500, 1400, 1000, 1400, 1000} 
                            };

//Definire pini pentru incheietura
const int wristServo = 5;
const int wristOpened = 100; 
const int wristClosed = 100;

// Configurarea pragurilor pentru mișcare (pentru a evita mișcările involuntare)
//const int thresholdMin = 300;
//const int thresholdMax = 600;

int servoTresholdMin = 500;
int servoTresholdMax = 2400;

int emgValue = 0;
String str__; 
char strChar__;

void setup() {
  Serial.begin(9600);
  pwm.begin();
  // Frecvența de 50 Hz pentru servomotoare
  pwm.setPWMFreq(50);
  
  for (int i = 0; i < 5; i++)  
    pwm.writeMicroseconds(fingerServos[i], 1500);
  pwm.writeMicroseconds(wristServo, 1500);
}

void loop(){
  delay(1500);
  for(int i = 0; i < nrVariante; i++){    
    for (int i__ = 0; i__ < 5; i__++)  
      pwm.writeMicroseconds(fingerServos[i__], variante[i][i__]);
    delay(2500);
  }
}
