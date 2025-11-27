#pragma once

#include <Arduino.h>
#include <vector>
#include "NotificationItem.h"

extern int calendarRevision;

void loadNotifications();
int addNotification(NotificationItem item);
void updateNotification(NotificationItem item);
void removeNotification(int id);
void removeAllNotifications();
const std::vector<NotificationItem>& getAllNotifications() noexcept;
void debugPrintAllNotifications(const char* header);