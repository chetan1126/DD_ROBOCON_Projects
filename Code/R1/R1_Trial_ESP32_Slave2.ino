#include "HardwareSerial.h"    
#include "ESP_Motor_Driver.h"
#include <ESP32Servo.h> 

CytronMD Lid2(PWM_DIR, 19, 18, 0, 10000, 8);
CytronMD Lid1(PWM_DIR, 5, 17, 1, 10000, 8);

Servo S1;
Servo S2;


HardwareSerial espSerial(0);
int Solonoid1 = 26;
int Solonoid2 = 25;           
int Roller1 = 32;
int Roller2 = 33;
void setup() {
  Serial.begin(1000000);
  espSerial.begin(2000000);
  pinMode(Solonoid1, OUTPUT);
  pinMode(Solonoid2, OUTPUT);
  pinMode(Roller1, OUTPUT);
  pinMode(Roller2, OUTPUT);
  ESP32PWM::allocateTimer(3);
  S1.setPeriodHertz(50);
  S2.setPeriodHertz(50);
  S1.attach(13);
  S2.attach(12);

}

void loop() {
  while(espSerial.available() > 0){

    String Mod = espSerial.readStringUntil('\n');
    // Serial.println(Mod);
    Mod.trim();

    if(Mod == "X"){
      Lid1.setSpeed(0);
      // Serial.println("x");
    }
    else if(Mod == "Y"){
      Lid1.setSpeed(255);
      // Serial.println("Y");
    }
    else if(Mod == "Z"){
      Lid1.setSpeed(-255);
      // Serial.println("Z");
    }
    if(Mod == "T"){
      S1.write(80);
      S2.write(0);
    }
    else if(Mod == "S"){
      S1.write(0);
      S2.write(80);
    }
    if(Mod == "U"){
      Lid2.setSpeed(0);
      // Serial.println("U");
    }
    else if(Mod == "V"){
      Lid2.setSpeed(255);
      // Serial.println("V");
    }
    else if(Mod == "W"){
      Lid2.setSpeed(-255);
      // Serial.println("w");
    }

    if(Mod == "C"){
      digitalWrite(Solonoid1, HIGH);
      // Serial.println("C");
    }
    else if(Mod == "D"){
      digitalWrite(Solonoid1, LOW);
      // Serial.println("D");
    }
    if(Mod == "E"){
      digitalWrite(Solonoid2, HIGH);
      // Serial.println("e");
    }
    else if(Mod == "F"){
      digitalWrite(Solonoid2, LOW);
      // Serial.println("f");
    }

    if(Mod == "G"){
      digitalWrite(Roller1, HIGH);
      // Serial.println("g");
    }
    else if(Mod == "H"){
      digitalWrite(Roller1, LOW);
      // Serial.println("h");
    }

    if(Mod == "J"){
      digitalWrite(Roller2, HIGH);
      // Serial.println("i");
    }
    else if(Mod == "I"){
      digitalWrite(Roller2, LOW);
      // Serial.println("j");
    }
  }
}
