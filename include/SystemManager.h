#pragma once

#include "TFTDisplay.h"
#include "MyRotaryEncoder.h"
#include "MenuManager.h"
#include "WiFiScanner.h"
#include "PermanentStorage.h"
#include "Buzzer.h"
#include <vector>

class SystemManager{
  enum class SystemState{
    MENU,
    FIND_DEVICES,
    SAVE_DEVICE,
    TRACK_DEVICE,
    SAVED_DEVICES,
    DELETE_ALL_SAVED,
    SENTRY_MODE
  };

  MyRotaryEncoder encoder;
  TFTDisplay tft;
  MenuManager menuManager;
  WiFiScanner wifiScanner;
  PermanentStorage permanentStorage;
  Buzzer buzzer;

  void StartWifiScan();
  void StartSaveDevices();
  void ExitSaveDevices();
  void ListSavedDevices();
  void DeleteSavedDevices();
  void ScanForUnknownDevices();

  const static uint8_t mainMenuOptions = 5;
  Menu::menuItem mainMenuItems[mainMenuOptions] = {
    {"FIND DEVICE", [this](){ StartWifiScan(); }},
    {"SAVE DEVICES", [this](){ StartSaveDevices(); }},
    {"SAVED DEVICES", [this](){ ListSavedDevices(); }},
    {"DELETE SAVED", [this](){ DeleteSavedDevices(); }},
    {"SENTRY", [this](){ ScanForUnknownDevices(); }}
  };

  SystemState currentSystemState = SystemState::MENU;

  int16_t currentNetworkIndex = 0;
  String trackedNetworkSSID = "";
  uint8_t trackedNetworkChannel = 0;
  bool networkDisplayed = false;

  int16_t currentSavedNetworkIndex = 0;
  
  std::vector<String> cachedSSIDList;

  uint16_t sentryScanDelay = 10000;
  unsigned long lastSentryScanTime = 0;
  bool intrusionDetected = false;
public:
  SystemManager();

  void init();

  void update();
};