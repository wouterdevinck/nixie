#ifndef TimeTask_h
#define TimeTask_h

#include "Arduino.h" // TODO Remove Arduino dependency
#include "Globals.h"
#include "NixieDisplay.h"
#include "Chronodot.h"
#include "Settings.h"
#include <Wire.h>

class TimeTask {
  public:
    TimeTask(NixieDisplay* nixie, Chronodot* rtc, Settings* settings);
    void task();
    void begin();
    void setState(State state);
  private: 
    NixieDisplay* _nixie;
    Chronodot* _rtc;
    Settings* _settings;
    State _state;
};

#endif

