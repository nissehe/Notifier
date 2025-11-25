#include "notificationStore.h"
#include <vector>
#include <FS.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

static std::vector<NotificationItem> g_notifications;
const char* const path = "/notifications.json";

static int nextId()
{
    int id = 0;
    bool exists;
    do {
        exists = false;
        for (const auto &n : g_notifications) {
            if (n.id == id) { exists = true; ++id; break; }
        }
    } while (exists);
    return id;
}

void load()
{
    File file = LittleFS.open(path, "r");

    if (!file) {
        Serial.println("Failed to open file for reading");
        return;
    }

    size_t size = file.size();
    if (size == 0) {
        file.close();
        return;
    }

    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, file);
    if (err) {
        Serial.print("Failed to parse JSON: ");
        Serial.println(err.c_str());
        file.close();
        return;
    }
    
    wipeAll();

    for (JsonObject obj : doc["data"].as<JsonArray>()) {
        NotificationItem item(obj["dayOfWeek"] | 0, obj["timeOfDayMinutes"] | 0, obj["id"] | 0);
        g_notifications.push_back(std::move(item));
    }

    file.close();
}

void save()
{
    File file = LittleFS.open(path, "w");
    if (!file) {
        Serial.println("Failed to open file for writing");
        return;
    }

    JsonDocument doc;
    JsonArray data = doc["data"].to<JsonArray>();

    for (const auto &n : g_notifications) {
        JsonObject obj = data.add<JsonObject>();
        obj["id"] = n.id;
        obj["dayOfWeek"] = (int)n.dayOfWeek;
        obj["timeOfDayMinutes"] = n.timeOfDayMinutes.count();
    }

    doc.shrinkToFit();

    String json;
    serializeJson(doc, json);

    if (!file.print(json)) {
        Serial.println("Write failed");
    }

    file.close();
}

void add(NotificationItem item)
{
    item.id = nextId();
    g_notifications.push_back(std::move(item));
}

void remove(int id)
{
    for (auto it = g_notifications.begin(); it != g_notifications.end(); ++it) {
        if (it->id == id) {
            g_notifications.erase(it);
            break;
        }
    }
}

const std::vector<NotificationItem> &getAll() noexcept
{
    return g_notifications;
}

void wipeAll()
{
    g_notifications.clear();
}

void debugPrintAll(const char* header)
{
    Serial.println(header);
    const auto& notifications = getAll();
    for (const auto& n : notifications) {
        Serial.print("Notification ID: ");
        Serial.print(n.id);
        Serial.print(", Time: ");
        Serial.println(n.toString().c_str());
    }
}
