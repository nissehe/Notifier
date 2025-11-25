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

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  if(!LittleFS.begin()){
    Serial.println("LittleFS Mount Failed");
    return;
  }

  load();

  connectToWiFi();

  //test();
}

void loop() {
}