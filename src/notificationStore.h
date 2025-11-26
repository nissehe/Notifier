#pragma once

#include <vector>
#include "NotificationItem.h"

void loadNotifications();
void save();
void add(NotificationItem item);
void remove(int index);
const std::vector<NotificationItem>& getAll() noexcept;
void wipeAll();
void debugPrintAll(const char* header);