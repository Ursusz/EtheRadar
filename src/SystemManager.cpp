#include "SystemManager.h"

SystemManager::SystemManager() : encoder(25, 26, 27, 50) {}

void SystemManager::init(){
  encoder.init();
  wifiScanner.init();
  tft.init();
  tft.drawStartupScreen();
  menuManager.setMenuItems(mainMenuItems, mainMenuOptions);
}

void SystemManager::update(){
  encoder.update();

  switch(currentSystemState){
    case SystemState::MENU: {
      if(menuManager.wasMenuChanged()){
        tft.drawMenu(menuManager);
      }
      if(encoder.wasButtonPressed()){
        menuManager.select();
      }
      if(encoder.getUpdatedState()){
        if(encoder.wasCW()){
          menuManager.next();
        }else if(encoder.wasCCW()){
          menuManager.prev();
        }
      }
      break;
    }
    case SystemState::FIND_DEVICES: {
      int numberOfDiscoveredNetworks = WiFi.scanComplete();
      if(numberOfDiscoveredNetworks < 0){
        tft.drawLoading();
      }
      NetworkCFG result = {"None", 0.0, -100};
      if(numberOfDiscoveredNetworks > 0){
        result = wifiScanner.getIndexedNetwork(currentNetworkIndex);
        if(!networkDisplayed){
          networkDisplayed = true;
          tft.drawNetworkCFG(result, 0);
        }
        
        if(encoder.getUpdatedState()){
          if(encoder.wasCW()){
            currentNetworkIndex++;
          }else if(encoder.wasCCW()){
            currentNetworkIndex--;
          }
          if(currentNetworkIndex >= 0 && currentNetworkIndex < numberOfDiscoveredNetworks){
            result = wifiScanner.getIndexedNetwork(currentNetworkIndex);
            if(currentNetworkIndex == 0){
              tft.drawNetworkCFG(result, 0);
            }else if(currentNetworkIndex == numberOfDiscoveredNetworks - 1){
              tft.drawNetworkCFG(result, 1);
            }else{
              tft.drawNetworkCFG(result, -1);
            }
          }else{
            currentNetworkIndex = 0;
            networkDisplayed = false;
            currentSystemState = SystemState::MENU;
            tft.clearScreen();
            tft.drawMenu(menuManager);
          }
        }
        if(encoder.wasButtonPressed()){
          currentSystemState = SystemState::TRACK_DEVICE;
          trackedNetworkChannel = WiFi.channel(currentNetworkIndex);
          trackedNetworkSSID = WiFi.SSID(currentNetworkIndex);
          wifiScanner.trackDevice(trackedNetworkChannel);
          currentNetworkIndex = 0;
        }
      }
      break;
    }
    case SystemState::SAVE_DEVICE: {
      int numberOfDiscoveredNetworks = WiFi.scanComplete();
      NetworkCFG result = {"None", 0.0, -100};
      if(numberOfDiscoveredNetworks < 0){
        tft.drawLoading();
      }
      if(numberOfDiscoveredNetworks > 0){
        result = wifiScanner.getIndexedNetwork(currentNetworkIndex);
        if(std::find(cachedSSIDList.begin(), cachedSSIDList.end(), result.ssid) != cachedSSIDList.end()){
          result.ssid += "(s)";
        }
        if(!networkDisplayed){
          networkDisplayed = true;
          tft.drawNetworkCFG(result, 0);
        }
        
        if(encoder.getUpdatedState()){
          if(encoder.wasCW()){
            currentNetworkIndex++;
          }else if(encoder.wasCCW()){
            currentNetworkIndex--;
          }
          if(currentNetworkIndex >= 0 && currentNetworkIndex < numberOfDiscoveredNetworks){
            result = wifiScanner.getIndexedNetwork(currentNetworkIndex);
            if(std::find(cachedSSIDList.begin(), cachedSSIDList.end(), result.ssid) != cachedSSIDList.end()){
              result.ssid += "(s)";
            }
            if(currentNetworkIndex == 0){
              tft.drawNetworkCFG(result, 0);
            }else if(currentNetworkIndex == numberOfDiscoveredNetworks - 1){
              tft.drawNetworkCFG(result, 1);
            }else{
              tft.drawNetworkCFG(result, -1);
            }
          }else{
            currentNetworkIndex = 0;
            currentSystemState = SystemState::MENU;
            tft.clearScreen();
            tft.drawMenu(menuManager);
          }
        }
        if(encoder.wasButtonPressed()){
          currentSystemState = SystemState::MENU;
          String res = WiFi.SSID(currentNetworkIndex);
          bool saved = permanentStorage.saveSSID(res);
          if(saved){
            tft.drawSavedMessage();
            delay(2000);
          }else{
            tft.drawNotSavedMessage();
            delay(2000);
          }
          currentNetworkIndex = 0;
          networkDisplayed = false;
          tft.clearScreen();
          tft.drawMenu(menuManager);
        }
      }
      break;
    }
    case SystemState::TRACK_DEVICE: {
      int numberOfDevices = WiFi.scanComplete();
      if(numberOfDevices >= 0){
        bool found = false;
        if(numberOfDevices > 0){
          found = false;
          
          for(size_t i = 0; i < numberOfDevices; i++){
            if(WiFi.SSID(i) == trackedNetworkSSID){
              found = true;
              int rawRSSI = WiFi.RSSI(i);

              float filteredRSSI = wifiScanner.kalmanFilter(rawRSSI);
              float distance = wifiScanner.convertRSSItoDistance(filteredRSSI);
              NetworkCFG res = {WiFi.SSID(i), distance, rawRSSI};
              tft.drawDistanceResults(res);
              break;
            }
          }
        }

        if(!found){
          NetworkCFG res = {"Disconnected", -1.0, -100};
          tft.drawDistanceResults(res);
        }

        WiFi.scanDelete();
      }
      wifiScanner.trackDevice(trackedNetworkChannel);
      
      if(encoder.wasButtonPressed()){
        currentSystemState = SystemState::MENU;
        networkDisplayed = false;
        tft.clearScreen();
        tft.drawMenu(menuManager);

        WiFi.scanDelete();
        wifiScanner.resetKalmanFilter();
      }
      break;
    }
    case SystemState::SAVED_DEVICES : {
      if(!networkDisplayed){
        networkDisplayed = true;
        String res = permanentStorage.getSavedSSID(currentSavedNetworkIndex);
        tft.drawSaved(res, currentSavedNetworkIndex);
      }

      if(encoder.getUpdatedState()){
        if(encoder.wasCW()){
          currentSavedNetworkIndex++;
        }else if(encoder.wasCCW()){
          currentSavedNetworkIndex--;
        }
        if(currentSavedNetworkIndex >= 0 && currentSavedNetworkIndex < permanentStorage.MAX_SSID){
          String res = permanentStorage.getSavedSSID(currentSavedNetworkIndex);
          tft.drawSaved(res, currentSavedNetworkIndex);
        }else{
          networkDisplayed = false;
          currentSavedNetworkIndex = 0;
          tft.clearScreen();
          tft.drawMenu(menuManager);
          currentSystemState = SystemState::MENU;
        }
      }

      if(encoder.wasButtonPressed()){
        bool deleted = permanentStorage.deleteSSID(currentSavedNetworkIndex);
        currentSavedNetworkIndex = 0;
        networkDisplayed = false;
        tft.clearScreen();
        if(deleted){
          tft.drawDeletedMessage();
          delay(2000);
        }else{
          tft.drawNotFoundMessage();
          delay(2000);
        }
        tft.clearScreen();
        tft.drawMenu(menuManager);
        currentSystemState = SystemState::MENU;
      }
      break;
    }
    case SystemState::SENTRY_MODE: {
      buzzer.update();
      if(encoder.wasButtonPressed()){
        intrusionDetected = false;
        buzzer.setOff();
        currentSystemState = SystemState::MENU;
        tft.clearScreen();
        tft.drawMenu(menuManager);
        break;
      }

      unsigned long currentTime = millis();
      if(currentTime - lastSentryScanTime > sentryScanDelay && !intrusionDetected){
        wifiScanner.startScanAllChannels();
        lastSentryScanTime = currentTime;
      }
      int numberOfDevices = WiFi.scanComplete();
      if(numberOfDevices > 0){
        for(int i = 0; i < numberOfDevices; i++){
          auto it = std::find(cachedSSIDList.begin(), cachedSSIDList.end(), WiFi.SSID(i));
          if(it == cachedSSIDList.end() && !intrusionDetected){
            intrusionDetected = true;
            tft.drawIntrusion();
            buzzer.setOn();
          }
        }
      }
      break;
    }
    default: break;
  }
}

void SystemManager::StartWifiScan(){
  tft.clearScreen();
  currentSystemState = SystemState::FIND_DEVICES;
  wifiScanner.startScanAllChannels();
  currentNetworkIndex = 0;
}

void SystemManager::StartSaveDevices(){
  cachedSSIDList = permanentStorage.getAllSavedSSID();
  tft.clearScreen();
  currentSystemState = SystemState::SAVE_DEVICE;
  wifiScanner.startScanAllChannels();
  currentNetworkIndex = 0;
}

void SystemManager::ListSavedDevices(){
  currentSystemState = SystemState::SAVED_DEVICES;
  currentSavedNetworkIndex = 0;
  networkDisplayed = false;
}

void SystemManager::DeleteSavedDevices(){
  permanentStorage.deleteAllSSID();
  tft.drawDeletedMessage();
  delay(2000);
  tft.clearScreen();
  tft.drawMenu(menuManager);
}

void SystemManager::ScanForUnknownDevices(){
  tft.clearScreen();
  tft.drawSentry();
  cachedSSIDList = permanentStorage.getAllSavedSSID();
  currentSystemState = SystemState::SENTRY_MODE;
}