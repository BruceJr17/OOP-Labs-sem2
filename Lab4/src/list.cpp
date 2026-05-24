#include "list.h"
#include <functional>
// ── Node ─────────────────────────────────────────────────────────────────────

// guard node — not linked to anything yet
List::Node::Node() : pPrev(nullptr), pNext(nullptr), m_Data() {}

// real node — links itself between prev and next
List::Node::Node(Node* prev, Node* next, const Circle* pc)
    : pPrev(prev), pNext(next), m_Data(*pc)
{
    prev->pNext = this;
    next->pPrev = this;
}

// unlinks itself from the list
List::Node::~Node() {
    if (pPrev) pPrev->pNext = pNext;
    if (pNext) pNext->pPrev = pPrev;
}

// ── List ──────────────────────────────────────────────────────────────────────

List::List() : m_size(0) {
    Head.pNext = &Tail;
    Tail.pPrev = &Head;
}

// Copy constructor
List::List(const List& other) : m_size(0) {
    Head.pNext = &Tail;
    Tail.pPrev = &Head;
    for (Node* cur = other.Head.pNext; cur != &other.Tail; cur = cur->pNext) {
        push_back(cur->m_Data);
    }
}

// Move constructor
List::List(List&& other) : m_size(other.m_size) {
    Head.pNext = other.Head.pNext;
    Tail.pPrev = other.Tail.pPrev;
    Head.pNext->pPrev = &Head;
    Tail.pPrev->pNext = &Tail;
    other.Head.pNext = &other.Tail;
    other.Tail.pPrev = &other.Head;
    other.m_size = 0;
}

// Copy assignment operator
List& List::operator=(const List& other) {
    if (this != &other) {
        clear();
        for (Node* cur = other.Head.pNext; cur != &other.Tail; cur = cur->pNext) {
            push_back(cur->m_Data);
        }
    }
    return *this;
}

// Move assignment operator
List& List::operator=(List&& other) {
    if (this != &other) {
        clear();
        Head.pNext = other.Head.pNext;
        Tail.pPrev = other.Tail.pPrev;
        Head.pNext->pPrev = &Head;
        Tail.pPrev->pNext = &Tail;
        m_size = other.m_size;
        other.Head.pNext = &other.Tail;
        other.Tail.pPrev = &other.Head;
        other.m_size = 0;
    }
    return *this;
}

List::~List() {
    clear();
}

void List::push_front(const Circle& c) {
    new Node(&Head, Head.pNext, &c);
    m_size++;
}

void List::push_back(const Circle& c) {
    new Node(Tail.pPrev, &Tail, &c);
    m_size++;
}

bool List::remove_first(const Circle& c) {
    for (Node* cur = Head.pNext; cur != &Tail; cur = cur->pNext) {
        if (cur->m_Data == c) {
            delete cur;
            m_size--;
            return true;
        }
    }
    return false;
}

int List::remove_all(const Circle& c) {
    int count = 0;
    Node* cur = Head.pNext;
    while (cur != &Tail) {
        Node* next = cur->pNext;
        if (cur->m_Data == c) {
            delete cur;
            m_size--;
            count++;
        }
        cur = next;
    }
    return count;
}

void List::clear() {
    Node* cur = Head.pNext;
    while (cur != &Tail) {
        Node* next = cur->pNext;
        delete cur;
        cur = next;
    }
    m_size = 0;
}


// ── Benchmark Results (average of 5 runs, random data) ───────────────────────
// Size       Bubble       Insertion    Insert-Ptr   Merge
// 1000       5.91 ms      1.09 ms      0.82 ms      0.15 ms
// 10000      845.24 ms    169.75 ms    131.43 ms    2.23 ms
// 100000     221069 ms    41603 ms     43392 ms     51.16 ms
//
// Conclusion:
// - Bubble sort is the slowest — O(n^2) with many data swaps
// - Insertion sort (data swap) is ~5x faster than bubble
// - Insertion sort (pointer) is slightly faster than data swap
// - Merge sort is by far the fastest — O(n log n)
//   At 100000 elements: bubble=221s, merge=0.05s (4000x faster!)
// ─────────────────────────────────────────────────────────────────────────────

