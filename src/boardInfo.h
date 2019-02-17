#include <Arduino.h>
#include <config.h>

#ifndef MODULE_DESCRIPTION 
#define MODULE_DESCRIPTION "arduinoESP8266 wifi & eeprom setting template"
#endif
#ifndef OTAWEB_BUILD_VERSION 
#define OTAWEB_BUILD_VERSION "NO OTA BUILD"
#endif

void displayBoardInfo(system_info_t& info) 
{
  Serial.println(MODULE_DESCRIPTION);
  Serial.println(OTAWEB_BUILD_VERSION);
  Serial.print("SDK version: "); Serial.println(ESP.getSdkVersion());
  Serial.print("Flash real size: "); Serial.println(info.flashRealSize);
  Serial.print("Firmware compiled for flash: "); Serial.println(info.flashSize);
}
