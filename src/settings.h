#include <config.h>

#define EEPROM_START 0
#ifndef DEFAULT_SENDER_ENABLED 
#define DEFAULT_SENDER_ENABLED false
#endif
#ifndef DEFAULT_STA_ENABLED
#define DEFAULT_STA_ENABLED false
#endif
#ifndef DEFAULT_DISABLE_AP
#define DEFAULT_DISABLE_AP false
#endif
#ifndef DEFAULT_HTTP_AUTH_ENABLED
#define DEFAULT_HTTP_AUTH_ENABLED false
#endif
#ifndef STA_SSID_DEFAULT
#define STA_SSID_DEFAULT "CLIENTSSID"
#endif
#ifndef STA_PASSWORD_DEFAULT
#define STA_PASSWORD_DEFAULT "WiFinetKEY"
#endif
#ifndef CONFIG_HTTP_USER_DEFAULT
#define CONFIG_HTTP_USER_DEFAULT "123"
#endif
#ifndef CONFIG_HTTP_PASSWORD_DEFAULT
#define CONFIG_HTTP_PASSWORD_DEFAULT "321"
#endif

#ifndef EEPROM_DATA_DEFINED__
#define EEPROM_DATA_DEFINED__ 
struct eeprom_data_t {
  bool senderEnabled;
  bool STAenabled;
  bool disableAP;
  bool CONFIGauthEnabled;
  char STAssid[17];
  char STApass[17];
  char CONFIGuser[10];
  char CONFIGpass[10];
};
#endif

void readSettings(struct eeprom_data_t eeprom_data, system_info_t& info);
