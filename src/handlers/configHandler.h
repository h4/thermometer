#include <ESP8266WebServer.h>
#include <config.h>
#include <settings.h>

class ConfigHandler : public RequestHandler
{
public:
  ConfigHandler(system_info_t &info, eeprom_data_t& eeprom_data);

  bool canHandle(HTTPMethod requestMethod, String requestUri) override;
  bool handle(ESP8266WebServer& server, HTTPMethod requestMethod, String requestUri) override;
  bool canUpload(String requestUri) override;
  void upload(ESP8266WebServer& server, String requestUri, HTTPUpload& upload) override;

private:
  system_info_t _info;
  eeprom_data_t _eeprom_data;
  void displayForm(ESP8266WebServer& server);
  void processForm(ESP8266WebServer& server);
};
