#include "timeServer.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "passwords.h"

String dstUrl = "http://api.timezonedb.com/v2.1/get-time-zone?key=" +
                String(TIMEZONEDB_API_KEY) +
                "&format=json&by=zone&zone=Europe/Stockholm";

bool isDst()
{
    bool isDst = false;

    WiFiClient client;
    HTTPClient http;
    http.begin(client, dstUrl);

    int httpCode = http.GET();

    if (httpCode == 200) {

      String response = http.getString();

      JsonDocument filter;
      filter["dst"] = true;

      JsonDocument doc;

      DeserializationError error = deserializeJson(doc, response, DeserializationOption::Filter(filter));
      if (!error) {
        int dstFlag = doc["dst"];
        isDst = dstFlag > 0;
        Serial.printf("DST: %s\n", isDst ? "true" : "false");
      } else {
        Serial.print("JSON parse error! ");
        Serial.println(error.c_str());
      }
    } else {
      Serial.printf("HTTP error: %d\n", httpCode);
    }

    http.end();

    return isDst;
  }

  DayOfWeek getDayOfWeekFromString(const String &dayStr)
  {
      if (dayStr == "Monday")    return DayOfWeek::Monday;
      if (dayStr == "Tuesday")   return DayOfWeek::Tuesday; 
      if (dayStr == "Wednesday") return DayOfWeek::Wednesday;
      if (dayStr == "Thursday")  return DayOfWeek::Thursday;    
      if (dayStr == "Friday")    return DayOfWeek::Friday;    
      if (dayStr == "Saturday")  return DayOfWeek::Saturday;    
      if (dayStr == "Sunday")    return DayOfWeek::Sunday;
      return DayOfWeek::Monday; // Default
  }

  String getStringFromDayOfWeek(DayOfWeek day)
  {
      switch (day) {
          case DayOfWeek::Monday:    return "Monday";
          case DayOfWeek::Tuesday:   return "Tuesday";
          case DayOfWeek::Wednesday: return "Wednesday";
          case DayOfWeek::Thursday:  return "Thursday";
          case DayOfWeek::Friday:    return "Friday";
          case DayOfWeek::Saturday:  return "Saturday";
          case DayOfWeek::Sunday:    return "Sunday";
          default:                   return "Monday";
      }
  }

  void syncRtc()
  {
      // Implement RTC synchronization if needed
      if (WiFi.status() != WL_CONNECTED)
      {
          Serial.println("WiFi not connected, skipping RTC sync");
          return;
      }

      // Configure NTP - adjust gmtOffset_sec and daylightOffset_sec to your timezone
      const char *ntpServer = "pool.ntp.org";
      const long gmtOffset_sec = 3600; // e.g. 3600 for CET (UTC+1)
      const int daylightOffset_sec = isDst() ? 3600 : 0;

      configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

      Serial.print("Syncing time with NTP");
      time_t now = time(nullptr);
      int attempts = 0;
      const int maxAttempts = 30; // ~15 seconds (30 * 500ms)
      while (now < 24 * 3600 && attempts++ < maxAttempts)
      {
          digitalWrite(LED_BUILTIN, HIGH);
          delay(600);
          digitalWrite(LED_BUILTIN, LOW);
          delay(600);
          now = time(nullptr);
      }
      Serial.println();

      if (now < 24 * 3600)
      {
          Serial.println("Failed to synchronize time");
          return;
      }

      struct tm timeinfo;
      gmtime_r(&now, &timeinfo);
      char buf[64];
      strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeinfo);
      Serial.print("Time synchronized UTC:   ");
      Serial.println(buf);

      localtime_r(&now, &timeinfo);
      strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeinfo);
      Serial.print("Time synchronized Local: ");
      Serial.println(buf);
  }

DayOfWeek getCurrentDayOfWeek()
{
    time_t now = time(nullptr);
    struct tm timeinfo;
    localtime_r(&now, &timeinfo);
    int wday = timeinfo.tm_wday; // tm_wday: 0=Sunday, 1=Monday, ..., 6=Saturday
    if (wday == 0) {
        return DayOfWeek::Sunday;
    } else {
        return static_cast<DayOfWeek>(wday - 1);
    }
}

int getCurrentTimeOfDayMinutes()
{
    time_t now = time(nullptr);
    struct tm timeinfo;
    localtime_r(&now, &timeinfo);
    return timeinfo.tm_hour * 60 + timeinfo.tm_min;
}
