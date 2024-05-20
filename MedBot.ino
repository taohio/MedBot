#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Definirea pinului analogic pentru senzorul EMG
const int emgPin = A0;

// Definirea pinilor pentru servomotoare pe modulul PCA9685
const int fingerServos[] = {0, 1, 2, 3, 4};  // Pin assignments pt degete
const int wristServo = 5;  // Pin assignment pt incheietura

// Intervalul de mișcare al servomotoarelor (de la 0 la 180 de grade)
const int servoMin = 150;  // Valoarea minimă (0 grade)
const int servoMax = 600;  // Valoarea maximă (180 grade)

// Configurarea pragurilor pentru mișcare (pentru a evita mișcările involuntare)
const int thresholdMin = 300;
const int thresholdMax = 600;

void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(60);  // Frecvența de 60 Hz pentru servomotoare

  // Inițializare servomotoare la poziția de mijloc
  for (int i = 0; i < 5; i++) {
    pwm.setPWM(fingerServos[i], 0, (servoMin + servoMax) / 2);
  }
  pwm.setPWM(wristServo, 0, (servoMin + servoMax) / 2);
}

void loop() {
  // Citirea valorii de la senzorul EMG
  int emgValue = analogRead(emgPin);
  
  // Verificarea dacă valoarea EMG este în afara pragurilor pentru a activa servomotoarele
  if (emgValue > thresholdMin && emgValue < thresholdMax) {
    // Mapează valoarea EMG (0-1023) la intervalul de mișcare al servomotoarelor
    int servoPosition = map(emgValue, thresholdMin, thresholdMax, servoMin, servoMax);

    // Setează servomotoarele pentru degete la poziția calculată
    for (int i = 0; i < 5; i++) {
      pwm.setPWM(fingerServos[i], 0, servoPosition);
    }

    // Setează servomotorul pentru încheietură la poziția calculată
    pwm.setPWM(wristServo, 0, servoPosition);
  } else {
    // Dacă valoarea EMG este în afara pragurilor, setează servomotoarele la poziția de mijloc
    for (int i = 0; i < 5; i++) {
      pwm.setPWM(fingerServos[i], 0, (servoMin + servoMax) / 2);
    }
    pwm.setPWM(wristServo, 0, (servoMin + servoMax) / 2);
  }

  // Afișează valorile pentru debug
  Serial.print("EMG Value: ");
  Serial.print(emgValue);
  Serial.print(" -> Servo Position: ");
  Serial.println(map(emgValue, thresholdMin, thresholdMax, servoMin, servoMax));

  delay(50); // Mică întârziere pentru stabilizarea valorilor
}
