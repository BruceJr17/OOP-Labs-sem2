#include "notification.h"
#include <cstring>
#include <iostream>
#include <stdexcept>

Notification make_system_notification(long ts, const char* msg, SystemSeverity sev) {
    Notification n;
    n.timestamp = ts; n.type = NOTIF_SYSTEM;
    strncpy(n.system.message, msg, 127); n.system.message[127] = '\0';
    n.system.severity = sev;
    return n;
}

Notification make_message_notification(long ts, const char* contact, const char* text) {
    Notification n;
    n.timestamp = ts; n.type = NOTIF_MESSAGE;
    strncpy(n.message.contact, contact, 63); n.message.contact[63] = '\0';
    strncpy(n.message.text,    text,    127); n.message.text[127]   = '\0';
    return n;
}

Notification make_app_notification(long ts, const char* app,
                                    const char* title, const char* text) {
    Notification n;
    n.timestamp = ts; n.type = NOTIF_APP;
    strncpy(n.app.app_name, app,   63); n.app.app_name[63] = '\0';
    strncpy(n.app.title,    title, 63); n.app.title[63]    = '\0';
    strncpy(n.app.text,     text,  127); n.app.text[127]   = '\0';
    return n;
}

void print_notification(const Notification& n) {
    std::cout << "[t=" << n.timestamp << "] ";
    switch (n.type) {
        case NOTIF_SYSTEM:
            std::cout << (n.system.severity == SEV_URGENT ? "URGENT " : "")
                      << "SYSTEM: " << n.system.message << "\n"; break;
        case NOTIF_MESSAGE:
            std::cout << "MSG from " << n.message.contact
                      << ": " << n.message.text << "\n"; break;
        case NOTIF_APP:
            std::cout << "APP [" << n.app.app_name << "] "
                      << n.app.title << ": " << n.app.text << "\n"; break;
    }
}

int count_by_type(const Notification* arr, int size, NotificationType type) {
    int c = 0;
    for (int i = 0; i < size; i++) if (arr[i].type == type) c++;
    return c;
}

NotificationQueue::NotificationQueue()
    : head(nullptr), tail(nullptr), count(0) {}

NotificationQueue::~NotificationQueue() {
    while (head) {
        Node* next = head->next;
        delete head;
        head = next;
    }
}

void NotificationQueue::push(const Notification& n) {
    Node* node = new Node(n, nullptr);
    if (!tail) { head = tail = node; }
    else       { tail->next = node; tail = node; }
    count++;
}

Notification NotificationQueue::pop() {
    if (!head) throw std::runtime_error("Queue is empty");
    Notification data = head->data;
    Node* old = head;
    head = head->next;
    if (!head) tail = nullptr;
    delete old;
    count--;
    return data;
}

int NotificationPriorityQueue::type_rank(const Notification& n) {
    if (n.type == NOTIF_SYSTEM && n.system.severity == SEV_URGENT) return 3;
    if (n.type == NOTIF_MESSAGE) return 2;
    if (n.type == NOTIF_SYSTEM)  return 1;
    return 0;
}

Notification NotificationPriorityQueue::pop() {
    // recount from actual list — avoids any count drift
    int actual = 0;
    for (Node* n = head; n; n = n->next) actual++;
    if (actual == 0) throw std::runtime_error("Queue is empty");

    // find highest priority node
    // We track (prev, node) pairs by walking the list
    Node* best_prev = nullptr;
    Node* best      = head;

    Node* prev = nullptr;
    Node* cur  = head;

    while (cur) {
        bool cur_wins = false;
        bool best_urg = (best->data.type == NOTIF_SYSTEM &&
                         best->data.system.severity == SEV_URGENT);
        bool cur_urg  = (cur->data.type  == NOTIF_SYSTEM &&
                         cur->data.system.severity  == SEV_URGENT);

        if (cur != best) {  // don't compare best against itself
            if      (cur_urg && !best_urg) cur_wins = true;
            else if (!cur_urg && best_urg) cur_wins = false;
            else if (cur->data.timestamp < best->data.timestamp) cur_wins = true;
            else if (cur->data.timestamp == best->data.timestamp)
                if (type_rank(cur->data) > type_rank(best->data)) cur_wins = true;

            if (cur_wins) { best_prev = prev; best = cur; }
        }

        prev = cur;
        cur  = cur->next;
    }

    // unlink best from list
    if (best_prev) {
        best_prev->next = best->next;
    } else {
        head = best->next;
    }

    // fix tail if we removed the last node
    if (best->next == nullptr) {
        tail = best_prev;
    }

    Notification data = best->data;
    delete best;

    // set count from actual count minus 1
    count = actual - 1;
    return data;
}
