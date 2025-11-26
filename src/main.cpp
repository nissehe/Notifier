#include <Arduino.h>
#include <FS.h>
#include <LittleFS.h>
#include <ESP8266WiFi.h>
#include "notificationStore.h"
#include "passwords.h"
#include "webserverCalendar.h"
#include "timeServer.h"
#include "scheduler.h"

const int BELL_PIN = 4;     // D2
const int WEB_MODE_PIN = 5; // D1

bool isWebMode;

void test()
{
  loadNotifications();
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
  loadNotifications();

  debugPrintAll("After save/load:");
}

void connectWiFi() 
{
  Serial.print("Connect to Wifi");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(300);
    digitalWrite(LED_BUILTIN, LOW);
    delay(300);
  }

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void disconnectWifi()
{
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  Serial.println("WiFi disconnected");
}

void ringTheBell()
{
  digitalWrite(BELL_PIN, HIGH);
  delay(500);
  digitalWrite(BELL_PIN, LOW);
}

void setup() {
  Serial.begin(115200);
  while(!Serial) { }
  Serial.println();

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BELL_PIN, OUTPUT);
  pinMode(WEB_MODE_PIN, INPUT);

  isWebMode = digitalRead(WEB_MODE_PIN) == HIGH;
  Serial.printf("Web mode: %s\n", isWebMode ? "ENABLED" : "DISABLED");

  if(!LittleFS.begin()){
    Serial.println("LittleFS Mount Failed");
    return;
  }

  loadNotifications();

  connectWiFi();

  syncRtc();

  if(isWebMode)
  {
    initWebServer();
  }
  else
  {
    disconnectWifi();
  }

  //test();
}

void loop() {
  if(!isWebMode){
    waitForNextNotification();
    ringTheBell();
  }
}