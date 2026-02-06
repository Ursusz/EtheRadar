#pragma once

#include <ezButton.h>

class MyRotaryEncoder{
  const uint8_t CLOCK_PIN;
  const uint8_t DT_PIN;
  const uint8_t SW_PIN;

  ezButton button;

  const uint8_t DIRECTION_CW = 0;
  const uint8_t DIRECTION_CCW = 1;

  uint8_t direction = DIRECTION_CW;
  uint8_t CLK_STATE;
  uint8_t PREV_CLK_STATE;

  bool wasUpdated = false;
  bool wasButtonPress = false;
  bool prevButtonState = false;
  public:
  MyRotaryEncoder(uint8_t CLOCK_PIN, uint8_t DT_PIN, uint8_t SW_PIN, uint8_t buttonDebounceDelay);
  
  void init();
  
  void update();
  
  bool getUpdatedState();

  bool wasCW();

  bool wasCCW();

  bool wasButtonPressed();
};