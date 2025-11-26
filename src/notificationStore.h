#pragma once

#include <vector>
#include "NotificationItem.h"

void loadNotifications();
void saveNotifications();
int addNotification(NotificationItem item);
void removeNotification(int id);
const std::vector<NotificationItem>& getAllNotifications() noexcept;
void removeAllNotifications();
void debugPrintAllNotifications(const char* header);