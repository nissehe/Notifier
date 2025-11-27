#pragma once

#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>
#include <ArduinoJson.h>
#include "timeServer.h"

struct NotificationItem {

    DayOfWeek dayOfWeek{DayOfWeek::Monday};

    std::chrono::minutes timeOfDayMinutes{0};

    int id;

    NotificationItem() = default;

    NotificationItem(DayOfWeek d, int hours, int minutes = 0)
        : dayOfWeek(d),
          timeOfDayMinutes(std::chrono::hours(hours) + std::chrono::minutes(minutes))
    {}

    NotificationItem(String d, int hours, int minutes = 0)
        : dayOfWeek(getDayOfWeekFromString(d)),
          timeOfDayMinutes(std::chrono::hours(hours) + std::chrono::minutes(minutes))
    {}

    NotificationItem(int day, int tod, int id)
        : dayOfWeek(static_cast<DayOfWeek>((day >= 0 && day <= 6) ? day : 0)),
          timeOfDayMinutes(tod),
          id(id)
    {}

    // Accessors
    int hours() const   { return static_cast<int>(timeOfDayMinutes.count() / 60); }
    int minutes() const { return static_cast<int>(timeOfDayMinutes.count() % 60); }

    // Simple textual representation: "Monday 08:30"
    std::string toString() const {
        std::ostringstream ss;
        ss << getStringFromDayOfWeek(dayOfWeek).c_str() << ' '
           << std::setfill('0') << std::setw(2) << hours() << ':'
           << std::setfill('0') << std::setw(2) << minutes();
        return ss.str();
    }
};
