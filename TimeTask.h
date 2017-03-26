#ifndef TimeTask_h
#define TimeTask_h

#include "Arduino.h" // TODO Remove Arduino dependency
#include "Globals.h"
#include "NixieDisplay.h"
#include "Chronodot.h"
#include <Wire.h>

class TimeTask {
  public:
    TimeTask(NixieDisplay* nixie, Chronodot* rtc);
    void task();
    void begin();
  private: 
    NixieDisplay* _nixie;
    Chronodot* _rtc;
};

#endif

