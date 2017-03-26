// Nixie clock firmware
// ====================
// December 2016 - March 2017
// by Wouter Devinck

// Dependencies:
//  * Chronodot library (for DS3231)   - https://github.com/Stephanie-Maks/Arduino-Chronodot

#include "TaskScheduler.h"
#include "NixieDisplay.h"
#include "TimeTask.h"
#include "MenuTask.h"
#include "ButtonsTask.h"
#include <Wire.h>

Chronodot rtc;
TaskScheduler sched;
NixieDisplay nixie;
HvSupply hvsupply;
TimeTask timeTask(&nixie, &rtc);
MenuTask menuTask(&nixie, &rtc, &hvsupply);
ButtonsTask buttonsTask;

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
  timeTask.begin();
  
  // Initiate the button inputs
  Serial.println(F("[INFO] 2. Init buttons"));
  buttonsTask.begin();

  // Initiate the HV5622 outputs
  Serial.println(F("[INFO] 3. Init HV shift registers"));
  nixie.begin();

  // Initiate the DC/DC enable pin
  Serial.println(F("[INFO] 4. Init DC/DC"));
  hvsupply.begin();

  // Initiate the OS tasks
  Serial.println(F("[INFO] 5. Init RTOS tasks"));
  sched.addTask(execMenuTask, 100);
  sched.addTask(execButtonsTask, 100);
  sched.addTask(execTimeTask, 1000);
  
  // Debug info
  Serial.println(F("[INFO] Clock done booting."));
  menuTask.printMenu();
  
}

void loop() {
  sched.loop();
}

void execTimeTask() {
  timeTask.task();
}

void execButtonsTask() {
  buttonsTask.task();
}

void execMenuTask() {
  menuTask.task();
}
