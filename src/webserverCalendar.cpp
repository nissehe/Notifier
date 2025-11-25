#include "webserverCalendar.h"

// Create the server instance
AsyncWebServer server(80);

// Simple HTML page
static const char webpage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>ESP8266 Async Server</title>
</head>
<body>
  <h1>Hello from ESP8266 (AsyncWebServer)!</h1>
</body>
</html>
)rawliteral";

void initWebServer() {
  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", webpage);
  });

  server.begin();
}
