#include "worker_db.h"
#include <iostream>
using namespace std;

// ── WorkerDb ──────────────────────────────────────────────────────────────────

WorkerDb::WorkerDb() : head(nullptr), count(0) {}

WorkerDb::~WorkerDb() {
    WorkerNode *cur = head;
    while (cur) {
        WorkerNode *next = cur->next;
        delete cur;
        cur = next;
    }
}

// find existing node by key, return nullptr if not found
static WorkerNode* find_node(WorkerNode *head, const MyString& key) {
    for (WorkerNode *cur = head; cur; cur = cur->next)
        if (cur->key == key) return cur;
    return nullptr;
}

WorkerData& WorkerDb::operator[](const MyString& last_name) {
    WorkerNode *node = find_node(head, last_name);
    if (node) return node->value;

    // not found — create new entry at front of list
    WorkerNode *new_node = new WorkerNode(last_name, WorkerData(), head);
    head = new_node;
    count++;
    return new_node->value;
}

WorkerData& WorkerDb::operator[](const char* last_name) {
    return (*this)[MyString(last_name)];
}

// ── free functions ────────────────────────────────────────────────────────────

void print_db(WorkerDb& db) {
    cout << "=== Worker Database ===\n";
    for (auto it = db.begin(); it != db.end(); ++it) {
        cout << "  " << it.key()
                  << " -> name=" << it->name
                  << "  age="    << it->age
                  << "  dept="   << it->dept
                  << "\n";
    }
    cout << "======================\n";
}

double get_avg_age(WorkerDb& db) {
    if (db.size() == 0) return 0.0;
    double total = 0;
    for (auto it = db.begin(); it != db.end(); ++it)
        total += it->age;
    return total / db.size();
}
