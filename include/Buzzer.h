#pragma once

#include "Arduino.h"

class Buzzer{
  int pin = 14;
  int freq = 500;
  int delay = 1000;
  int duration = 500;
  unsigned long lastUpdateTime = 0;

  bool shouldPlay = false;
public:
  void update();
  void setOn();
  void setOff();    
};