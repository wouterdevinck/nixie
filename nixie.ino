// Nixie clock firmware
// ====================
// December 2016
// by Wouter Devinck

// Dependencies:
//  * Chronodot library (for DS3231)   - https://github.com/Stephanie-Maks/Arduino-Chronodot

#include "Chronodot.h"
#include <Wire.h>

// Pins to buttons
const int pinSW1 = 4;
const int pinSW2 = 5;

// Pins to HV5622 shift registers
const int pinHvLE = A1;
const int pinHvPOL = 2;
const int pinHvBL = A3;
const int pinHvData = A0;
const int pinHvCLK = A2;

// Pin to DC/DC 170V enable pin
const int pinHvEN = 6;

// Frame of data to be shifted into 64 bit HV shift register
byte frame[8]; // 8 bytes = 64 bits = 6 digits @ 10 bits + 2 dots @ 1 bit + 2 unused bits

// The real time clock chip (DS3231)
Chronodot RTC;

// Tasks
const int noTasks = 6;
const int wait = 10;
typedef struct Tasks {
   long unsigned int previous;
   int interval;
   void (*function)();
} Task;
Task tasks[noTasks];

// Debounce buttons
boolean sw1db;
boolean sw2db;

// State
boolean dotson = false;
boolean hvon = false;
boolean mustsetdate = false;

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

void setup() {
  
  // Start serial
  Serial.begin(9600);
  Serial.println(F("[INFO] Nixie clock is booting..."));

  // Debug: reset reason
  switch (MCUSR) {
    case 2:
      Serial.println(F("[DEBUG] Reset caused by reset pin or software reset"));
      break;
    case 7:
      Serial.println(F("[DEBUG] Reset caused by loss of power"));
      break;
  }
  MCUSR = 0;

  // Initiate the Real Time Clock
  Serial.println(F("[INFO] 1. Init real time clock"));
  Wire.begin();
  RTC.begin();
  //if (! RTC.isrunning()) {
  //  Serial.println(F("[WARNING] RTC is NOT running!"));
  //  RTC.adjust(DateTime(__DATE__, __TIME__));
  //}
  
  // Initiate the button inputs
  Serial.println(F("[INFO] 2. Init buttons"));
  pinMode(pinSW1, INPUT);
  pinMode(pinSW2, INPUT);  

  // Initiate the HV5622 outputs
  Serial.println(F("[INFO] 3. Init HV shift registers"));
  pinMode(pinHvLE, OUTPUT);
  pinMode(pinHvPOL, OUTPUT);
  pinMode(pinHvBL, OUTPUT);
  pinMode(pinHvData, OUTPUT);
  pinMode(pinHvCLK, OUTPUT);
  digitalWrite(pinHvBL, HIGH);
  digitalWrite(pinHvPOL, HIGH);
  disableAllSegments();
  updateDisplay(); // Write 64 zeros into the shift registers before enabling HV

  // Initiate the DC/DC enable pin
  Serial.println(F("[INFO] 4. Init DC/DC"));
  pinMode(pinHvEN, OUTPUT);
  digitalWrite(pinHvEN, HIGH); // There is now 170V on the board!
  hvon = true;

  // Initiate the tasks
  Serial.println(F("[INFO] 5. Tasks"));
  loadTasks();
  
  // Debug info
  Serial.println(F("[INFO] Clock done booting."));
  printMenu();
  
}

void loadTasks() {
  int i = 0;
  // Listen for input on the serial interface
  tasks[i].previous = 0;
  tasks[i].interval = 100;
  tasks[i].function = serialMenu;
  // Read the button inputs
  i++;
  tasks[i].previous = 0;
  tasks[i].interval = 100;
  tasks[i].function = readButtons;
  // Update time
  i++;
  tasks[i].previous = 0;
  tasks[i].interval = 500;
  tasks[i].function = updateTime;
  // Update dots
  i++;
  tasks[i].previous = 0;
  tasks[i].interval = 1000;
  tasks[i].function = updateDots;
  // Update display
  i++;
  tasks[i].previous = 0;
  tasks[i].interval = 500;
  tasks[i].function = updateDisplay;  
  // Run slot machine effect to prevent cathode poisoning
  i++;
  tasks[i].previous = 0;
  tasks[i].interval = 1000;
  tasks[i].function = unPoison;
}

void loop() {
  unsigned long time = millis();
  for(int i = 0; i < noTasks; i++) {
    Task task = tasks[i];
    if (time - task.previous > task.interval) {
      tasks[i].previous = time;
      task.function();
    }
  }  
  delay(wait);
}

// task
void updateTime() {
  DateTime now = RTC.now();
  disableSegments(hourTens, 10);
  disableSegments(hourUnits, 10);
  disableSegments(minuteTens, 10);
  disableSegments(minuteUnits, 10);
  disableSegments(secondTens, 10);
  disableSegments(secondUnits, 10);
  int hour = now.hour();
  enableSegment(hourTens[(hour / 10) % 10]);
  enableSegment(hourUnits[hour % 10]);
  int minute = now.minute();
  enableSegment(minuteTens[(minute / 10) % 10]);
  enableSegment(minuteUnits[minute % 10]);
  int second = now.second();
  enableSegment(secondTens[(second / 10) % 10]);
  enableSegment(secondUnits[second % 10]);
}

