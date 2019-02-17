#include "WString.h"

String humanTimeMillis(unsigned long milli) {
  String s;

  //unsigned long milli;
  //milli = millis();

  unsigned int secs = milli / 1000, mins = secs / 60;
  unsigned int hours = mins / 60, days = hours / 24;
  milli -= secs * 1000;
  secs -= mins * 60;
  mins -= hours * 60;
  hours -= days * 24;

  s += days != 0 ?  (String)days : "";
  s += days != 0 ?  "d " : "";
  s += hours != 0 ?  (String)hours : "";
  s += hours != 0 ?  ":" : "";
  s += mins > 9 ?  "" : "0";
  s += mins;
  s += ":";
  s += secs > 9 ?  "" : "0";
  s += secs;
  /*s += ".";
    if (milli < 10)    s += "00";  else if (milli < 100)    s += "0";
    s += milli;*/

  return s;
}
