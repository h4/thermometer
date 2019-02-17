#include <Arduino.h>

unsigned long crc_byte(byte *b, int len);
unsigned long crc_update(unsigned long crc, byte data);
