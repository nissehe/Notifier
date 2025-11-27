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

    NotificationItem(int day, int tod, int id)
        : dayOfWeek(static_cast<DayOfWeek>((day >= 0 && day <= 6) ? day : 0)),
          timeOfDayMinutes(tod),
          id(id)
    {}

    NotificationItem(String json)
    {
        Serial.println("Parsing notification JSON: " + json);

        // Parse the provided string as JSON and build a new NotificationItem.
        JsonDocument doc;
        DeserializationError err = deserializeJson(doc, json);
        if (err) {
            Serial.print("Failed to parse notification JSON: ");
            Serial.println(err.c_str());
            return;
        }

        int day = doc["dayOfWeek"] | 0;
        dayOfWeek = static_cast<DayOfWeek>((day >= 0 && day <= 6) ? day : 0);

        int tod = doc["timeOfDayMinutes"] | 0;
        timeOfDayMinutes = std::chrono::minutes(tod);

        id = doc["id"] | 0;
    }

    // Accessors
    int hours() const   { return static_cast<int>(timeOfDayMinutes.count() / 60); }
    int minutes() const { return static_cast<int>(timeOfDayMinutes.count() % 60); }

    // Simple textual representation: "Monday 08:30"
    std::string toString() const {
        static const char* names[] = {
            "Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"
        };
        std::ostringstream ss;
        ss << names[static_cast<int>(dayOfWeek)] << ' '
           << std::setfill('0') << std::setw(2) << hours() << ':'
           << std::setfill('0') << std::setw(2) << minutes();
        return ss.str();
    }
};
