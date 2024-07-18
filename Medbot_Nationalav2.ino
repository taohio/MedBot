#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Declararea sensorilor si a modulelor
const int emgPin = A0, sunetPin = A1, soundSensorPin = 3, ledPin = 13;

// Praguri pentru servo-uri
int servoTresholdMin = 500, servoTresholdMax = 2400;

// Stări pentru senzorii de sunet și EMG
int valSunet = 0, valEMG = 400;

// Variabile Detectie Sunet
unsigned long openTime = 1000, lastVoiceDetectedTime = 0;
bool voiceDetected = false, muscleFlexed = false;

// Pin assignment pentru servo-urile de la degete
const int fingerServos[] = {0, 1, 2, 3, 4};
const int fingerPosOpened[] = {1000, 1000, 1000, 1000, 1000};
const int fingerPosClosed[] = {2000, 2000, 2000, 2000, 2000};

// Pin pentru servo-ul de la încheietură
const int wristServo = 5, wristOpened = 1000, wristClosed = 2000;

// Valoare care ne spune in ce stare se afla mana (este inchisa / deschisa)
bool closedHand = false;

void openHand() { 
    for (int i = 0; i < 5; i++) { 
        pwm.writeMicroseconds(fingerServos[i], fingerPosOpened[i]); 
        delay(50);
    } 
}

void closeHand() { 
    for (int i = 0; i < 5; i++) { 
        pwm.writeMicroseconds(fingerServos[i], fingerPosClosed[i]); 
        delay(50);
    } 
}

void setup() {
    pinMode(soundSensorPin, INPUT);
    pinMode(ledPin, OUTPUT);
    Serial.begin(9600);
    pwm.begin();
    
    // Frecvența de 50 Hz pentru servomotoare
    pwm.setPWMFreq(50);
    
    openHand();
    Serial.println("Setup complet!");
}

void loop() {
    int sunetValue = digitalRead(soundSensorPin);
    int emgValue = analogRead(emgPin);
 
    Serial.print("EMG Sensor Value: ");
    Serial.println(emgValue);

    // Verificare dacă senzorul de voce detectează un sunet
    if (sunetValue == HIGH)
     {
        Serial.println("Sunet detectat!");
        digitalWrite(ledPin, HIGH);
        voiceDetected = true;
        lastVoiceDetectedTime = millis();
    } else
    {
        digitalWrite(ledPin, LOW);
        voiceDetected = false;
    }

    ///Verificam daca sensorul EMG a citit o valoare care a depasit tresholdul
    muscleFlexed = (emgValue > valEMG);    

    ///închidere / Deschidere mână -> Pe bază de prioritate (Prima dată se verifică sunet)
    if((voiceDetected || (millis() - lastVoiceDetectedTime < openTime)) || !muscleFlexed)
    {
        Serial.println("Mână deschisă"); 
        if(closedHand) {
          openHand();
          closedHand = false;
        }
          
    } else
    {
        Serial.println("Mână închisă"); 
        if(!closedHand) 
        {
          closeHand();
          closedHand = true;
        }
    }

    delay(100);
}