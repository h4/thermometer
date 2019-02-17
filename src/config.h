#include <WString.h>

#ifndef __esp_config
#define __esp_config = true

#define NAME_TAG "ESP"
#define AP_SSID NAME_TAG "8266-"
#define AP_PASSWORD_PREFIX "SECRET" // + mac XXXX
#define AP_PASSWORD_SUFFIX ""
#endif

#ifndef __system_info_defined
#define __system_info_defined
struct system_info_t 
{
public:
    String macId;
    String APMacStr;
    uint32_t flashRealSize;
    uint32_t flashSize;
    bool isEepromSet;
    unsigned int staConnectTime;
};
#endif

String getAPmacStr();
String getMacId();

system_info_t getSystemInfo();
