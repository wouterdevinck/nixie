#ifndef ButtonsTask_h
#define ButtonsTask_h

#include "Arduino.h" // TODO Remove Arduino dependency
#include "Globals.h"

class ButtonsTask {
  public:
    ButtonsTask();
    void task();
    void begin();
  private: 
    boolean sw1db;
    boolean sw2db;
    boolean debounce(boolean value, boolean* store);
};

#endif