// task
void unPoison() {
  DateTime now = RTC.now();
  if (now.minute() == 0 && now.second() == 0) {
    runSlotMachine();
  }
}

void runSlotMachine() {
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

// task
void updateDots() {
  if (dotson) {
    dotson = false;
    disableSegment(leftDot);
    disableSegment(rightDot);
  } else {
    dotson = true;
    enableSegment(leftDot);
    enableSegment(rightDot);
  }
}

inline void enableSegment(byte segment) {
  byte f = 7 - (segment / 8);
  byte b = segment % 8;
  frame[f] |= 1 << b;  
}

inline void disableSegments(const byte segments[], int count) {
  for(int i = 0; i < count; ++i) {
    byte segment = segments[i];
    disableSegment(segment);
  } 
}

inline void disableAllSegments() {
  for(int i = 0; i < 8; ++i) {
    frame[i] = 0b00000000;
  }
}

inline void disableSegment(byte segment) {
  byte f = 7 - (segment / 8);
  byte b = segment % 8;
  frame[f] &= ~(1 << b);  
}

// task
void updateDisplay() {
  digitalWrite(pinHvLE, LOW);
  for(int i = 0; i < 8; ++i) {
    shiftOut(pinHvData, pinHvCLK, MSBFIRST, frame[i]);
  }
  digitalWrite(pinHvLE, HIGH);
}

// task
void serialMenu() {
  if (Serial.available() > 0) {
    if(mustsetdate) {
      String input = Serial.readString();
      Serial.print(F("Input: "));
      Serial.println(input);
      char* datestr = strtok(input.c_str(), " ");      
      char* timestr = strtok(0, " ");
      if (datestr != 0 && timestr != 0) {
        char* year = strtok(datestr, "-");
        char* month = strtok(0, "-");
        char* day = strtok(0, "-");
        char* hour = strtok(timestr, ":");
        char* minute = strtok(0, ":");
        char* second = strtok(0, ":");
        if(year != 0 && month != 0 && day != 0 && hour != 0 && minute != 0 && second != 0) {
          Serial.print(F("Setting year: "));
          Serial.println(year);
          Serial.print(F("Setting month: "));
          Serial.println(month);
          Serial.print(F("Setting day: "));
          Serial.println(day);
          Serial.print(F("Setting hour: "));
          Serial.println(hour);
          Serial.print(F("Setting minute: "));
          Serial.println(minute);
          Serial.print(F("Setting second: "));
          Serial.println(second);
          RTC.adjust(DateTime(atoi(year), atoi(month), atoi(day), 
            atoi(hour), atoi(minute), atoi(second), 0, 0));
        } else {
          Serial.println(F("[ERROR] Cannot read input - date or time"));
        }
      } else {
        Serial.println(F("[ERROR] Cannot read input"));
      }
      mustsetdate = false;
      printMenu();
    } else {
      int in = Serial.read();
      if (in == 49) {
        Serial.println(F("You have entered [1]"));
        printTime();
        printMenu();
      } else if (in == 50) {
        Serial.println(F("You have entered [2] - Enter date/time in this format: 'YYYY-MM-DD HH:MM:SS'"));
        mustsetdate = true;
      } else if (in == 51) {
        Serial.println(F("You have entered [3] - Running slot machine effect..."));
        runSlotMachine();
        Serial.println(F("Done."));
        printMenu();
      } else if (in == 52) {
        Serial.println(F("You have entered [4] - Toggling HV"));
        if(hvon) {
          digitalWrite(pinHvEN, LOW);
          Serial.println(F("HV supply is now turned off"));
          hvon = false;
        } else {
          digitalWrite(pinHvEN, HIGH);
          Serial.println(F("HV supply is now turned on - 170V on board!!"));
          hvon = true;
        }
        printMenu();
      } else {
        Serial.println(F("[ERROR] Whut?"));
        printMenu();
      }
    }
  }
}

// task
void readButtons() {
  boolean sw1 = debounce(digitalRead(pinSW1) == LOW, &sw1db);
  boolean sw2 = debounce(digitalRead(pinSW2) == LOW, &sw2db);
  // TODO

  if(sw1) Serial.println(F("[DEBUG] Button 1 pressed - Not implemented"));
  if(sw2) Serial.println(F("[DEBUG] Button 2 pressed - Not implemented"));
  
}

boolean debounce(boolean value, boolean* store) {
  if(value) {
    if(*store) {
      value = false;
    } else {
      *store = true;
    }
  } else {
    *store = false;
  }
  return value;
}

void printMenu() {
  Serial.println(F(""));
  Serial.println(F("Menu"));
  Serial.println(F("----"));
  Serial.println(F("  1. Get time"));
  Serial.println(F("  2. Set time"));
  Serial.println(F("  3. Run slot machine effect"));
  Serial.println(F("  4. Toggle HV"));
  Serial.println(F(""));
}

void printTime() {
  DateTime now = RTC.now();
  Serial.print(F("Current time is:  "));
  if(now.hour() < 10) Serial.print(F("0"));
  Serial.print(now.hour(), DEC);
  Serial.print(F(":"));
  if(now.minute() < 10) Serial.print(F("0"));
  Serial.print(now.minute(), DEC);
  Serial.print(F(":"));
  if(now.second() < 10) Serial.print(F("0"));
  Serial.print(now.second(), DEC);
  Serial.println();
}
