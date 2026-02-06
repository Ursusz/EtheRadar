#include "PermanentStorage.h"

bool PermanentStorage::saveSSID(String& SSID){
  preferences.begin(NAMESPACE.c_str(), false);

  bool locationFound = false;
  for(int i = 0; i < MAX_SSID; i++){
    String res = preferences.getString(KEYS[i].c_str(), "");
    if(res == SSID){
      preferences.end();
      return false;
    }
    if(res == ""){
      preferences.putString(KEYS[i].c_str(), SSID);
      locationFound = true;
      break;
    }
  }
  preferences.end();

  return locationFound;
}

String PermanentStorage::getSavedSSID(uint8_t index){
  preferences.begin(NAMESPACE.c_str(), false);
  String res = "none";
  if(index >=0 && index < MAX_SSID){
    res = preferences.getString(KEYS[index].c_str(), "none");
  }
  preferences.end();
  return res;
}

bool PermanentStorage::deleteSSID(int16_t index){
  if(index < 0 || index >= MAX_SSID){
    return false;
  }
  preferences.begin(NAMESPACE.c_str(), false);

  bool found = false;
  if(preferences.isKey(KEYS[index].c_str())){
    preferences.remove(KEYS[index].c_str());
    found = true;
  }
  
  preferences.end();
  return found;
}

void PermanentStorage::deleteAllSSID(){
  preferences.begin(NAMESPACE.c_str(), false);

  preferences.clear();

  preferences.end();
}

std::vector<String> PermanentStorage::getAllSavedSSID(){
  std::vector<String> cachedList;
  
  preferences.begin(NAMESPACE.c_str(), true);
  
  for(size_t i = 0; i < MAX_SSID; i++){
    String res = preferences.getString(KEYS[i].c_str(), "");

    if(res != "" && res != "none"){
      cachedList.push_back(res);
    }
  }

  preferences.end();
  return cachedList;
}