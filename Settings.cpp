#include "Settings.h"

Settings::Settings() {}

void Settings::begin() {
  _euDstEnabled = EEPROM.read(0);
  _euSummertime = EEPROM.read(1);
}

bool Settings::getEuropeDstEnabled() {
  return _euDstEnabled;
}

void Settings::setEuropeDstEnabled(bool value) {
  if(_euDstEnabled != value) { 
    _euDstEnabled = value;
    EEPROM.write(0, value);
  }
}

bool Settings::getEuropeSummerTime() {
  return _euSummertime;
}

void Settings::setEuropeSummerTime(bool value) {
  if(_euSummertime != value) { 
    _euSummertime = value;
    EEPROM.write(1, value);
  }
}
