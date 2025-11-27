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

    NotificationItem(DayOfWeek dayOfWeek, int hours, int minutes = 0)
        : dayOfWeek(dayOfWeek),
          timeOfDayMinutes(std::chrono::hours(hours) + std::chrono::minutes(minutes))
    {}

    NotificationItem(String dayOfWeekString, int hours, int minutes = 0)
        : dayOfWeek(getDayOfWeekFromString(dayOfWeekString)),
          timeOfDayMinutes(std::chrono::hours(hours) + std::chrono::minutes(minutes))
    {}

    NotificationItem(int dayOfWeekInt, int tod, int id)
        : dayOfWeek(static_cast<DayOfWeek>((dayOfWeekInt >= 0 && dayOfWeekInt <= 6) ? dayOfWeekInt : 0)),
          timeOfDayMinutes(tod),
          id(id)
    {}

    NotificationItem(JsonObject obj)
        : dayOfWeek(static_cast<DayOfWeek>(obj["dayOfWeek"] | 0)),
          timeOfDayMinutes(obj["timeOfDayMinutes"] | 0),
          id(obj["id"] | 0)
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

    void addAsJson(JsonObject obj) const {
        obj["id"] = id;
        obj["dayOfWeek"] = static_cast<int>(dayOfWeek);
        obj["timeOfDayMinutes"] = timeOfDayMinutes.count();
    }
};
