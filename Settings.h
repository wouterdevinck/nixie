#ifndef Settings_h
#define Settings_h

#include "Arduino.h" // TODO Remove Arduino dependency
#include <EEPROM.h>

class Settings {
  public:
    Settings();
    void begin();
    bool getEuropeDstEnabled();
    void setEuropeDstEnabled(bool value);
    bool getEuropeSummerTime();
    void setEuropeSummerTime(bool value);
  private:
    bool _euDstEnabled;
    bool _euSummertime;
};

#endif

