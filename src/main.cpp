#include <Arduino.h>
#include <FS.h>
#include <LittleFS.h>
#include <ESP8266WiFi.h>
#include "notificationStore.h"
#include "passwords.h"
#include "webserverCalendar.h"
#include "timeServer.h"
#include "scheduler.h"

const int BELL_PIN = 4;
const int WEB_MODE_PIN = 5;

const char* webModeFlagFile = "/webmode.flag";

bool isWebMode;

void test()
{
  // loadNotifications();
  // debugPrintAllNotifications("Initial notifications:");
  removeAllNotifications();
  saveNotifications();

  NotificationItem item1(DayOfWeek::Wednesday, 8, 30);
  NotificationItem item2(DayOfWeek::Monday, 14, 00);
  NotificationItem item3(DayOfWeek::Monday, 13, 50);
  addNotification(item1);
  int id2 = addNotification(item2);
  addNotification(item3);
  debugPrintAllNotifications("Added three items:");

  removeNotification(id2);
  debugPrintAllNotifications("Removed first item:");

  addNotification(NotificationItem(DayOfWeek::Tuesday, 9, 15));
  debugPrintAllNotifications("After adding another notification:");

  saveNotifications();
  loadNotifications();

  debugPrintAllNotifications("After save/load:");
}

void setWebModeFlag()
{
  File file = LittleFS.open(webModeFlagFile, "w");
  if (file) {
      file.write('1');
      file.close();
  }
}

void checkWebModeFlag()
{
  File file = LittleFS.open(webModeFlagFile, "r");
  if (file) {
    isWebMode = true;
    file.close();
    LittleFS.remove(webModeFlagFile);
    Serial.println("Web mode flag detected, starting in web mode.");
  }
}

void IRAM_ATTR ISR() {
    setWebModeFlag();
    Serial.println("Web mode flag set, restarting...");

    ESP.restart();
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

  if(!LittleFS.begin()){
    Serial.println("LittleFS Mount Failed");
    return;
  }

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BELL_PIN, OUTPUT);
  pinMode(WEB_MODE_PIN, INPUT_PULLUP);
  attachInterrupt(WEB_MODE_PIN, ISR, FALLING);

  loadNotifications();
  
  connectWiFi();
  
  syncRtc();

  checkWebModeFlag();

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