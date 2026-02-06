#include "TFTDisplay.h"

void TFTDisplay::init(){
  tft.init();
  tft.setRotation(0);
  clearScreen();
  tft.setTextColor(TFT_WHITE);
  sprite.setColorDepth(8);
  sprite.createSprite(100, 100);
}
void TFTDisplay::drawStartupScreen(){
  tft.setTextColor(TFT_BLACK);
  tft.drawRect(0, 0, 240, 240, TFT_DARKCYAN);
  tft.fillRoundRect(10, 10, 220, 30, 5, TFT_CYAN);
  tft.setTextSize(2);
  tft.drawCentreString("EtheRadar", tft.width() / 2, 10 + 30 / 2 - tft.fontHeight(2) / 2, 2);
  uint8_t R = 70;
  uint8_t r = 25;
  uint8_t centerX = tft.width() / 2;
  uint8_t centerY = (tft.height() + 40) / 2;
  tft.drawCircle(centerX, centerY, R, TFT_DARKGREY);
  tft.drawCircle(centerX, centerY, r, TFT_DARKGREY);
  
  tft.drawLine(centerX, centerY - R, centerX, centerY + R, TFT_GREEN);
  tft.drawLine(centerX - R, centerY, centerX + R, centerY, TFT_GREEN);
  
  float half_sqrt2 = 0.707;
  tft.drawLine(centerX - R * half_sqrt2, centerY - R * half_sqrt2, centerX + R * half_sqrt2, centerY + R * half_sqrt2, TFT_GREEN);
  tft.drawLine(centerX - R * half_sqrt2, centerY + R * half_sqrt2, centerX + R * half_sqrt2, centerY - R * half_sqrt2, TFT_GREEN);
  tft.fillCircle(centerX, centerY, 8, TFT_CYAN);
  tft.fillCircle(centerX + 35, centerY + 17, 4, TFT_RED);
  tft.fillCircle(centerX - 50, centerY - 37, 4, TFT_RED);
  delay(STARTUP_SCREEN_DURATION);
  clearScreen();
}

void TFTDisplay::drawMenu(MenuManager& manager){
  tft.setTextColor(TFT_BLACK);
  tft.fillRoundRect(10, 10, 220, 40, 10, TFT_SILVER);
  tft.setTextSize(2);
  tft.drawCentreString("MAIN MENU", tft.width() / 2, 30 - tft.fontHeight(2) / 2, 2);
  tft.setTextSize(2);
  for(int8_t i = 0; i < manager.itemCount; i++){
    if(i == manager.selectedIndex){
      tft.setTextColor(TFT_BLACK, TFT_SKYBLUE);
    }else{
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
    }
    tft.drawCentreString(manager.items[i].label.c_str(), tft.width() / 2, 60 + i*tft.fontHeight(2) + 1, 2);
  }
}

void TFTDisplay::drawDistanceResults(NetworkCFG& b){
  clearScreen();
  tft.setTextColor(TFT_WHITE);

  tft.setTextSize(2);
  tft.drawCentreString(b.ssid, tft.width() / 2, 20, 2);

  tft.setTextSize(2);
  tft.drawCentreString("DISTANCE:", tft.width() / 2, 50, 2);
  tft.setTextSize(4);
  tft.setTextColor(TFT_BLUE);
  tft.drawFloat(b.distance, 2, tft.width() / 2 - 40, tft.height() / 2 - 20);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
  tft.drawCentreString("METERS", tft.width() / 2, 150, 2);
}

void TFTDisplay::clearScreen(){
  tft.fillScreen(TFT_BLACK);
}

void TFTDisplay::drawNetworkCFG(NetworkCFG& result, int8_t flag){
  clearScreen();
  tft.setTextSize(2);
  if(flag == 0){
    tft.drawCentreString("first", tft.width() / 2, 200, 2);
  }
  if(flag == 1){
    tft.drawCentreString("last", tft.width() / 2, 200, 2);
  }
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(3);
  tft.drawCentreString("SSID:", tft.width() / 2, tft.height() / 2 - 50, 2);
  tft.setTextSize(2);
  tft.drawCentreString(result.ssid.c_str(), tft.width() / 2, tft.height() / 2, 2);
}

