#include <Arduino.h>
#include "notificationStore.h"

const int LED_PIN = LED_BUILTIN; // built-in LED might be defined for your board

void Test()
{
  NotificationItem item1(NotificationItem::DayOfWeek::Monday, 8, 30);
  NotificationItem item2(NotificationItem::DayOfWeek::Wednesday, 14, 00);
  add(item1);
  add(item2);

  const auto& notifications = getAll();
  for (const auto& n : notifications) {
    Serial.print("Notification ID: ");
    Serial.print(n.id);
    Serial.print(", Time: ");
    Serial.println(n.toString().c_str());
  }

  remove(0); // Remove the first notification
  Serial.println("After removal:");
  const auto& updatedNotifications = getAll();
  for (const auto& n : updatedNotifications) {
    Serial.print("Notification ID: ");
    Serial.print(n.id);
    Serial.print(", Time: ");
    Serial.println(n.toString().c_str());
  }

  add(NotificationItem(NotificationItem::DayOfWeek::Friday, 9, 15));
  Serial.println("After adding another notification:");
  const auto& finalNotifications = getAll();
  for (const auto& n : finalNotifications) {
    Serial.print("Notification ID: ");
    Serial.print(n.id);
    Serial.print(", Time: ");
    Serial.println(n.toString().c_str());
  }
}


void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  Serial.println("LED ON");

  load();

  Test();
}

void loop() {
}