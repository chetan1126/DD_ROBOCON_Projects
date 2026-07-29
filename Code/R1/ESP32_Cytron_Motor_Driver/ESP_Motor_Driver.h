#ifndef ESP_Motor_DRIVER_H
#define ESP_Motor_DRIVER_H

#include <Arduino.h>
#include <stdint.h>



enum MODE {
  PWM_DIR,
  PWM_PWM,
};



class CytronMD
{
  public:
    CytronMD(MODE mode, uint8_t pin1, uint8_t pin2, uint8_t ch, int16_t freq, uint8_t dycy);
    void setSpeed(int16_t speed);
    
  protected:
    MODE _mode;
  	uint8_t _pin1;
    uint8_t _pin2;
    uint8_t _ch; 
    uint16_t _freq;
    uint8_t _dycy;
};

#endif