void List::sort_by_area() {
    // bubble sort — swap data between nodes
    // TODO: what is big-O complexity of such sort? can it be better?
    //       if it can, implement better algorithm

    // Bubble sort is O(n^2) — for each pass it compares adjacent elements.
    // It could be better: merge sort on a linked list is O(n log n)
    // and works efficiently without random access.
    // For small lists bubble sort is acceptable.

    // Insertion sort O(n^2) worst case, better than bubble sort in practice.
    if (m_size < 2) return;
    Node* cur = Head.pNext->pNext;
    while (cur != &Tail) {
        Node* next = cur->pNext;
        Circle key = cur->m_Data;
        Node* pos = cur->pPrev;
        while (pos != &Head && pos->m_Data.area() > key.area()) {
            pos->pNext->m_Data = pos->m_Data;
            pos = pos->pPrev;
        }
        pos->pNext->m_Data = key;
        cur = next;
     }
   }

    void List::sort_bubble() {
     // Bubble sort 0(n^2) - repeatedly swaps adjacent elements
    if (m_size < 2) return;
    bool swapped = true;
    while (swapped) {
        swapped = false;
        for (Node* cur = Head.pNext; cur->pNext != &Tail; cur = cur->pNext) {
            if (cur->m_Data.area() > cur->pNext->m_Data.area()) {
                Circle tmp = cur->m_Data;
                cur->m_Data = cur->pNext->m_Data;
                cur->pNext->m_Data = tmp;
                swapped = true;
            }
        }
    }
 }

     void List::sort_insertion_ptr() {
     // Insertion sort by relinking nodes - on data copying
     if(m_size < 2) return;
     Node* cur = Head.pNext->pNext;
     while (cur != &Tail) {
        Node* next = cur->pNext;
        // unlink cur
        cur->pPrev->pNext = cur->pNext;
        cur->pNext->pPrev = cur->pPrev;
        // find insertion point
        Node* pos = cur->pPrev;
        while (pos != &Head && pos->m_Data.area() > cur->m_Data.area()) {
            pos = pos->pPrev;
        }
        // relink cur after pos
        cur->pNext = pos->pNext;
        cur->pPrev = pos;
        pos->pNext->pPrev = cur;
        pos->pNext = cur;
        cur = next;
    }
}

      void List::sort_merge() {
    // Merge sort O(n log n) using node relinking
    if (m_size < 2) return;

    // detach all nodes into a raw chain
    Node* chain = Head.pNext;
    chain->pPrev = nullptr;
    Tail.pPrev->pNext = nullptr;
    Head.pNext = &Tail;
    Tail.pPrev = &Head;
    m_size = 0;

    // merge sort the raw chain
    std::function<Node*(Node*)> merge_sort = [&](Node* h) -> Node* {
        if (!h || !h->pNext) return h;
        // split
        Node* slow = h;
        Node* fast = h->pNext;
        while (fast && fast->pNext) {
            slow = slow->pNext;
            fast = fast->pNext->pNext;
        }
        Node* mid = slow->pNext;
        slow->pNext = nullptr;
        // recurse
        Node* l = merge_sort(h);
        Node* r = merge_sort(mid);
        // merge
        Node dummy;
        Node* tail = &dummy;
        while (l && r) {
            if (l->m_Data.area() <= r->m_Data.area()) {
                tail->pNext = l; l = l->pNext;
            } else {
                tail->pNext = r; r = r->pNext;
            }
            tail = tail->pNext;
        }
        tail->pNext = l ? l : r;
        return dummy.pNext;
    };

    chain = merge_sort(chain);

    // reattach sorted chain
    Node* cur = chain;
    Node* prev = &Head;
    while (cur) {
        prev->pNext = cur;
        cur->pPrev = prev;
        prev = cur;
        cur = cur->pNext;
        m_size++;
    }
    prev->pNext = &Tail;
    Tail.pPrev = prev;
}

void List::print() const {
    std::cout << *this;
}

std::ostream& operator<<(std::ostream& os, const List& l) {
    os << "List [" << l.m_size << " elements]:\n";
    for (List::Node* cur = l.Head.pNext; cur != &l.Tail; cur = cur->pNext) {
        os << "  ";
        cur->m_Data.print();
        os << "\n";
    }
    return os;
}

void List::write_to_stream(std::ostream& os) const {
    os << m_size << "\n";
    for (Node* cur = Head.pNext; cur != &Tail; cur = cur->pNext) {
        os << cur->m_Data << "\n";
    }
}

void List::read_from_stream(std::istream& is) {
    clear();
    int n;
    is >> n;
    for (int i = 0; i < n; i++) {
        Circle c;
        is >> c;
        push_back(c);
    }
}

