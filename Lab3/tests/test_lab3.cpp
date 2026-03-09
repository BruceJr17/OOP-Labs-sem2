#include <cassert>
#include <iostream>
#include "../src/my_string.h"
#include "../src/matrix.h"
#include "../src/worker_db.h"
#include "../src/notification.h"
#include "../src/bool_array.h"

void test_mystring_operators() {
    MyString s1("AAA"), s2;
    s2 = s1;
    assert(s2 == "AAA");
    s1 = s1;
    assert(s1 == "AAA");

    MyString a("hello"), b(" world");
    a += b;
    assert(a == "hello world");
    MyString c = a + "!";
    assert(c == "hello world!");
    MyString d = "say: " + c;
    assert(d == "say: hello world!");

    MyString m1("abc");
    MyString m2 = std::move(m1);
    assert(m2 == "abc");
    assert(m1 == "");

    MyString x("abc"), y("ab"), z("abc");
    assert(y < x);
    assert(x == z);
    assert(x != y);
    assert(x <= z);
    assert("ab" == y);
    assert("ab" < x);

    MyString s("abc");
    s[0] = 'X';
    assert(s[0] == 'X');

    std::cout << "test_mystring_operators PASSED\n";
}

void test_matrix_operators() {
    Matrix m1(3, 3, 1.0), m2(3, 3, 2.0);
    m1 = m2;
    assert(m1.get(0,0) == 2.0);
    m1 = m1;
    assert(m1.get(0,0) == 2.0);

    Matrix a(2, 2, 3.0), b(2, 2, 1.0);
    a += b;
    assert(a.get(0,0) == 4.0);
    a -= b;
    assert(a.get(0,0) == 3.0);
    a *= 2.0;
    assert(a.get(0,0) == 6.0);
    a /= 3.0;
    assert(a.get(0,0) == 2.0);

    Matrix c(2, 2, 1.0), d(2, 2, 2.0);
    Matrix e = c + d;
    assert(e.get(0,0) == 3.0);
    Matrix f = d - c;
    assert(f.get(0,0) == 1.0);
    Matrix g = d * 3.0;
    assert(g.get(0,0) == 6.0);
    Matrix h = 2.0 * d;
    assert(h.get(0,0) == 4.0);
    Matrix neg = -d;
    assert(neg.get(0,0) == -2.0);

    Matrix mv1(2, 2, 5.0);
    Matrix mv2 = std::move(mv1);
    assert(mv2.get(0,0) == 5.0);

    std::cout << "test_matrix_operators PASSED\n";
}

void test_worker_db() {
    WorkerDb db;
    db["Ivanov"] = WorkerData("Ivan",  30, "IT");
    db["Petrov"] = WorkerData("Petr",  40, "HR");
    db["Sidorov"]= WorkerData("Sidor", 50, "IT");

    assert(db["Ivanov"].age  == 30);
    assert(db["Petrov"].age  == 40);
    assert(db["Sidorov"].age == 50);
    assert(db.size() == 3);

    int count = 0;
    for (auto it = db.begin(); it != db.end(); ++it) count++;
    assert(count == 3);

    assert(get_avg_age(db) == 40.0);
    std::cout << "test_worker_db PASSED\n";
}

void test_notifications() {
    Notification arr[3] = {
        make_system_notification (1, "x", SEV_NORMAL),
        make_message_notification(2, "A", "hi"),
        make_app_notification    (3, "App", "t", "b"),
    };
    assert(count_by_type(arr, 3, NOTIF_SYSTEM)  == 1);
    assert(count_by_type(arr, 3, NOTIF_MESSAGE) == 1);
    assert(count_by_type(arr, 3, NOTIF_APP)     == 1);

    NotificationQueue q;
    q.push(arr[0]); q.push(arr[1]); q.push(arr[2]);
    assert(q.size() == 3);
    Notification p1 = q.pop();
    assert(p1.type == NOTIF_SYSTEM);
    assert(q.size() == 2);

    NotificationPriorityQueue pq;
    pq.push(make_app_notification    (10, "A","t","b"));
    pq.push(make_system_notification (10, "urgent!", SEV_URGENT));
    pq.push(make_message_notification(10, "B", "hi"));
    pq.push(make_system_notification (10, "normal",  SEV_NORMAL));
    pq.push(make_message_notification(5,  "C", "early"));

    Notification n1 = pq.pop();
    assert(n1.type == NOTIF_SYSTEM && n1.system.severity == SEV_URGENT);
    Notification n2 = pq.pop();
    assert(n2.timestamp == 5);
    Notification n3 = pq.pop();
    assert(n3.type == NOTIF_MESSAGE);
    Notification n4 = pq.pop();
    assert(n4.type == NOTIF_SYSTEM);
    Notification n5 = pq.pop();
    assert(n5.type == NOTIF_APP);

    std::cout << "test_notifications PASSED\n";
}

void test_bool_array() {
    BoolArray ar(10, false);
    assert(ar.size() == 10);
    for (int i = 0; i < 10; i++) assert((bool)ar[i] == false);

    ar[3] = ar[7] = true;
    assert((bool)ar[3] == true);
    assert((bool)ar[7] == true);
    assert((bool)ar[0] == false);

    BoolArray ar2(ar);
    ar2[3] = false;
    assert((bool)ar[3]  == true);
    assert((bool)ar2[3] == false);

    ar.resize(15, true);
    assert(ar.size() == 15);
    assert((bool)ar[3]  == true);
    assert((bool)ar[10] == true);

    ar.resize(5);
    assert(ar.size() == 5);
    assert((bool)ar[3] == true);

    std::cout << "test_bool_array PASSED\n";
}

int main() {
    test_mystring_operators();
    test_matrix_operators();
    test_worker_db();
    test_notifications();
    test_bool_array();
    std::cout << "\nAll Lab 3 tests passed!\n";
    return 0;
}
