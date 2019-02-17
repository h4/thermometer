#include <ESP8266WiFi.h>
#include <settings.h>

class WiFiManager
{
public:
    WiFiManager();

    void begin();

    void handleStationModeGotIP(const WiFiEventStationModeGotIP&);
    void handleStationModeDisconnected(const WiFiEventStationModeDisconnected&);
    struct eeprom_data_t eeprom_settings;

private:
    String APmacStr;
    String APmacID;
    String checkClientNetStr;
    String STAmacStr;
    String STAmacID;
    char AP_NameChar[32];
    char AP_PassChar[32];

    bool checkAPinair(String name);
    void softAPinit();
    void STAinit();
};
