#include "test.h"
#include "notificationStore.h"
#include "notificationItem.h"

void generateTestData()
{
  removeAllNotifications();

  NotificationItem item1(DayOfWeek::Wednesday, 8, 30);
  NotificationItem item2(DayOfWeek::Monday, 14, 00);
  NotificationItem item3(DayOfWeek::Monday, 13, 50);
  addNotification(item1);
  addNotification(item2);
  addNotification(item3);
}

void test()
{
  // debugPrintAllNotifications("Initial notifications:");
  removeAllNotifications();

  NotificationItem item1(DayOfWeek::Wednesday, 8, 30);
  NotificationItem item2(DayOfWeek::Monday, 14, 00);
  NotificationItem item3(DayOfWeek::Monday, 13, 50);
  addNotification(item1);
  int id2 = addNotification(item2);
  addNotification(item3);
  debugPrintAllNotifications("Added three items:");

  removeNotification(id2);
  debugPrintAllNotifications("Removed first item:");

  addNotification(NotificationItem(DayOfWeek::Tuesday, 9, 15));
  debugPrintAllNotifications("After adding another notification:");
}