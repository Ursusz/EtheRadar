#include "Buzzer.h"

void Buzzer::setOn(){
  shouldPlay = true;
}

void Buzzer::setOff(){
  shouldPlay = false;
  noTone(pin);
}

void Buzzer::update(){
  if(!shouldPlay) return;
  if(millis() - lastUpdateTime > delay){
    tone(pin, freq, duration);

    lastUpdateTime = millis();
  }
}