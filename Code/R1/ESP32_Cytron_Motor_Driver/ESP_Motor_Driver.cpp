#include "ESP_Motor_Driver.h"

CytronMD::CytronMD(MODE mode, uint8_t pin1, uint8_t pin2, uint8_t ch, int16_t freq, uint8_t dycy)
{
  _mode = mode;
  _pin1 = pin1;
  _pin2 = pin2;
  _ch = ch;
  _freq = freq;
  _dycy = dycy;
  
  pinMode(_pin1, OUTPUT);
  pinMode(_pin2, OUTPUT);
  ledcSetup(_ch, _freq, _dycy);
  ledcAttachPin(_pin1, _ch);
}

void CytronMD::setSpeed(int16_t speed)
{
  // Make sure the speed is within the limit.
  if (speed > 255) {
    speed = 255;
  } else if (speed < -255) {
    speed = -255;
  }
  
  // Set the speed and direction.
  switch (_mode) {
    case PWM_DIR:
      if (speed >= 0) {
        ledcWrite(_ch, speed);
        digitalWrite(_pin2, LOW);
      } else {
        ledcWrite(_ch, -speed);
        digitalWrite(_pin2, HIGH);
      }
      break;

    case PWM_PWM:
      if (speed >= 0) {
        analogWrite(_pin1, speed);
        analogWrite(_pin2, 0);
      } else {
        analogWrite(_pin1, 0);
        analogWrite(_pin2, -speed);
      }
      break;
  }
}
