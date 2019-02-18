#include <Arduino.h>
#include <EEPROM.h>

#include <utils/crc.h>
#include <config.h>
#include <settings.h>

boolean setEEPROM = false;
uint32_t memcrc; 
uint8_t *p_memcrc = (uint8_t*)&memcrc;

void readSettings(eeprom_data_t& eeprom_data, system_info_t& info)
{
  int i;
  uint32_t datacrc;
  byte eeprom_data_tmp[sizeof(eeprom_data)];

  EEPROM.begin(sizeof(eeprom_data) + sizeof(memcrc));

  for (i = EEPROM_START; i < EEPROM_START + sizeof(eeprom_data); i++)
  {
    eeprom_data_tmp[i] = EEPROM.read(i);
  }

  p_memcrc[0] = EEPROM.read(i++);
  p_memcrc[1] = EEPROM.read(i++);
  p_memcrc[2] = EEPROM.read(i++);
  p_memcrc[3] = EEPROM.read(i++);

  datacrc = crc_byte(eeprom_data_tmp, sizeof(eeprom_data_tmp));

  if (memcrc == datacrc)
  {
    info.isEepromSet = true;
    memcpy(&eeprom_data, eeprom_data_tmp, sizeof(eeprom_data));
    Serial.println("Settings read");
    Serial.print("SSID: "); Serial.println(eeprom_data.STAssid);
  }
  else
  {
    Serial.println("Load defaults");
    eeprom_data.senderEnabled = DEFAULT_SENDER_ENABLED;
    eeprom_data.STAenabled = DEFAULT_STA_ENABLED;
    eeprom_data.disableAP = DEFAULT_DISABLE_AP;
    eeprom_data.CONFIGauthEnabled = DEFAULT_HTTP_AUTH_ENABLED;
    strncpy(eeprom_data.STAssid, STA_SSID_DEFAULT, sizeof(STA_SSID_DEFAULT));
    strncpy(eeprom_data.STApass, STA_PASSWORD_DEFAULT, sizeof(STA_PASSWORD_DEFAULT));
    strncpy(eeprom_data.CONFIGuser, CONFIG_HTTP_USER_DEFAULT, sizeof(CONFIG_HTTP_USER_DEFAULT));
    strncpy(eeprom_data.CONFIGpass, CONFIG_HTTP_PASSWORD_DEFAULT, sizeof(CONFIG_HTTP_PASSWORD_DEFAULT));
  }
}

void writeSettings(eeprom_data_t& eeprom_data)
{
  int i;
  byte eeprom_data_tmp[sizeof(eeprom_data)];

  EEPROM.begin(sizeof(eeprom_data) + sizeof(memcrc));

  memcpy(eeprom_data_tmp, &eeprom_data, sizeof(eeprom_data));

  for (i = EEPROM_START; i < EEPROM_START+sizeof(eeprom_data); i++)
  {
    EEPROM.write(i, eeprom_data_tmp[i]);
  }
  memcrc = crc_byte(eeprom_data_tmp, sizeof(eeprom_data_tmp));

  EEPROM.write(i++, p_memcrc[0]);
  EEPROM.write(i++, p_memcrc[1]);
  EEPROM.write(i++, p_memcrc[2]);
  EEPROM.write(i++, p_memcrc[3]);

  EEPROM.commit();
}
