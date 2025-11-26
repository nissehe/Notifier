#include "webserverCalendar.h"
#include "notificationStore.h"

// Create the server instance
AsyncWebServer server(80);

void initWebServer() {
  
server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String page;

    page.reserve(1024);  // avoid fragmentation

    page += F("<!DOCTYPE html><html><head><meta charset=\"UTF-8\"><title>Notifications</title></head><body>");
    page += F("<h1>Scheduled Notifications</h1>");

    auto notifications = getAllNotifications();

    for (const auto& n : notifications) {
        page += "<div>";
        page += n.toString().c_str();
        page += "</div>\n";
    }

    page += F("</body></html>");

    request->send(200, "text/html", page);
  });

  server.begin();
}

