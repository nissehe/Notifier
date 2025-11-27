#include <Arduino.h>
#include "scheduler.h"
#include "notificationStore.h"
#include "timeServer.h"

int usedCalendarRevision = -1;

NotificationItem* getNextNotification()
{
    const auto& notifications = getAllNotifications();
    if (notifications.empty()) {
        return nullptr;
    }

    DayOfWeek currentDay = getCurrentDayOfWeek();
    int currentMinutes = getCurrentTimeOfDayMinutes();

    for (const auto& n : notifications) {
        if (n.dayOfWeek > currentDay ||
            (n.dayOfWeek == currentDay && n.timeOfDayMinutes.count() > currentMinutes)) {
            return const_cast<NotificationItem*>(&n);
        }
    }

    // If no upcoming notification found, return the first one (next week)
    return const_cast<NotificationItem*>(&notifications.front());
}

void waitForNextNotification()
{
    NotificationItem* next = getNextNotification();
    if (!next) {
        Serial.println("No notifications scheduled.");
        return;
    }

    usedCalendarRevision = calendarRevision;

    int currentMinutes = getCurrentTimeOfDayMinutes();
    int targetMinutes = static_cast<int>(next->timeOfDayMinutes.count());
    int daysUntil = (static_cast<int>(next->dayOfWeek) - static_cast<int>(getCurrentDayOfWeek()) + 7) % 7;

    int totalWaitMinutes = daysUntil * 1440 + (targetMinutes - currentMinutes);
    if (totalWaitMinutes <= 0) {
        totalWaitMinutes += 7 * 1440; // Schedule for next week
    }

    Serial.printf("Next notification (ID: %d) in %d minutes (%s)\n",
                  next->id, totalWaitMinutes, next->toString().c_str());

    while (totalWaitMinutes > 0) {
        delay(60000); // Wait for one minute
        --totalWaitMinutes;

        if(usedCalendarRevision != calendarRevision) {
            Serial.println("Calendar changed, recalculating next notification.");
            waitForNextNotification();
            return;
        }
    }
}