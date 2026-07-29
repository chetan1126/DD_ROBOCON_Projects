#include <ps5Controller.h>
#include "Arduino_NineAxesMotion.h"      
#include <Wire.h>
#include "ESP_Motor_Driver.h"
#include <ESP32Servo.h> 
#include "HardwareSerial.h"
HardwareSerial espSerial(0);

NineAxesMotion mySensor;         
unsigned long lastStreamTime = 0;     
const int streamPeriod = 10;

int xstep = 0;
int O = 0;

Servo ESC1;
Servo ESC2;


TaskHandle_t Task1;
TaskHandle_t Task2;

int j;
int Q;

int reset;

CytronMD motor1(PWM_DIR, 14, 27, 0, 10000, 8);
CytronMD motor2(PWM_DIR, 13, 12, 1, 10000, 8);
CytronMD motor3(PWM_DIR, 26, 25, 2, 10000, 8);
CytronMD motor4(PWM_DIR, 33, 32, 3, 10000, 8);


unsigned long previousMillis1 = 0;  
const long period1 = 400;

unsigned long previousMillis2 = 0;  
const long period2 = 400;


unsigned long previousmillis = 0;
const long period = 100;

int i;
int rate = 2;
unsigned long previous = 0;
const long per = 50;
unsigned long current;
int point;

int x;                  
int y;                   
float kp = 1;                      
float ki = 0.3;              
float kd = 3;

double Perror, Ierror, Derror;
double Output, Setpoint;
double prerror;

int Share, Options, Touchpad;
int Triangle, Square, Circle, Cross, R1, L1;

int Pt1 = 0;
int curr, previ; 
int perio = 1500;

#define in1 35
#define in2 34
#define in3 36
#define in4 39

int Mode = 0;
int Power = 0;
int Roller = 0;

int Mic = 0;
int M1 = 0;

int val;
int valb;
int vale = 1100;

int D1 = 0;
int D2 = 0;
int D3 = 0;
int D4 = 0;

int v1 = 0;
int v2 = 0;
int v3 = 0;
int v4 = 0;

int Change1, Change2;

int Relay_30A = 5;

int nine_axis();
int error(float Kp, float Ki, float Kd, float Setpoint, float Input);
void right(int u, int v);
void left(int u, int v);
void forward(int u, int v);
void backward(int u, int v);
void clockwise(int z);
void anticlockwise(int z);
void hardstop();

void setup() {
  Serial.begin(1000000);
  espSerial.begin(2000000);
  pinMode(Relay_30A, OUTPUT);
  ps5.begin("D0:BC:C1:A3:40:83");   
  // ps5.begin("7C:66:EF:3C:EF:D5");        
  Wire.begin(); 
	ESP32PWM::allocateTimer(3);
  ESC1.setPeriodHertz(50);
  ESC2.setPeriodHertz(50);
  ESC1.attach(2, 1000, 1705);
  ESC2.attach(15, 1000, 1705);
  

  pinMode(in1, INPUT);
  pinMode(in2, INPUT);
  pinMode(in3, INPUT);
  pinMode(in4, INPUT);
  // Setpoint = 0;

  xTaskCreatePinnedToCore(Maincode, "Task1", 10000, NULL, 1, &Task1, 1);           
  xTaskCreatePinnedToCore(Nine_axis_value, "Task2", 10000, NULL, 1, &Task2, 0);                        
  mySensor.initSensor();          
  mySensor.setOperationMode(OPERATION_MODE_NDOF);  
  mySensor.setUpdateMode(MANUAL);
  delay(300);
}

