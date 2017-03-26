#include "ButtonsTask.h"

// TODO Implement setting time through the buttons

ButtonsTask::ButtonsTask() {}

void ButtonsTask::task() {
  boolean sw1 = debounce(digitalRead(PIN_SW1) == LOW, &sw1db);
  boolean sw2 = debounce(digitalRead(PIN_SW2) == LOW, &sw2db);
  
  // TODO
  if(sw1) Serial.println(F("[DEBUG] Button 1 pressed - Not implemented"));
  if(sw2) Serial.println(F("[DEBUG] Button 2 pressed - Not implemented"));

}

void ButtonsTask::begin() {
  pinMode(PIN_SW1, INPUT);
  pinMode(PIN_SW2, INPUT);  
}

boolean ButtonsTask::debounce(boolean value, boolean* store) {
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
