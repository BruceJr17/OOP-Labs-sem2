#pragma once
#include "my_string.h"
#include <stdexcept>

enum NotificationType { NOTIF_SYSTEM, NOTIF_MESSAGE, NOTIF_APP };
enum SystemSeverity   { SEV_NORMAL, SEV_URGENT };

struct SystemData  { char message[128]; SystemSeverity severity; };
struct MessageData { char contact[64];  char text[128]; };
struct AppData     { char app_name[64]; char title[64]; char text[128]; };

struct Notification {
    long             timestamp;
    NotificationType type;
    union {
        SystemData  system;
        MessageData message;
        AppData     app;
    };
};

Notification make_system_notification(long ts, const char* msg, SystemSeverity sev);
Notification make_message_notification(long ts, const char* contact, const char* text);
Notification make_app_notification(long ts, const char* app, const char* title, const char* text);
void print_notification(const Notification& n);
int  count_by_type(const Notification* arr, int size, NotificationType type);

class NotificationQueue {
protected:
    struct Node {
        Notification data;
        Node* next;
        Node(const Notification& d, Node* n) : data(d), next(n) {}
    };
    Node* head;
    Node* tail;
    int   count;
public:
    NotificationQueue();
    virtual ~NotificationQueue();
    void             push(const Notification& n);
    virtual Notification pop();
    int              size() const { return count; }
    struct Iterator {
        Node* node;
        Iterator(Node* n) : node(n) {}
        Notification&  operator*()  { return node->data; }
        Notification*  operator->() { return &node->data; }
        Iterator& operator++() { node = node->next; return *this; }
        bool operator!=(const Iterator& o) const { return node != o.node; }
    };
    Iterator begin() { return Iterator(head); }
    Iterator end()   { return Iterator(nullptr); }
};

class NotificationPriorityQueue : public NotificationQueue {
private:
    static int type_rank(const Notification& n);
public:
    NotificationPriorityQueue() {}
    Notification pop() override;
};
