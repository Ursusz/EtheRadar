#include <Arduino.h>
#include "SystemManager.h"

SystemManager systemManager;

void setup(){
  Serial.begin(115200);
  systemManager.init();
}

void loop(){
  systemManager.update();
}