#include <Arduino.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Adafruit_BMP085.h>
#include <WiFiManager.h>
#include <Ticker.h>

#define LED_PIN 3

Ticker ticker;
Adafruit_BMP085 bmp;
ESP8266WebServer server(80);

void tick() {
  int state = digitalRead(LED_PIN);
  digitalWrite(LED_PIN, !state);
}

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  Serial.println(myWiFiManager->getConfigPortalSSID());
  ticker.attach(0.2, tick);
}

void handleRoot() {
  String message = "";
  message += bmp.readTemperature();
  message += " °C";
  server.send(200, "text/plain", message);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setupWiFi() {
  pinMode(LED_PIN, OUTPUT);
  ticker.attach(0.6, tick);
  
  WiFiManager wifiManager;
  // wifiManager.resetSettings();

  wifiManager.setAPCallback(configModeCallback);

  if (!wifiManager.autoConnect("Thermo", "password")) {
    Serial.println("failed to connect and hit timeout");
    ESP.reset();
    delay(1000);
  }

  Serial.println("connected...yeey :)");
  Serial.print("IP Address: ");
  WiFi.enableAP(0);
  Serial.println(WiFi.localIP());
  ticker.attach(0.6, tick);
}

void setupSensor() {
  // SDA - D3 (GPIO 0)
  // SCL - D4 (GPIO 2)
  Wire.begin(D3, D4);

  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (true) {
      delay(1000);
    }
  }
  Serial.println("BMP085 connected!");
}

void setupServer() {
  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
}

void setup() {
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
  setupWiFi();
  setupSensor();
  setupServer();
  ticker.detach();
  digitalWrite(LED_PIN, HIGH);
}

void loop() {
  server.handleClient();
}
