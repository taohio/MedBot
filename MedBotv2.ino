#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Definirea pinului analogic pentru senzorul EMG
const int emgPin = A0;

// Definirea pinilor pentru servomotoare pe modulul PCA9685
const int fingerServos[] = {0, 1, 2, 3, 4};  // Pin assignments pt degete
const int fingerPosOpened[] = {400, 400, 400, 400, 400};
const int fingerPosClosed[] = {2400, 2400, 2400, 2400, 2000};

const int wristServo = 5;  // Pin assignment pt incheietura
const int wristOpened = 100; 
const int wristClosed = 100;

// Configurarea pragurilor pentru mișcare (pentru a evita mișcările involuntare)
const int thresholdMin = 300;
const int thresholdMax = 600;

///treshold min = 500
///treshold max = 2400

int emgValue = 0;
String str__; char strChar__;

int fromStrToInt(String s){
  int x = 0;
  for(int i = 0; i < s.length(); i++){
    x = 10 * x + s[i] - '0';
  }
  return x;
}

void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(50);  // Frecvența de 60 Hz pentru servomotoare
  
  for (int i = 0; i < 5; i++)  
    pwm.writeMicroseconds(fingerServos[i], 1500); ///Mijloc
  pwm.writeMicroseconds(wristServo, 1500);
}

void loop() {
  for(; Serial.available() > 0; )
    delay(2), strChar__ = Serial.read(), str__ += strChar__;
  if(str__.length() > 0){
    
    emgValue = fromStrToInt(str__); str__ = "";
    Serial.println(emgValue);

    for (int i = 0; i < 5; i++)
      pwm.writeMicroseconds(fingerServos[i], emgValue);
    ///pwm.writeMicroseconds(wristServo, wristOpened);
  }
  delay(50);
}
