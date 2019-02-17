#include <ESP8266WiFi.h>

#include <wifi.h>
#include <config.h>
#include <settings.h>

using namespace std::placeholders;

unsigned long staConnectTime;
unsigned long nextSenderTick;
bool staInitOk;
IPAddress ipSTA;
IPAddress ipAP;
String ipAPstr;
String ipSTAstr;

WiFiManager::WiFiManager()
{}

void WiFiManager::begin()
{
  WiFi.persistent(false);
  WiFi.disconnect(true);
  WiFi.setPhyMode(WIFI_PHY_MODE_11G);
  delay(500);
  
  std::function<void(const WiFiEventStationModeGotIP&)> f = std::bind(&WiFiManager::handleStationModeGotIP, this, _1);
  std::function<void(const WiFiEventStationModeDisconnected&)> f1 = std::bind(&WiFiManager::handleStationModeDisconnected, this, _1);

  WiFi.onStationModeGotIP(f);
  WiFi.onStationModeDisconnected(f1);

  if (eeprom_settings.STAenabled == true )
  {
    Serial.print("STA enabled, checking : ");
    Serial.println(eeprom_settings.STAssid);

    if (checkAPinair(String(eeprom_settings.STAssid)))
    {
      WiFi.mode(WIFI_AP_STA);
      softAPinit();
      STAinit();
      Serial.print(millis() / 1000); Serial.println(": AP found, AP_STA mode");
    }
    else
    {
      WiFi.mode(WIFI_AP);
      softAPinit();
      Serial.print(millis() / 1000); Serial.println(": AP not found, AP mode");
    }
  }

  else
  {
    Serial.println("STA disabled");
    WiFi.mode(WIFI_AP);
    softAPinit();
  }

  Serial.println("WiFI Manager configured");
}

bool WiFiManager::checkAPinair(String name) {
  name.toUpperCase();

  int n = WiFi.scanNetworks();
  if (n == 0)
  {
    return false;
  }

  else
  {
    String nnn;
    for (int i = 0; i < n; ++i)
    {
      nnn = WiFi.SSID(i);
      nnn.toUpperCase();

      if (nnn == name)
      {
        return true;
      }
    }
  }
  return false;
}

void WiFiManager::softAPinit() {
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);

  APmacStr = getAPmacStr();

  getMacId();

  APmacID = "";

  if (mac[WL_MAC_ADDR_LENGTH - 2] < 0x10)  APmacID = APmacID + "0";
  APmacID = APmacID + String(mac[WL_MAC_ADDR_LENGTH - 2], HEX);

  if (mac[WL_MAC_ADDR_LENGTH - 1] < 0x10)  APmacID = APmacID + "0";
  APmacID = APmacID + String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);

  APmacStr.toUpperCase();
  APmacID.toUpperCase();

  String AP_NameString = AP_SSID + APmacID;

  for (int i = 0; i < AP_NameString.length(); i++)
    AP_NameChar[i] = AP_NameString.charAt(i);
  AP_NameChar[AP_NameString.length() ] = 0;

  String AP_PassString = AP_PASSWORD_PREFIX + APmacID + AP_PASSWORD_SUFFIX;

  for (int i = 0; i < AP_PassString.length(); i++)
    AP_PassChar[i] = AP_PassString.charAt(i);
  AP_PassChar[AP_PassString.length() ] = 0;

  WiFi.softAP(AP_NameChar, AP_PassChar);
  Serial.print("AP SSID: ");  Serial.println(AP_NameChar);

  ipAP = WiFi.softAPIP();

  ipAPstr = String(ipAP[0]) + '.' + String(ipAP[1]) + '.' + String(ipAP[2]) + '.' + String(ipAP[3]);
  checkClientNetStr = String(ipAP[0]) + '.' + String(ipAP[1]) + '.' + String(ipAP[2]) + '.' ;

  Serial.print("AP IP address: ");
  Serial.println(ipAPstr);

  Serial.print("AP MAC address: ");
  Serial.println(APmacStr);

  Serial.print("AP password: ");
  Serial.println(AP_PassString);
}

void WiFiManager::STAinit() {
  WiFi.begin(eeprom_settings.STAssid, eeprom_settings.STApass);

  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.macAddress(mac);

  STAmacStr = String(mac[WL_MAC_ADDR_LENGTH - 6], HEX) + ":" +
              String(mac[WL_MAC_ADDR_LENGTH - 5], HEX) + ":" +
              String(mac[WL_MAC_ADDR_LENGTH - 4], HEX) + ":" +
              String(mac[WL_MAC_ADDR_LENGTH - 3], HEX) + ":" +
              String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) + ":" +
              String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);

  STAmacID = "";

  if (mac[WL_MAC_ADDR_LENGTH - 2] < 0x10)  STAmacID = STAmacID + "0";
  STAmacID = STAmacID + String(mac[WL_MAC_ADDR_LENGTH - 2], HEX);

  if (mac[WL_MAC_ADDR_LENGTH - 1] < 0x10)  STAmacID = STAmacID + "0";
  STAmacID = STAmacID + String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);

  STAmacStr.toUpperCase();
  STAmacID.toUpperCase();

  Serial.print("STA MAC address: ");
  Serial.println(STAmacStr);

  Serial.print("STA SSID: ");
  Serial.println(eeprom_settings.STAssid);
}

void WiFiManager::handleStationModeGotIP(const WiFiEventStationModeGotIP& state)
{
    staConnectTime = millis();
    if (staInitOk == false)
    {
      staInitOk = true;
      nextSenderTick =  millis() + 30000;

      ipSTA = state.ip;
      ipSTAstr = String(ipSTA[0]) + '.' + String(ipSTA[1]) + '.' + String(ipSTA[2]) + '.' + String(ipSTA[3]);

      if (eeprom_settings.disableAP == true)
      {
        WiFi.mode(WIFI_STA);
      }

      Serial.print(millis() / 1000); 
      Serial.print(": STA IP address: "); 
      Serial.println(ipSTAstr);
    }
}

void WiFiManager::handleStationModeDisconnected(const WiFiEventStationModeDisconnected&)
{
  if (staInitOk == true)
      {
        staInitOk = false;
        if (eeprom_settings.disableAP == true)
        {
          WiFi.mode(WIFI_AP_STA);
        }
      }
}
