#include <iostream>
#include <cassert>
#include "my_string.h"
#include "matrix.h"
#include "worker_db.h"
#include "notification.h"
#include "base_file.h"
#include "bool_array.h"
using namespace std;

int main() {
    // ── Task 1.1: assignment operator ────────────────────────────────────
    {
        cout << "--- Task 1.1: MyString assignment ---\n";
        MyString s1("AAA"), s2;
        s2 = s1;            // copy assign
        s1 = "CCC";         // assign from c-string
        s2 = MyString("tmp"); // assign from temporary
        s1 = s1;            // self-assignment — must not crash
        cout << "s1=" << s1 << " s2=" << s2 << "\n";
    }

    // ── Task 1.2.4: concatenation operators ──────────────────────────────
    {
        cout << "\n--- Task 1.2.4: MyString concatenation ---\n";
        MyString s1("abc"), s2("def"), s3;
        s1 += s2 += "111";          // s2 = "def111", s1 = "abcdef111"
        cout << "s1=" << s1 << " s2=" << s2 << "\n";

        s2 += "222" + s1 + "333";   // "222" + s1 = "222abcdef111"
        cout << "s2=" << s2 << "\n";

        s3 += s3;                   // empty += empty = still empty
        cout << "s3='" << s3 << "'\n";
    }

    // ── Task 1.3: move semantics ──────────────────────────────────────────
    {
        cout << "\n--- Task 1.3: move semantics ---\n";
        MyString s1("abc");
        MyString s2 = s1 + "ccc";   // move constructor used for return value
        cout << "s2=" << s2 << "\n";
        s1 = "abc" + s2;
        cout << "s1=" << s1 << "\n";
        s2 = std::move(s1);         // s1 becomes empty, s2 gets s1's data
        cout << "s2 after move=" << s2 << "\n";
        cout << "s1 after move='" << s1 << "'\n";
    }

    // ── Task 1.4: comparison operators ───────────────────────────────────
    {
        cout << "\n--- Task 1.4: comparison operators ---\n";
        MyString s1("abc"), s2("ab"), s3 = s1;
        assert(s2 < s1);
        assert(s1 == s3);
        assert(s1 != s2);
        assert(s1 <= s3);
        assert("ab" == s2);
        assert("aba" < s1);   // "aba" > "abc"? no — 'b'<'b' no, 'a'<'c' yes
        assert("aba" <= s3);
        cout << "All comparison asserts passed!\n";
    }

    // ── Task 1.5: index operator ──────────────────────────────────────────
    {
        cout << "\n--- Task 1.5: index operator ---\n";
        MyString s1("abc");
        s1[0] = s1[1] = 'd';    // s1 becomes "ddc"
        cout << "s1=" << s1 << "\n";
    }

    // ── Task 1.6: stream operators ────────────────────────────────────────
    {
        cout << "\n--- Task 1.6: stream operators ---\n";
        MyString s("123");
        cout << "This is my string: '" << s << "'\n";
    }

    // ── Task 1.1: Matrix assignment ───────────────────────────────────────
    {
        cout << "\n--- Task 1.1: Matrix assignment ---\n";
        Matrix m1(10), m2(10, 10, 2);
        m1 = m2;                    // copy assign
        m2 = Matrix(3, 4, 1.0);     // assign from temporary
        m1 = m1;                    // self-assignment — must not crash
        cout << "m1 size: " << m1.get_height() << "x" << m1.get_width() << "\n";
        cout << "m2 size: " << m2.get_height() << "x" << m2.get_width() << "\n";
    }

    // ── Task 1.2.1: compound assignment operators ─────────────────────────
    {
        cout << "\n--- Task 1.2.1: Matrix compound assignment ---\n";
        Matrix m1(4), m2(4, 4, 1);
        m2 += m1;
        m2 -= m1 += m1;   // m1 *= 2 via +=, then m2 -= that
        m1 *= 2;
        m2 /= 3;
        cout << "m1[0][0]=" << m1.get(0,0) << "\n";
        cout << "m2[0][0]=" << m2.get(0,0) << "\n";
    }

    // ── Task 1.2.2: binary operators ─────────────────────────────────────
    {
        cout << "\n--- Task 1.2.2: Matrix binary operators ---\n";
        Matrix m1(4), m2(4, 4, 1);
        Matrix m3 = (m1 + m2) * m1 * 4 / 2 - m2;
        cout << "m3 size: " << m3.get_height() << "x" << m3.get_width() << "\n";
        cout << "m3[0][0]=" << m3.get(0,0) << "\n";
    }

    // ── Task 1.2.3: global operators ─────────────────────────────────────
    {
        cout << "\n--- Task 1.2.3: global operators ---\n";
        Matrix m1(4, 4, 3);
        Matrix m2 = -m1;          // unary minus
        Matrix m3 = 2.0 * m1;    // double * Matrix
        cout << "m2[0][0]=" << m2.get(0,0) << "\n";  // should be -3
        cout << "m3[0][0]=" << m3.get(0,0) << "\n";  // should be 6
    }

    // ── Task 1.3: Matrix move semantics ──────────────────────────────────
    {
        cout << "\n--- Task 1.3: Matrix move semantics ---\n";
        Matrix m1(4), m2 = m1 + m1;
        m1 = m2 * m1;
        m2 = move(m1);   // m1 becomes empty
        cout << "m2 size after move: "
                  << m2.get_height() << "x" << m2.get_width() << "\n";
    }

    // ── Task 3.1 + 3.2 + 3.3: WorkerDb ──────────────────────────────────
    {
        cout << "\n--- Task 3: WorkerDb ---\n";
        WorkerDb db;

        db["Ivanov"]  = WorkerData("Ivan",  34, "Engineering");
        db["Petrov"]  = WorkerData("Petr",  43, "Marketing");
        db["Sidorov"] = WorkerData("Sidor", 28, "Engineering");

        cout << "Ivanov's name = " << db["Ivanov"].name  << "\n";
        cout << "Petrov's age  = " << db["Petrov"].age   << "\n";

        // Task 3.2: iterator
        cout << "\nIterating with iterator:\n";
        for (auto it = db.begin(); it != db.end(); ++it) {
            cout << "  " << it.key() << " -> " << it->name << "\n";
        }

        // Task 3.3: free functions
        print_db(db);
        cout << "Average age: " << get_avg_age(db) << "\n";
    }

    // ── Task 4.1: Notification struct with union ──────────────────────────
    {
        cout << "\n--- Task 4.1: Notifications ---\n";

        Notification arr[5] = {
            make_system_notification (100, "Low battery",      SEV_NORMAL),
            make_message_notification(101, "Alice",            "Hey!"),
            make_app_notification    (102, "Gmail", "New mail","You have 3 new emails"),
            make_system_notification (103, "System overload!", SEV_URGENT),
            make_message_notification(104, "Bob",              "Lunch?"),
        };

        for (int i = 0; i < 5; i++) print_notification(arr[i]);

        cout << "System count:  " << count_by_type(arr, 5, NOTIF_SYSTEM)  << "\n";
        cout << "Message count: " << count_by_type(arr, 5, NOTIF_MESSAGE) << "\n";
        cout << "App count:     " << count_by_type(arr, 5, NOTIF_APP)     << "\n";
    }

    // ── Task 4.3: NotificationQueue (FIFO) ───────────────────────────────

    {
        cout << "\n--- Task 4.3: NotificationQueue (FIFO) ---\n";
        NotificationQueue q;
        q.push(make_system_notification (1, "Boot complete",   SEV_NORMAL));
        q.push(make_message_notification(2, "Carol",           "Hi!"));
        q.push(make_app_notification    (3, "Spotify","Song",  "Now playing"));

        cout << "Queue size: " << q.size() << "\n";
        while (q.size() > 0) {
            print_notification(q.pop());
        }
    }

    // ── Task 4.4: NotificationPriorityQueue ──────────────────────────────

    {
        cout << "\n--- Task 4.4: NotificationPriorityQueue ---\n";
        NotificationPriorityQueue pq;

        pq.push(make_app_notification    (10, "Maps","Traffic", "Delay"));
        pq.push(make_system_notification (10, "Disk full!",    SEV_URGENT));
        pq.push(make_message_notification(10, "Dave",          "Call me"));
        pq.push(make_system_notification (10, "Update ready",  SEV_NORMAL));
        pq.push(make_message_notification(5,  "Eve",           "Early msg"));

        cout << "Queue size before popping: " << pq.size() << "\n";
        cout << "Popping in priority order:\n";
        while (pq.size() > 0) {
            cout << "  size=" << pq.size() << " -> ";
            print_notification(pq.pop());
        }
    }

// ── Task 5: delete copy, add move for BaseFile ────────────────────────
    {
        cout << "\n--- Task 5: BaseFile move semantics ---\n";

        // This would fail to compile — copy is deleted:
        // BaseFile bf1("build/t.txt", "w");
        // BaseFile bf2 = bf1;   // ERROR: copy constructor deleted

        // Move is allowed — transfers ownership of FILE*
        BaseFile bf1("build/task5.txt", "w");
        BaseFile bf2 = std::move(bf1);  // bf1 no longer owns the file
        bf2.write("written via moved file\n", 23);
        // bf1 is now empty — its file pointer is null
        cout << "bf1 is_open after move: " << bf1.is_open() << "\n";
        cout << "bf2 is_open after move: " << bf2.is_open() << "\n";
    }

// ── Task 6: BoolArray ─────────────────────────────────────────────────
    {
        cout << "\n--- Task 6: BoolArray ---\n";

        BoolArray ar1(10);          // 10 x false
        BoolArray ar2(5, true);     // 5 x true
        BoolArray ar3(ar2);         // deep copy of ar2

        ar1[4] = ar1[6] = true;

        ar1[2] = (!ar1[6] && ar1[8] || (ar1[0] != true));

        cout << "ar1: [";
        for (int i = 0; i < ar1.size(); i++) {
            if (i > 0) cout << ", ";
            cout << (bool)ar1[i];
        }
        cout << "]\n";

        cout << "ar1 true indices: [";
        for (int i = 0, printed = 0; i < ar1.size(); i++) {
            if (ar1[i]) {
                if (printed++ > 0) cout << ", ";
                cout << i;
            }
        }
        cout << "]\n";

        ar1.resize(12, true);
        cout << "after resize(12,true): [";
        for (int i = 0; i < ar1.size(); i++) {
            if (i > 0) cout << ", ";
            cout << (bool)ar1[i];
        }
        cout << "]\n";

        ar1.resize(4);
        cout << "after resize(4): [";
        for (int i = 0; i < ar1.size(); i++) {
            if (i > 0) cout << ", ";
            cout << (bool)ar1[i];
        }
        cout << "]\n";

        // verify ar3 is independent copy of ar2
        ar2[0] = false;
        cout << "ar2[0]=" << (bool)ar2[0]
                  << " ar3[0]=" << (bool)ar3[0]
                  << " (should differ)\n";
    }

    return 0;
}
