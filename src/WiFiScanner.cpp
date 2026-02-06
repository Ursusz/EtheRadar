#include "WiFiScanner.h"

void WiFiScanner::init(){
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  // globalBest is saving the strongest signal between channels
  globalBest = {"None", 0.0, -100};
}

void WiFiScanner::startScanAllChannels(){
  WiFi.scanNetworks(true, true, false, 100);
}

NetworkCFG WiFiScanner::getIndexedNetwork(int16_t idx){
  if(idx >= 0){
    return {WiFi.SSID(idx), 0.0, WiFi.RSSI(idx)};
  }
  return {"", 0.0, -100};
}

float WiFiScanner::kalmanFilter(float z){
  P = P + Q; // predict: increase uncertainty
  float K = P / (P + R); // computer Kalman gain
  x_est = x_est + K * (z - x_est); // update estimate
  P = (1 - K) * P; // update uncertainty
  return x_est; // return filtered RSSI
}

void WiFiScanner::resetKalmanFilter(){
  x_est = -60.0;
  P = 1.0;
  Q = 0.05;
  R = 4.0;
}

float WiFiScanner::convertRSSItoDistance(float rssi){
  return pow(10.0, (MEASURED_POWER_1_METER - rssi) / (10.0 * PATH_LOSS));
}

void WiFiScanner::trackDevice(uint8_t channel){
  if(millis() - lastTrackDevice > trackDeviceDelay){
    WiFi.scanNetworks(true, true, false, 50, channel);
    lastTrackDevice = millis();
  }
}