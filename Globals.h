#ifndef Globals_h
#define Globals_h

typedef enum State {
  Run,
  SetHour,
  SetMinute,
  SetSecond
} State;

// Task scheduler
#define NOTASKS 3
#define TASKLOOPWAITMS 20

// Pins to HV5622 shift registers
#define PIN_HV_LE A1
#define PIN_HV_POL 2
#define PIN_HV_BL A3
#define PIN_HV_DATA A0
#define PIN_HV_CLK A2

// Pin to DC/DC 170V enable pin
#define PIN_HV_EN 6

// Pins to buttons
#define PIN_SW1 4
#define PIN_SW2 5

#endif
