#include <Arduino.h>
#include <FS.h>
#include <LittleFS.h>
#include "notificationStore.h"

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

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  Serial.println("LED ON");

  if(!LittleFS.begin()){
    Serial.println("LittleFS Mount Failed");
    return;
  }

  // load();

  test();
}

void loop() {
}