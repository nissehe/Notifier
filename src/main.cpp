#include <Arduino.h>
#include <FS.h>
#include <LittleFS.h>
#include <ESP8266WiFi.h>
#include "notificationStore.h"
#include "passwords.h"

const int LED_PIN = LED_BUILTIN; // built-in LED might be defined for your board

void test()
{
  load();
  debugPrintAll("Initial notifications:");

  NotificationItem item1(NotificationItem::DayOfWeek::Monday, 8, 30);
  NotificationItem item2(NotificationItem::DayOfWeek::Wednesday, 14, 00);
  add(item1);
  add(item2);
  debugPrintAll("Added two items:");

  remove(0); // Remove the first notification
  debugPrintAll("Removed first item:");

  add(NotificationItem(NotificationItem::DayOfWeek::Friday, 9, 15));
  debugPrintAll("After adding another notification:");

  save();
  load();

  debugPrintAll("After save/load:");
}

void connectToWiFi() {

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(LED_PIN, HIGH);
    delay(300);
    digitalWrite(LED_PIN, LOW);
    delay(300);
  }

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void syncRtc() 
{
  // Implement RTC synchronization if needed
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected, skipping RTC sync");
    return;
  }

  // Configure NTP - adjust gmtOffset_sec and daylightOffset_sec to your timezone
  const char* ntpServer = "pool.ntp.org";
  const long gmtOffset_sec = 3600; // e.g. 3600 for CET (UTC+1)
  const int daylightOffset_sec = 0;   // e.g. 3600 when DST applies

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  Serial.print("Syncing time with NTP");
  time_t now = time(nullptr);
  int attempts = 0;
  const int maxAttempts = 30; // ~15 seconds (30 * 500ms)
  while (now < 24 * 3600 && attempts++ < maxAttempts) {
    digitalWrite(LED_PIN, HIGH);
    delay(600);
    digitalWrite(LED_PIN, LOW);
    delay(600);
    now = time(nullptr);
  }
  Serial.println();

  if (now < 24 * 3600) {
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

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  if(!LittleFS.begin()){
    Serial.println("LittleFS Mount Failed");
    return;
  }

  load();

  connectToWiFi();

  syncRtc();

  //test();
}

void loop() {
}