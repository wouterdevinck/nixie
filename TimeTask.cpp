#include "TimeTask.h"

TimeTask::TimeTask(NixieDisplay* nixie, Chronodot* rtc, Settings* settings) {
  _nixie = nixie;
  _rtc = rtc;
  _settings = settings;
}

void TimeTask::begin() {
  Wire.begin();
  _rtc->begin();
  //if (! rtc.isrunning()) {
  //  Serial.println(F("[WARNING] RTC is NOT running!"));
  //  _rtc->adjust(DateTime(__DATE__, __TIME__));
  //}
}

void TimeTask::task() {
  
  // Get current time from DS3231 RTC chip
  DateTime now = _rtc->now();
  int hour = now.hour();
  int minute = now.minute();
  int second = now.second();

  // DST auto-adjust for Europe
  if(_settings->getEuropeDstEnabled()) {
    if(now.dayOfWeek() == 0 && now.month() == 10 && now.day() >= 25 && 
      hour == 3 && _settings->getEuropeSummerTime()) {

      // It's 3 am on the last Sunday of October and summertime is still in effect
      _rtc->adjust(DateTime(now.year(), now.month(), now.day(), 2, minute, second)); // 3 am -> 2 am
      _settings->setEuropeSummerTime(false); // Remember that winter time is now in effect
      
    } else if (now.dayOfWeek() == 0 && now.month() == 3 && now.day() >= 25 && 
      hour == 2 && !_settings->getEuropeSummerTime()) {

      // It's 2 am on the last Sunday of March and wintertime is still in effect
      _rtc->adjust(DateTime(now.year(), now.month(), now.day(), 3, minute, second)); // 2 am -> 3 am
      _settings->setEuropeSummerTime(true); // Remember that summer time is now in effect
    
    }
  }

  // Enable and disable the right segments
  _nixie->disableSegments(hourTens, 10);
  _nixie->disableSegments(hourUnits, 10);
  _nixie->disableSegments(minuteTens, 10);
  _nixie->disableSegments(minuteUnits, 10);
  _nixie->disableSegments(secondTens, 10);
  _nixie->disableSegments(secondUnits, 10);
  if(!(_state == SetHour && second % 2 == 0)) {
    _nixie->enableSegment(hourTens[(hour / 10) % 10]);
    _nixie->enableSegment(hourUnits[hour % 10]);
  }
  if(!(_state == SetMinute && second % 2 == 0)) {
    _nixie->enableSegment(minuteTens[(minute / 10) % 10]);
    _nixie->enableSegment(minuteUnits[minute % 10]);
  }
  if(!(_state == SetSecond && second % 2 == 0)) {
    _nixie->enableSegment(secondTens[(second / 10) % 10]);
    _nixie->enableSegment(secondUnits[second % 10]);
  }

  // Flash the dots once per second
  if (second % 2 == 0) {
    _nixie->disableSegment(leftDot);
    _nixie->disableSegment(rightDot);
  } else {
    _nixie->enableSegment(leftDot);
    _nixie->enableSegment(rightDot);
  }

  // Write to display
  _nixie->updateDisplay();

  // Once an hour, run the slot machine effect to prevent cathode poisoning
  if (minute == 0 && second == 0) {
    _nixie->runSlotMachine();
  }
  
}

void TimeTask::setState(State state) {
  _state = state;
}
