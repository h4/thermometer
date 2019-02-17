#include <Arduino.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Adafruit_BMP085.h>

#include <utils/crc.h>
#include <config.h>
#include <settings.h>
#include <boardInfo.h>
#include <wifi.h>
#include <webServer.h>

#define SSID "ARRIVAL_Staff"
#define PASSWORD "Ch4rg3RD!"

const int ledPin = 0;
const int rxPin = 3;

WiFiClient espClient;
Adafruit_BMP085 bmp;

String header;

WiFiManager wifiManager = WiFiManager();
system_info_t boardInfo;

ESP8266WebServer server(80);

eeprom_data_t eeprom_data;

void setup() 
{
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
  // Serial.setDebugOutput(true);
  boardInfo = getSystemInfo();

  displayBoardInfo(boardInfo);
  readSettings(eeprom_data, boardInfo);

  wifiManager.begin();
  boardInfo.staConnectTime = millis();
  setupRoutes(server, boardInfo);

  server.begin();

  /*
  Wire.pins(0, 2);
  Wire.begin(0, 2);
  if (!bmp.begin()) {
	  Serial.println("Could not find a valid BMP085 sensor, check wiring!");
	  while (1) {}
  }
  Serial.println("BMP085 connected!");
  server.begin();
  Serial.println("Server started.");
  */
}

void loop() 
{
  server.handleClient();
}
