#ifndef HvSupply_h
#define HvSupply_h

#include "Arduino.h" // TODO Remove Arduino dependency
#include "Globals.h"

class HvSupply {
  public:
    HvSupply();
    void begin();
    bool isOn();
    void switchOn();
    void switchOff();
  private:
    boolean _hvon = false;
};

#endif

