#pragma once

#include <cstdint>
#include "WiFi.h"
#include "NetworkCFG.h"

class WiFiScanner{
  int16_t numberOfNetworksDiscovered = 0;

  const int MEASURED_POWER_1_METER = -42.5; // default value, needs calibration
  const float PATH_LOSS = 3.0; // path loss exponent, how fast the power of the signal is lost when moving away from the source
  float x_est = -60.0; // initial RSSI estimate
  float P = 1.0; // estimation error covariance
  float Q = 0.05; // process noise
  float R = 4.0; // measurement noise (RSSI fluctuations)
  int trackDeviceDelay = 500;
  long long lastTrackDevice = 0;
public:
  NetworkCFG globalBest;
  bool isScanning = false;
  void init();
  void startScanAllChannels();
  NetworkCFG getIndexedNetwork(int16_t idx);
  void trackDevice(uint8_t channel);

  float kalmanFilter(float z);
  float convertRSSItoDistance(float rssi);
  void resetKalmanFilter();
};