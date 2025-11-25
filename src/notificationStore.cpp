#include "notificationStore.h"
#include <vector>

static std::vector<NotificationItem> g_notifications;

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
    // TODO: implement loading from persistent storage
}

void save()
{
    // TODO: implement saving to persistent storage
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
