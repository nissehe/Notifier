#include <Arduino.h>
#include <FS.h>
#include <LittleFS.h>
#include <ESP8266WiFi.h>
#include "notificationStore.h"
#include "passwords.h"
#include "webserverCalendar.h"
#include "timeServer.h"
#include "scheduler.h"
#include "test.h"

const int BELL_PIN = 4;
const int WEB_MODE_PIN = 5;

const char* webModeFlagFile = "/webmode.flag";

bool isWebMode;

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

  delay(100); // Avoid any bounce from the web button

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BELL_PIN, OUTPUT);
  pinMode(WEB_MODE_PIN, INPUT_PULLUP);
  attachInterrupt(WEB_MODE_PIN, ISR, FALLING);

  loadNotifications();
  
  connectWiFi();
  
  syncRtc();

  checkWebModeFlag();

  isWebMode = true;
  if(isWebMode)
  {
    initWebServer();
  }
  else
  {
    disconnectWifi();
  }

  // test();
}

void loop() {
  if(!isWebMode){
    waitForNextNotification();
    ringTheBell();
  }
}