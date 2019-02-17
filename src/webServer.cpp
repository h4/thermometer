#include <ESP8266WebServer.h>

#include <config.h>
#include <settings.h>
#include <webServer.h>
#include <handlers/rootHandler.h>
#include <handlers/configHandler.h>

void setupRoutes(ESP8266WebServer& server, system_info_t& info, eeprom_data_t& eeprom_data) {
  server.addHandler(new RootHandler(info));
  server.addHandler(new ConfigHandler(info, eeprom_data));
}
