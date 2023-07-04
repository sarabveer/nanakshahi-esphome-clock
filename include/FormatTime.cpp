#include <esphome.h>
#include "FormatTime.h"

std::string FormatTime(esphome::time::ESPTime date, bool round, bool ampm, bool pad) {
  int minute = ( round && date.second >= 30 ) ? date.minute + 1 : date.minute;
  int hour = date.hour;
  if ( minute == 60 ) {
    minute = 0;
    hour += 1;
  }
  std::string period = "AM";
  if ( hour >= 12 ) {
    period = "PM";
    hour -= 12;
  }
  if ( hour == 0 ) {
      hour = 12;
  }
  std::string hourStr = ( pad && hour < 10 ) ? " " + esphome::to_string(hour) : esphome::to_string(hour);
  std::string minStr = minute < 10 ? "0" + esphome::to_string(minute) : esphome::to_string(minute);
  std::string time = hourStr + ":" + minStr;
  if (ampm) {
      time += " " + period;
  }
  return time;
}
