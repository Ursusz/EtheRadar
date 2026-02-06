#include "MyRotaryEncoder.h"

MyRotaryEncoder::MyRotaryEncoder(uint8_t CLOCK_PIN /*25*/, uint8_t DT_PIN /*26*/, uint8_t SW_PIN /*27*/, uint8_t buttonDebounceDelay)
  : CLOCK_PIN(CLOCK_PIN), DT_PIN(DT_PIN), SW_PIN(SW_PIN), button(SW_PIN) 
  {
    button.setDebounceTime(buttonDebounceDelay);
  }

void MyRotaryEncoder::init(){
  pinMode(CLOCK_PIN, INPUT);
  pinMode(DT_PIN, INPUT);
  PREV_CLK_STATE = digitalRead(CLOCK_PIN);
}

void MyRotaryEncoder::update(){
  button.loop();

  CLK_STATE = digitalRead(CLOCK_PIN);
  if(CLK_STATE != PREV_CLK_STATE && CLK_STATE == HIGH){
    if(digitalRead(DT_PIN) == HIGH){
      direction = DIRECTION_CCW;
      wasUpdated = true;
    }else{
      direction = DIRECTION_CW;
      wasUpdated = true;
    }
  }
  PREV_CLK_STATE = CLK_STATE;
  
  bool currentButtonState = button.isPressed();
  if(currentButtonState && !prevButtonState){
    wasButtonPress = true;
  }
  prevButtonState = currentButtonState;
}

bool MyRotaryEncoder::getUpdatedState(){
  if(wasUpdated){
    wasUpdated = false;
    return true;
  }
  return false;
}

bool MyRotaryEncoder::wasCW(){
  return (direction == DIRECTION_CW);
}

bool MyRotaryEncoder::wasCCW(){
  return (direction == DIRECTION_CCW);
}

bool MyRotaryEncoder::wasButtonPressed(){
  if(wasButtonPress){
    wasButtonPress = false;
    return true;
  }
  return false;
}