#include "TimeTask.h"

TimeTask::TimeTask(NixieDisplay* nixie, Chronodot* rtc) {
  _nixie = nixie;
  _rtc = rtc;
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

  // Enable and disable the right segments
  _nixie->disableSegments(hourTens, 10);
  _nixie->disableSegments(hourUnits, 10);
  _nixie->disableSegments(minuteTens, 10);
  _nixie->disableSegments(minuteUnits, 10);
  _nixie->disableSegments(secondTens, 10);
  _nixie->disableSegments(secondUnits, 10);
  int hour = now.hour();
  _nixie->enableSegment(hourTens[(hour / 10) % 10]);
  _nixie->enableSegment(hourUnits[hour % 10]);
  int minute = now.minute();
  _nixie->enableSegment(minuteTens[(minute / 10) % 10]);
  _nixie->enableSegment(minuteUnits[minute % 10]);
  int second = now.second();
  _nixie->enableSegment(secondTens[(second / 10) % 10]);
  _nixie->enableSegment(secondUnits[second % 10]);

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
  if (now.minute() == 0 && now.second() == 0) {
    _nixie->runSlotMachine();
  }
  
}
