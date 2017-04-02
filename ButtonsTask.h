#ifndef ButtonsTask_h
#define ButtonsTask_h

#include "Arduino.h" // TODO Remove Arduino dependency
#include "Globals.h"
#include "TimeTask.h"
#include "Chronodot.h"

class ButtonsTask {
  public:
    ButtonsTask(Chronodot* rtc, TimeTask* buddy);
    void task();
    void begin();
  private: 
    boolean sw1db;
    boolean sw2db;
    State _state;
    Chronodot* _rtc;
    TimeTask* _buddy;
    boolean debounce(boolean value, boolean* store);
};

#endif