void Maincode(void * parameter) {
  while(1){
    // Serial.print(x);
    // Serial.print("  ");
    // Serial.print(y);
    // Serial.print("  ");
    // Serial.println(j);
    // Setpoint = 0;
    if(ps5.isConnected() == true){ 
      if(ps5.Share() && Share == 0){
        Share = 1;
        Power++;
        if(Power % 2 == 0){
          digitalWrite(Relay_30A, HIGH);
          espSerial.println('S');
          EMERGENCY_STOP();
        }
        else if(!(Power % 2 == 0)){
          digitalWrite(Relay_30A, LOW);
          espSerial.println('T');
        }
      }
      else if(!(ps5.Share())){
        Share = 0;
      } 


      if(ps5.PSButton()){
        EMERGENCY_STOP();
        abort();
      }
      else{
        current = millis();
        curr = millis();
        int RS = map(ps5.R2Value(), 0, 255, 0, val - 20);
        int LS = map(ps5.L2Value(), 0, 255, 0, val - 20);
        error(kp, ki, kd, 0, O);
        x = constrain(val + Output, 0, 200);
        y = constrain(val - Output, 0, 200);
        // Serial.println(Setpoint);
        // Serial.print("  ");
        // Serial.print(x);
        // Serial.print("  ");
        if(ps5.Square() == 1 && val > 20 && val <= 100 && Square == 0){
          val -= 10;
          Square = 1;
          // Serial.println(val);
        }
        else if(ps5.Circle() == 1 && val >= 0 && val < 100 && Circle == 0){
          val += 10;
          Circle = 1;
          // Serial.println(val);
        }
        else if(!(ps5.Square()) && !(ps5.Circle())) {
          Square = 0;
          Circle = 0;
        } 
        if(ps5.R2Value() >= 100){
          clockwise(RS);
          Setpoint = j;
          // reset = 1;
        }
        else if(ps5.L2Value() >= 100){
          anticlockwise(LS);
          Setpoint = j;
          // reset = 1;
        }
        else if(ps5.Up()){
          error(kp, ki, kd, 0, O);
          ace(rate, per);
          Serial.println(O);
          x = constrain(i + Output, 0, 200);
          y = constrain(i - Output, 0, 200);
          forward(x, y);
        }
        else if(ps5.Down()){
          error(kp, ki, kd, 0, O);
          ace(rate, per);
          x = constrain(i + Output, 0, 200);
          y = constrain(i - Output, 0, 200);
          backward(x, y);
        }
        else if(ps5.Right()){
          error(kp, ki, kd, 0, O);
          ace(rate, per);
          x = constrain(i + Output, 0, 200);
          y = constrain(i - Output, 0, 200);
          right(x, y);
        }
        else if(ps5.Left()){
          ace(rate, per);
          error(kp, ki, kd, 0, O);
          x = constrain(i + Output, 0, 200);
          y = constrain(i - Output, 0, 200);
          left(x, y);
        }
        else{
          hardstop();
          previous = current;
          i = 50;
          Setpoint = j;
          point = 0; 
        }

        if(ps5.Options() == 1 && Options == 0){
          Mode ++;
          Options = 1;
          // Serial.println(Mode);
        }
        else if(!(ps5.Options())){
          Options = 0;
        }

        if(!(Mode % 2 == 0)){
          v1 = digitalRead(in1);
          v2 = digitalRead(in2);
          v3 = digitalRead(in3);
          v4 = digitalRead(in4);
          Serial.print(v1);
          Serial.print("  ");
          Serial.print(v2);
          Serial.print("  ");
          Serial.print(v3);
          Serial.print("  ");
          Serial.println(v4);


          if(Change1 == 0){
            if(v1 == 0 && v2 == 0){
              espSerial.println('X');
            }
            else if(v1 == 0){
              espSerial.println('X');
              Change1 = 1;
            }
            else if(v2 == 0){
              espSerial.println('Y');
            }
          }  
          if(Change2 == 0){
            if(v3 == 0 && v4 == 0){
              espSerial.println('U');
            }
            else if(v3 == 0){
              espSerial.println('U');
              Change2 = 1;
            }
            else if(v4 == 0){
              espSerial.println('V');
            }
          }

          // Serial.println("mode 1");
          if(ps5.L1() == 1 && vale > 1100 && vale <= 1700 && L1 == 0){
            vale -= 5;
            L1 = 1;
            // Serial.println(vale);
          }
          else if(ps5.R1() == 1 && vale >= 1000 && vale < 1700 && R1 == 0){
            vale += 5;
            R1 = 1;
            // Serial.println(vale);
          }
          else if(!(ps5.L1()) && !(ps5.R1())){
            R1 = 0;
            L1 = 0;
          }
          if(ps5.RStickY() >= 100 && ps5.RStickY() < 128){
            Pt1 = 1;
          }
          else if(Pt1 == 1){
            espSerial.println('I');
            ESC1.writeMicroseconds(vale);
            ESC2.writeMicroseconds(vale);
            if (curr - previ >= perio) {
              Pt1 = 0;
              previ = curr;
            }
          }
          else if(ps5.Touchpad() && Touchpad == 0){
            Touchpad = 1;
            Roller++;
            if(Roller % 2 == 0){
              espSerial.println('H');
              ESC1.writeMicroseconds(vale + 15);
              ESC2.writeMicroseconds(vale + 15);
            }
            else if(!(Roller % 2 == 0)){
              espSerial.println('G');
              ESC1.writeMicroseconds(1000);
              ESC2.writeMicroseconds(1000);
            }
          }
          else if(!(ps5.Touchpad())){
            Touchpad = 0;
            previ = curr;
            espSerial.println('J');
            ESC1.writeMicroseconds(vale + 15);
            ESC2.writeMicroseconds(vale + 15);
          }
        }
        
        else if(Mode % 2 == 0){
          Change1 = 0;
          Change2 = 0;
          espSerial.println('G');
          ESC1.writeMicroseconds(1000);
          ESC2.writeMicroseconds(1000);
          v1 = digitalRead(in1);
          v2 = digitalRead(in2);
          v3 = digitalRead(in3);
          v4 = digitalRead(in4);
          

          if(ps5.LStickY() >= 80 && ps5.LStickY() < 128 && D1 == 0){
            D1 = 1;
          }
          else if(ps5.LStickY() < -80 && ps5.LStickY() >= -128 && D2 == 0){
            D2 = 1;
          }
          if(ps5.RStickY() >= 80 && ps5.RStickY() < 128 && D3 == 0){
            D3 = 1;
          }
          else if(ps5.RStickY() < -80 && ps5.RStickY() >= -128 && D4 == 0){
            D4 = 1;
          }

          

          if(v1 == 0 && v2 == 0){
            espSerial.println('X');
          }
          else if (v1 == 0){
            D1 = 0;
            if(D2 == 1){
              espSerial.println('Z');
            }
            else{
              espSerial.println('X');
            }
          }
          else if (v2 == 0){
            unsigned long currentMillis1 = millis();
            espSerial.println('C');
            D2 = 0;
            if(D1 == 1){
              espSerial.println('D');
              if (currentMillis1 - previousMillis1 >= period1) {
                previousMillis1 = currentMillis1;
                espSerial.println('Y');
              }  
            }
            else{
              espSerial.println('X');
              previousMillis1 = currentMillis1;
            }
          }

          if(v3 == 0 && v4 == 0){
            espSerial.println('U');
          }
          else if (v3 == 0){
            D3 = 0;
            if(D4 == 1){
              espSerial.println('W');
            }
            else{
              espSerial.println('U');
            }
          }
          else if (v4 == 0){
            unsigned long currentMillis2 = millis();
            espSerial.println('E');
            D4 = 0;
            if(D3 == 1){
              espSerial.println('F');
              if (currentMillis2 - previousMillis2 >= period2) {
                previousMillis2 = currentMillis2;
                espSerial.println('V');
              }
            }
            else{
              previousMillis2 = currentMillis2;
              espSerial.println('U');
            }
          }
        }
      }
    }
    else if(ps5.isConnected() == false){
      EMERGENCY_STOP();
    }   
  }
}
void Nine_axis_value(void * parameter){
  while(1){
    while (millis() - lastStreamTime >= streamPeriod){
      mySensor.updateEuler();   
      mySensor.updateCalibStatus();
      if(mySensor.readEulerHeading() < 360){
        j = mySensor.readEulerHeading();
      }
      if(Setpoint < 180){
        xstep = Setpoint + 180;
        if(j >= xstep){
          O = j - 360;
          O = O - Setpoint;
        }
        else{
          O = j - Setpoint;
        }
      }
      else if(Setpoint > 180){
        xstep = Setpoint - 180;
        if(j <= xstep){
          O = j + 360;
          O = O - Setpoint;
        }
        else{
          O = j - Setpoint; 
        }
      }
      lastStreamTime = millis();
    }
  }
} 
void loop(){

}

