#include "ESP_Motor_Driver.h"


// Configure the motor driver.
CytronMD motor1(PWM_DIR, 15, 2, 0, 30000, 8); 


// The setup routine runs once when you press reset.
void setup() {
  
}


// The loop routine runs over and over again forever.
void loop() {
  motor1.setSpeed(-128);
  delay(1000);
  motor1.setSpeed(225);
  delay(1000);
  motor1.setSpeed(128);
  delay(1000);
  motor1.setSpeed(-225);
  delay(1000);
}