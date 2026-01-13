# EtheRadar

# Description
**EtheRadar** is a handheld device based on the ESP32 microcontroller, designed for monitoring, analyzing and locating signals within the 2.4GHz radio spectrum (Wi-Fi and Bluetooth LE). The device scans the environment and provides real-time visual and auditory feedback based on signal strength (RSSI - Recved Signal Strength Indicator), allowing the user to track the proximity of the closest device.  
Key features include:  
- Dynamic Tracking: Ability to lock onto and track the strongest signal or select the n-th strongest signal in the vicinity.
- Device Whitelisting: Users can save specific MAC adresses to internal memory.
- Sentry Mode: A filtering option that ignores saved (known) devices and exclusively scans for new, unknown signals (can be used to alert the user to potential unauthorized devices).

# Bill Of Materials (BOM)
- ESP32 Development board
- OLED Display /LCD Display
- Joystick
- Passive Buzzer
- RGB LED
- Battery
- Breadboard

# Questions
* **What is the system boundary?**  
  Answer: The system boundary is defined physically by the electromagnetically by the effective range of the ESP32's PCB antenna (the local 2.4GHz spectrum).
* **Where does intelligence live?**  
  Answer: The intelligence resides within the ESP32 firmware, specifically in the filtering algorithm that distinguishes between background noise and relevant signals, the whitelisting logic that decides wether a device is a new threat or a known object and the mapping of logarithmic values (dBm) into linear sensory feedback.
* **What is the hardest technical problem?**  
  Answer: The most significant technological challenge is handling the blocking nature of the Wi-Fi scan operation. The standard WiFi.scanNetworks() function stops code execution for several seconds.
* **What is the minimum demo?**  
  Answer: The device boots up autonomously. It displays a list of detected networks. The user selects a network from the list and the screen enters Radar Mode.
* **Why is this not just a tutorial?**  
  Answer: Online tutorials stop at displaying network names. EtheRadar adds layers of complexity.
* **Do you need an ESP32?**  
  Answer: Yes, this project requires an ESP32 because it has the integrated Wi-Fi/Bluetooth radio module. But I already bought one myself, so there is no need to buy one for me.
