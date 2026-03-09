#pragma once
#include "my_string.h"

// ── Worker data ───────────────────────────────────────────────────────────────
struct WorkerData {
    MyString name;    // first name
    int      age;
    MyString dept;    // department

    WorkerData() : name(""), age(0), dept("") {}
    WorkerData(const char* n, int a, const char* d)
        : name(n), age(a), dept(d) {}
};

// ── Internal node for the associative array ───────────────────────────────────
struct WorkerNode {
    MyString   key;    // last name (the index)
    WorkerData value;
    WorkerNode *next;

    WorkerNode(const MyString& k, const WorkerData& v, WorkerNode* n)
        : key(k), value(v), next(n) {}
};

// ── Iterator ──────────────────────────────────────────────────────────────────
class WorkerDbIterator {
    WorkerNode *node;
public:
    WorkerDbIterator(WorkerNode *n) : node(n) {}

    // dereference — returns the WorkerData
    WorkerData&  operator*()  { return node->value; }
    WorkerData*  operator->() { return &node->value; }

    // key() returns the last name for this entry
    const MyString& key() const { return node->key; }

    // prefix ++
    WorkerDbIterator& operator++() {
        node = node->next;
        return *this;
    }

    // postfix ++
    WorkerDbIterator operator++(int) {
        WorkerDbIterator tmp = *this;
        node = node->next;
        return tmp;
    }

    bool operator==(const WorkerDbIterator& other) const {
        return node == other.node;
    }
    bool operator!=(const WorkerDbIterator& other) const {
        return node != other.node;
    }
};

// ── WorkerDb ──────────────────────────────────────────────────────────────────
class WorkerDb {
    WorkerNode *head;
    int         count;

public:
    WorkerDb();
    ~WorkerDb();

    // [] operator: returns reference to existing entry or creates a new one
    WorkerData& operator[](const char* last_name);
    WorkerData& operator[](const MyString& last_name);

    int size() const { return count; }

    WorkerDbIterator begin() { return WorkerDbIterator(head); }
    WorkerDbIterator end()   { return WorkerDbIterator(nullptr); }
};

// ── free functions (Task 3.3) ─────────────────────────────────────────────────
void   print_db(WorkerDb& db);
double get_avg_age(WorkerDb& db);
