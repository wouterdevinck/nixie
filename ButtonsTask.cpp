#include "ButtonsTask.h"

ButtonsTask::ButtonsTask(Chronodot* rtc, TimeTask* buddy) {
  _rtc = rtc;
  _buddy = buddy;
  _state = Run;
}

void ButtonsTask::begin() {
  pinMode(PIN_SW1, INPUT);
  pinMode(PIN_SW2, INPUT);  
}

void ButtonsTask::task() {
  boolean sw1 = debounce(digitalRead(PIN_SW1) == LOW, &sw1db);
  boolean sw2 = debounce(digitalRead(PIN_SW2) == LOW, &sw2db);
  
  if(sw1) {
    if(_state == Run) {
      _state = SetHour;
    } else if (_state == SetHour) {
      _state = SetMinute;
    } else if (_state == SetMinute) {
      _state = SetSecond;
    } else if (_state == SetSecond) {
      _state = Run;
    }
    _buddy->setState(_state);
  }
  
  if(sw2 && _state != Run) {
    DateTime now = _rtc->now();
    if (_state == SetHour) {
      int hour = now.hour();
      if(hour == 23) {
        hour = 0;
      } else {
        hour++;
      }
      _rtc->adjust(DateTime(now.year(), now.month(), now.day(), hour, now.minute(), now.second()));
    } else if (_state == SetMinute) {
      int minute = now.minute();
      if(minute == 59) {
        minute = 0;
      } else {
        minute++;
      }
      _rtc->adjust(DateTime(now.year(), now.month(), now.day(), now.hour(), minute, now.second()));
    } else if (_state == SetSecond) {
      int second = now.second();
      if(second == 59) {
        second = 0;
      } else {
        second++;
      }
      _rtc->adjust(DateTime(now.year(), now.month(), now.day(), now.hour(), now.minute(), second));
    }
    _buddy->task();
  }

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
