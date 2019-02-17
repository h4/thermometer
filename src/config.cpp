#include <ESP8266WiFi.h>
#include <config.h>

String getAPmacStr()
{
    unsigned long begin = micros();
    uint8_t mac[WL_MAC_ADDR_LENGTH];
    WiFi.softAPmacAddress(mac);

    String APmacStr = String(mac[WL_MAC_ADDR_LENGTH - 6], HEX) + ":" +
             String(mac[WL_MAC_ADDR_LENGTH - 5], HEX) + ":" +
             String(mac[WL_MAC_ADDR_LENGTH - 4], HEX) + ":" +
             String(mac[WL_MAC_ADDR_LENGTH - 3], HEX) + ":" +
             String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) + ":" +
             String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
    APmacStr.toUpperCase();

    unsigned long end = micros();

   Serial.println("");
   Serial.print("Getting AP mac string took "); Serial.println(end - begin);
    return APmacStr;
};

String getMacId()
{
    unsigned long begin = micros();
    uint8_t mac[WL_MAC_ADDR_LENGTH];
    WiFi.softAPmacAddress(mac);
    String APmacID = "";

   if (mac[WL_MAC_ADDR_LENGTH - 2] < 0x10)  APmacID = APmacID + "0";
   APmacID = APmacID + String(mac[WL_MAC_ADDR_LENGTH - 2], HEX);

   if (mac[WL_MAC_ADDR_LENGTH - 1] < 0x10)  APmacID = APmacID + "0";
   APmacID = APmacID + String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);

   APmacID.toUpperCase();

   unsigned long end = micros();

   Serial.println("");
   Serial.print("Getting mac id took "); Serial.println(end - begin);

   return APmacID;
}

system_info_t getSystemInfo()
{
    system_info_t info;
    String macId = getMacId();
    String APMacStr = getAPmacStr();

    info.isEepromSet = false;
    info.macId = macId;
    info.APMacStr = APMacStr;
    info.flashRealSize = ESP.getFlashChipRealSize();
    info.flashSize = ESP.getFlashChipSize();

    return info;
}
