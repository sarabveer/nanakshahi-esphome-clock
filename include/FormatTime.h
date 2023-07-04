#ifndef FormatTime_h
#define FormatTime_h

#include <esphome.h>

std::string FormatTime(esphome::time::ESPTime date, bool round = true, bool ampm = true, bool pad = false);

#endif