void TFTDisplay::drawSavedMessage(){
  clearScreen();
  tft.setTextColor(TFT_GREEN);
  tft.setTextSize(3);
  tft.drawCentreString("SAVED", tft.width() / 2, tft.height() / 2 - tft.fontHeight(2) / 2, 2);
}

void TFTDisplay::drawNotSavedMessage(){
  clearScreen();
  tft.setTextColor(TFT_RED);
  tft.setTextSize(3);
  tft.drawCentreString("NOT SAVED", tft.width() / 2, tft.height() / 2 - tft.fontHeight(2) / 2, 2);
}

void TFTDisplay::drawSaved(String& res, int16_t index){
  clearScreen();
  tft.setTextColor(TFT_BLUE);
  tft.setTextSize(2);
  tft.drawCentreString("SSID:", tft.width() / 2, 30, 2);
  tft.drawCentreString(res, tft.width() / 2, tft.height() / 2 - tft.fontHeight(2) / 2, 2);
  tft.drawNumber(index, tft.width() / 2, 220);
}

void TFTDisplay::drawDeletedMessage(){
  clearScreen();
  tft.setTextColor(TFT_GREEN);
  tft.setTextSize(3);
  tft.drawCentreString("DELETED", tft.width() / 2, tft.height() / 2 - tft.fontHeight(2) / 2, 2);
}

void TFTDisplay::drawNotFoundMessage(){
  clearScreen();
  tft.setTextColor(TFT_SILVER);
  tft.setTextSize(3);
  tft.drawCentreString("NOT FOUND", tft.width() / 2, tft.height() / 2 - tft.fontHeight(2) / 2, 2);
}

void TFTDisplay::drawLoading(){
  if(millis() - lastSpriteUpdateTime > spriteUpdateDelay){
    sprite.fillSprite(TFT_BLACK);

    sprite.drawArc(50, 50, R, r, angle, angle + arcLength, TFT_WHITE, TFT_BLACK, true);
    sprite.pushSprite((tft.width() - 100) / 2, (tft.height() - 100) / 2);

    angle += arcSpeed;

    if(angle >= 360){
      angle = -360;
    }

    lastSpriteUpdateTime = millis();
  }
}

void TFTDisplay::drawIntrusion(){
  clearScreen();
  tft.setTextColor(TFT_RED);
  tft.setTextSize(3);
  tft.drawCentreString("INTRUSION!", tft.width() / 2, tft.height() / 2 - tft.fontHeight(2) / 2, 2);
}

void TFTDisplay::drawSentry(){
  uint16_t centerX = tft.width() / 2;
  uint16_t centerY = tft.height() / 2 + 10;
  uint16_t color = TFT_RED;
  uint16_t dimColor = tft.color565(100, 0, 0);

  tft.setTextSize(2);
  tft.setTextColor(color, TFT_BLACK);
  tft.drawCentreString("SENTRY MODE", centerX, 10, 2);

  tft.drawCircle(centerX, centerY, 40, dimColor);
  tft.drawCircle(centerX, centerY, 70, dimColor);
  
  tft.fillCircle(centerX, centerY, 15, color);
  tft.drawCircle(centerX, centerY, 20, TFT_WHITE);

  tft.drawLine(20, 50, 50, 50, color);
  tft.drawLine(20, 50, 20, 80, color);

  tft.drawLine(190, 50, 220, 50, color);
  tft.drawLine(220, 50, 220, 80, color);

  static float angle = 0;
  int lineX = centerX + cos(angle) * 70;
  int lineY = centerY + sin(angle) * 70;
  
  tft.drawLine(centerX, centerY, lineX, lineY, color);
  angle += 0.2;
  tft.setTextSize(1);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.drawCentreString("SCANNING ENVIRONMENT...", centerX, 180, 2);
}