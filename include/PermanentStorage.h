#pragma once

#include <Preferences.h>
#include <vector>

class PermanentStorage{
  String KEYS[8] = {"SSID1", "SSID2", "SSID3", "SSID4", "SSID5", "SSID6", "SSID7", "SSID8"};
  String NAMESPACE = "SAVED_SSID";
  
  Preferences preferences;
  public:
  uint8_t MAX_SSID = 8;
  bool saveSSID(String& SSID);
  String getSavedSSID(uint8_t index);
  bool deleteSSID(int16_t index);
  void deleteAllSSID();
  std::vector<String> getAllSavedSSID();
};