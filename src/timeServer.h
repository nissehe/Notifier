#pragma once

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

void syncRtc();
DayOfWeek getCurrentDayOfWeek();
int getCurrentTimeOfDayMinutes();