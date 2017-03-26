#ifndef NixieDisplay_h
#define NixieDisplay_h

#include "Arduino.h" // TODO Remove Arduino dependency
#include "Globals.h"

// Location of each segment in each tube the 64 bit shift register
const byte leftDot       = 42;
const byte rightDot      = 21;
const byte hourTens[]    = {54,63,62,61,60,59,58,57,56,55};
const byte hourUnits[]   = {44,53,52,51,50,49,48,47,46,45};
const byte minuteTens[]  = {32,41,40,39,38,37,36,35,34,33};
const byte minuteUnits[] = {22,31,30,29,28,27,26,25,24,23};
const byte secondTens[]  = {11,20,19,18,17,16,15,14,13,12};
const byte secondUnits[] = {1,10,9,8,7,6,5,4,3,2};
// Not in use: positions 0 and 43

class NixieDisplay {
  public:
    NixieDisplay();
    void begin();
    void enableSegment(byte segment);
    void disableSegments(const byte segments[], int count);
    void disableAllSegments();
    void disableSegment(byte segment);
    void updateDisplay();
    void runSlotMachine();
  private: 
    // Frame of data to be shifted into 64 bit HV shift register
    byte _frame[8]; // 8 bytes = 64 bits = 6 digits @ 10 bits + 2 dots @ 1 bit + 2 unused bits
};

#endif

