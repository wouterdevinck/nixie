#include "NixieDisplay.h"

NixieDisplay::NixieDisplay() {}

void NixieDisplay::begin() {
  pinMode(PIN_HV_LE, OUTPUT);
  pinMode(PIN_HV_POL, OUTPUT);
  pinMode(PIN_HV_BL, OUTPUT);
  pinMode(PIN_HV_DATA, OUTPUT);
  pinMode(PIN_HV_CLK, OUTPUT);
  digitalWrite(PIN_HV_BL, HIGH);
  digitalWrite(PIN_HV_POL, HIGH);
  disableAllSegments();
  updateDisplay(); // Write 64 zeros into the shift registers before enabling HV
}

void NixieDisplay::enableSegment(byte segment) {
  byte f = 7 - (segment / 8);
  byte b = segment % 8;
  _frame[f] |= 1 << b;  
}

void NixieDisplay::disableSegments(const byte segments[], int count) {
  for(int i = 0; i < count; ++i) {
    byte segment = segments[i];
    disableSegment(segment);
  } 
}

void NixieDisplay::disableAllSegments() {
  for(int i = 0; i < 8; ++i) {
    _frame[i] = 0b00000000;
  }
}

void NixieDisplay::disableSegment(byte segment) {
  byte f = 7 - (segment / 8);
  byte b = segment % 8;
  _frame[f] &= ~(1 << b);  
}

void NixieDisplay::updateDisplay() {
  digitalWrite(PIN_HV_LE, LOW);
  for(int i = 0; i < 8; ++i) {
    shiftOut(PIN_HV_DATA, PIN_HV_CLK, MSBFIRST, _frame[i]);
  }
  digitalWrite(PIN_HV_LE, HIGH);
}

void NixieDisplay::runSlotMachine() {
  for (int j = 0; j < 5; ++j) {
    for (int i = 0; i < 10; ++i) {
      disableAllSegments();
      enableSegment(hourTens[i]);
      enableSegment(hourUnits[i]);
      enableSegment(minuteTens[i]);
      enableSegment(minuteUnits[i]);
      enableSegment(secondTens[i]);
      enableSegment(secondUnits[i]);
      updateDisplay();
      delay(100);
    }
  }
}
