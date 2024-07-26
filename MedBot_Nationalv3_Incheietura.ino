#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Declararea sensorilor si a modulelor
const int emgPin = A0, sunetPin = A1, soundSensorPin = 3, ledPin = 13, buttonPin=2, button2Pin=4;

// Praguri pentru servo-uri
int servoTresholdMin = 500, servoTresholdMax = 2400;

// Stări pentru senzorii de sunet și EMG
int valEMG = 600;

// Variabile detecție sunet & mușchi
bool voiceDetected = false, muscleFlexed = false;

// Pin assignment pentru servo-urile de la degete
const int fingerServos[] = {0, 1, 2, 3, 4};
const int fingerPosOpened[] = {1000, 1000, 1000, 1300, 1300};
const int fingerPosClosed[] = {2000, 2000, 2000, 2200, 2200};
// Pin pentru servo-ul de la încheietură
const int wristServo = 5, wristOpened = 1500, wristLeft = 2300, wristRight =1000;
int pressCount=0, lastButtonState=0;

// Valoare care ne spune in ce stare se afla mana (este inchisa / deschisa)
bool closedHand = false;

void openHand() { 
    for (int i = 0; i < 5; i++) { 
        pwm.writeMicroseconds(fingerServos[i], fingerPosOpened[i]); 

    } 
    delay(500);
}

void closeHand() { 
    for (int i = 0; i < 5; i++) { 
        pwm.writeMicroseconds(fingerServos[i], fingerPosClosed[i]); 
        
    } 
    delay(500);
}


void openWrist() {
    pwm.writeMicroseconds(wristServo, wristOpened );
}

void moveWrist() {
    pwm.writeMicroseconds(wristServo, wristLeft);
    pwm.writeMicroseconds(wristServo, wristRight);
}


void setup() {
    pinMode(soundSensorPin, INPUT);
    pinMode(buttonPin, INPUT);
    pinMode(button2Pin, INPUT);
    pinMode(ledPin, OUTPUT);
  
    Serial.begin(9600);
    pwm.begin();
    
    // Frecvența de 50 Hz pentru servomotoare
    pwm.setPWMFreq(50);
    
    openHand();
    openWrist();
    Serial.println("Setup complet!");
}

void loop() {


    int sunetValue = digitalRead(soundSensorPin);
    int emgValue = analogRead(emgPin);
    int butonValue= digitalRead(buttonPin);
    int buton2Value=digitalRead(button2Pin);
    
    
    //Valori pentru debugging
    
    /*Serial.print("EMG Sensor Value: ");
    Serial.println(emgValue);
    Serial.print("Button: ");
    Serial.println(butonValue);
    */

    // Verificare dacă senzorul de voce detectează un sunet
    if (sunetValue == HIGH)
     {
        //Serial.println("Sunet detectat!"); // Debugging pentru senzor sunet
        digitalWrite(ledPin, HIGH);
        voiceDetected = true;
    
    } else
    {
        digitalWrite(ledPin, LOW);
        voiceDetected = false;
    }
    

    ///Verificam daca sensorul EMG a citit o valoare care a depasit tresholdul
    muscleFlexed = (emgValue > valEMG);    

    ///închidere / Deschidere mână -> Pe bază de prioritate (Prima dată se verifică sunet)
    if ((voiceDetected && butonValue==HIGH) || !muscleFlexed)
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

    //Prezentare încheietură pe baza unui buton
  if (buton2Value == HIGH && lastButtonState == LOW) {
    delay(50); // Debounce
    pressCount++;
    if (pressCount > 4) {
      pressCount = 1; // Resetăm contorul după a patra apăsare
    }
    
    switch (pressCount) {
      case 1:
        pwm.writeMicroseconds(wristServo, wristLeft);
       // Serial.println("1"); 
        break;
      case 2:
        openWrist();
        //Serial.println("2");
        break;
      case 3:
        pwm.writeMicroseconds(wristServo, wristRight);
       // Serial.println("3");
        break;
      case 4:
        openWrist();
       // Serial.println("4");
        break;
    }
  }
  
  lastButtonState = buton2Value;

delay(100);

}