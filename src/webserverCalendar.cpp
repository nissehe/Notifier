#include "webserverCalendar.h"
#include "notificationStore.h"

// Create the server instance
AsyncWebServer server(80);

void initWebServer() {

  server.on("/add", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("value")) {
        String v = request->getParam("value")->value();
        NotificationItem newItem(v.c_str());
        addNotification(newItem);
    }
    else
    {
      Serial.println("Edit request missing parameter");
      Serial.println(request->url());
    }

    request->redirect("/");
  });

  server.on("/edit", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("i") && request->hasParam("value")) {
        int id = request->getParam("i")->value().toInt();
        String v = request->getParam("value")->value();
        NotificationItem updateItem(v.c_str());
        updateItem.id = id;
        updateNotification(updateItem);
    }
    else
    {
      Serial.println("Edit request missing parameter(s)");
      Serial.println(request->url());
    }

    request->redirect("/");
  });

  server.on("/remove", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("i")) {
        int id = request->getParam("i")->value().toInt();
        removeNotification(id);  // your function
    }
    else
    {
      Serial.println("Remove request missing index parameter");
      Serial.println(request->url());
    }
    
    request->redirect("/");
  });

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String html;

    html.reserve(1024);  // avoid fragmentation

    html = R"rawliteral(
      <!DOCTYPE html><html><head>
      <meta charset="UTF-8">
      <title>Notifications</title>
      <style>
        body {
              font-family: Arial, sans-serif;
              background: #f3f3f3;
              margin: 0;
              padding: 20px;
          }
          h1 {
              margin-bottom: 20px;
          }
          .item {
              background: white;
              padding: 12px;
              margin-bottom: 10px;
              border-radius: 6px;
              display: flex;
              justify-content: space-between;
              align-items: center;
              box-shadow: 0 2px 5px rgba(0,0,0,0.15);
          }
          .time {
              font-size: 18px;
          }
          button {
              padding: 6px 10px;
              border: none;
              border-radius: 4px;
              cursor: pointer;
              margin-left: 8px;
          }
          .edit-btn {
              background: #2196F3;
              color: white;
          }
          .remove-btn {
              background: #E53935;
              color: white;
          }
          .add-box {
              background: #dfeffd;
              padding: 12px;
              margin-bottom: 18px;
              border-radius: 6px;
              display: flex;
              justify-content: center;
              align-items: center;
              box-shadow: 0 2px 5px rgba(0,0,0,0.15);
              cursor: pointer;
              font-size: 18px;
              border: 2px dashed #2196F3;
              color: #0a3d75;
              font-weight: bold;
          }
          .add-box:hover {
              background: #c8e0ff;
          }
      </style>
      </head><body>
      <h1>Scheduled Notifications</h1>
      <div class='add-box' onclick='addItem()'>+ Add New Notification</div>
      <div id="list">
      )rawliteral";

    auto notifications = getAllNotifications();

    for (size_t i = 0; i < notifications.size(); i++) {
        html += "<div class='item'>";
        html +=   "<div class='time'>";
        html +=      String(notifications[i].toString().c_str());
        html +=   "</div>";
        html +=   "<div>";
        html +=     "<button class='edit-btn' onclick='editItem(" + String(notifications[i].id) + ")'>Edit</button>";
        html +=     "<button class='remove-btn' onclick='removeItem(" + String(notifications[i].id) + ")'>Remove</button>";
        html += "</div></div>";
    }

    html += R"rawliteral(
      </div>
      <script>
        function editItem(index) {
            const newValue = prompt("Enter new time (e.g. Monday 14:00):");
            if (newValue) {
                window.location = "/edit?i=" + index + "&value=" + encodeURIComponent(newValue);
            }
        }

        function removeItem(index) {
            if (confirm("Remove this notification?")) {
                window.location = "/remove?i=" + index;
            }
        }

        function addItem() {
            const newValue = prompt("Enter new time (e.g. Monday 14:00):");
            if (newValue) {
                window.location = "/add?value=" + encodeURIComponent(newValue);
            }
        }
      </script>
      </body></html>
      )rawliteral";

    request->send(200, "text/html", html);
  });

  server.begin();
}

