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

void connectWiFi() 
{
  IPAddress local_IP(192, 168, 0, 202);
  IPAddress gateway(192, 168, 0, 1);
  IPAddress subnet(255, 255, 255, 0);
  IPAddress dns1(192, 168, 0, 1);
  
  Serial.print("Connect to Wifi using static IP: ");
  Serial.println(local_IP);

  WiFi.mode(WIFI_STA);

  if (!WiFi.config(local_IP, gateway, subnet, dns1)) {
    Serial.println("Failed to configure static IP");
  }

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

void ringTheBell()
{
  Serial.println("Ring the bell!");
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

  loadNotifications();
  connectWiFi();
  syncRtc();
  initWebServer();
}

void loop() {
  waitForNextNotification();
  ringTheBell();
}