int error(float Kp, float Ki, float Kd, float setpoint, float input){         
  kp = Kp;
  ki=Ki;
  kd=Kd;
   
  Perror = setpoint - input;

  Derror = Perror - prerror; 
  Ierror = Perror + prerror;

  Output = Kp*Perror + Ki*Ierror + Kd*Derror;




  prerror = Perror;
  
  return Output;
}

void backward(int u, int v){
  motor1.setSpeed(u); 
  motor2.setSpeed(u);
  motor3.setSpeed(-v);
  motor4.setSpeed(-v);
  // Serial.println("B"); 
}
void forward(int u, int v){
  motor1.setSpeed(-v); 
  motor2.setSpeed(-v);
  motor3.setSpeed(u);
  motor4.setSpeed(u);
  // Serial.println("F");
}
void right(int u, int v){
  motor1.setSpeed(-v); 
  motor2.setSpeed(u);
  motor3.setSpeed(u);
  motor4.setSpeed(-v);
  // Serial.println("R");
}
void left(int u, int v){
  motor1.setSpeed(u); 
  motor2.setSpeed(-v);
  motor3.setSpeed(-v);
  motor4.setSpeed(u);
  // Serial.println("L");
} 
void clockwise(int w){
  motor1.setSpeed(w); 
  motor2.setSpeed(w);
  motor3.setSpeed(w);
  motor4.setSpeed(w);
  // Serial.println("Cl");
} 
void anticlockwise(int w){
  motor1.setSpeed(-w); 
  motor2.setSpeed(-w);
  motor3.setSpeed(-w);
  motor4.setSpeed(-w);
  // Serial.println("Acl");
  
}
void hardstop(){
  motor1.setSpeed(0); 
  motor2.setSpeed(0);
  motor3.setSpeed(0);
  motor4.setSpeed(0); 
}
void EMERGENCY_STOP(){
  digitalWrite(Relay_30A, HIGH);
  motor1.setSpeed(0); 
  motor2.setSpeed(0);
  motor3.setSpeed(0);
  motor4.setSpeed(0);
  espSerial.println('G');
  espSerial.println('J');
  espSerial.println('U');
  espSerial.println('X');
  ESC1.writeMicroseconds(1000);
  ESC2.writeMicroseconds(1000);
}

int ace(int rate, int per){
  if(i < val && point == 0){
    if(current - previous >= per){
      i += rate;
      point = 1;
    }
  }
  else{
    previous = current;
    point = 0;
  }
  return i;
}
