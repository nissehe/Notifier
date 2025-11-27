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
          .modal {
            position: fixed;
            top: 0;
            left: 0;
            right: 0;
            bottom: 0;
            background: rgba(0,0,0,0.45);
            display: none;
            justify-content: center;
            align-items: center;
        }
        .modal-content {
            background: white;
            padding: 20px;
            width: 300px;
            border-radius: 8px;
            box-shadow: 0 4px 12px rgba(0,0,0,0.3);
        }
        .modal h2 {
            margin-top: 0;
        }
        input[type="time"], select {
            width: 100%;
            padding: 8px;
            margin-top: 8px;
            margin-bottom: 12px;
            border-radius: 4px;
            border: 1px solid #aaa;
        }
        .modal-buttons {
            display: flex;
            justify-content: flex-end;
        }
        .modal-buttons button {
            margin-left: 10px;
        }
      </style></head>
      <div id="editModal" class="modal">
        <div class="modal-content">
          <h2 id="modalTitle">Add Notification</h2>

          <label>Weekday:</label>
          <select id="weekday">
            <option>Monday</option>
            <option>Tuesday</option>
            <option>Wednesday</option>
            <option>Thursday</option>
            <option>Friday</option>
            <option>Saturday</option>
            <option>Sunday</option>
          </select>

          <label>Time:</label>
          <input type="time" id="timeInput" required>

          <div class="modal-buttons">
            <button onclick="closeModal()">Cancel</button>
            <button id="modalSaveBtn">Save</button>
          </div>
        </div>
      </div>
      <body>
      <h1>Scheduled Notifications</h1>
      <div class='add-box' onclick='openAddModal()'>+ Add New Notification</div>
      <div id="list">
      )rawliteral";

    auto notifications = getAllNotifications();

    for (size_t i = 0; i < notifications.size(); i++) {
        html += "<div class='item'>";
        html +=   "<div class='time'>";
        html +=      String(notifications[i].toString().c_str());
        html +=   "</div>";
        html +=   "<div>";
        html +=     "<button class='edit-btn' onclick='openEditModal(" + String(notifications[i].id) + ", `" + notifications[i].toString().c_str() + "`)'>Edit</button>";
        html +=     "<button class='remove-btn' onclick='removeItem(" + String(notifications[i].id) + ")'>Remove</button>";
        html += "</div></div>";
    }

    html += R"rawliteral(
      </div>
      <script>

        function removeItem(index) {
            if (confirm("Remove this notification?")) {
                window.location = "/remove?i=" + index;
            }
        }

        let editId = -1;

        function openAddModal() {
            editId = -1;

            document.getElementById("modalTitle").textContent = "Add Notification";
            document.getElementById("weekday").value = "Monday";
            document.getElementById("timeInput").value = "12:00";

            document.getElementById("modalSaveBtn").onclick = saveAdd;
            document.getElementById("editModal").style.display = "flex";
        }

        function openEditModal(id, existingValue) {
            editId = id;

            document.getElementById("modalTitle").textContent = "Edit Notification";

            // existingValue format: "Monday 13:50"
            const parts = existingValue.split(" ");
            document.getElementById("weekday").value = parts[0];
            document.getElementById("timeInput").value = parts[1];

            document.getElementById("modalSaveBtn").onclick = saveEdit;
            document.getElementById("editModal").style.display = "flex";
        }

        function closeModal() {
            document.getElementById("editModal").style.display = "none";
        }

        function saveAdd() {
            const weekday = document.getElementById("weekday").value;
            const time = document.getElementById("timeInput").value;

            const value = weekday + " " + time;
            window.location = "/add?value=" + encodeURIComponent(value);
        }

        function saveEdit() {
            const weekday = document.getElementById("weekday").value;
            const time = document.getElementById("timeInput").value;

            const value = weekday + " " + time;
            window.location = "/edit?i=" + editId + "&value=" + encodeURIComponent(value);
        }

      </script>
      </body></html>
      )rawliteral";

    request->send(200, "text/html", html);
  });

  server.begin();
}

