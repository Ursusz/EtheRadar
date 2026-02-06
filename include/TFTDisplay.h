#pragma once

#include <TFT_eSPI.h>
#include "MenuManager.h"
#include "NetworkCFG.h"

class TFTDisplay {
  TFT_eSPI tft = TFT_eSPI();
  TFT_eSprite sprite = TFT_eSprite(&tft);

  const uint16_t STARTUP_SCREEN_DURATION = 2000;

  int angle = 0;
  int arcSpeed = 10;
  int arcLength = 90;
  int R = 40;
  int r = 30;
  uint8_t spriteUpdateDelay = 20;
  long long lastSpriteUpdateTime = 0;
public:
  void init();

  void drawStartupScreen();

  void drawMenu(MenuManager& manager);

  void drawDistanceResults(NetworkCFG& b);

  void clearScreen();

  void drawNetworkCFG(NetworkCFG& results, int8_t flag);

  void drawSavedMessage();
  void drawNotSavedMessage();

  void drawDeletedMessage();
  void drawNotFoundMessage();

  void drawSaved(String& res, int16_t index);

  void drawLoading();

  void drawIntrusion();
  void drawSentry();
};