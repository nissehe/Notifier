#pragma once

#include <Arduino.h>
#include <cstdint>

enum class DayOfWeek : uint8_t {
    Monday = 0,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday,
    Sunday
};

DayOfWeek getDayOfWeekFromString(const String& dayStr);
String getStringFromDayOfWeek(DayOfWeek day);

void syncRtc();
DayOfWeek getCurrentDayOfWeek();
int getCurrentTimeOfDayMinutes();