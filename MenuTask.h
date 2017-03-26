#ifndef MenuTask_h
#define MenuTask_h

#include "Arduino.h" // TODO Remove Arduino dependency
#include "Globals.h"
#include "NixieDisplay.h"
#include "Chronodot.h"
#include "HvSupply.h"

class MenuTask {
  public:
    MenuTask(NixieDisplay* nixie, Chronodot* rtc, HvSupply* hv);
    void task();
    void printMenu();
  private: 
    NixieDisplay* _nixie;
    Chronodot* _rtc;
    HvSupply* _hv;
    boolean _mustsetdate = false;
    void printTime();
};

#endif

