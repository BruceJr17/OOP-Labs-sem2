#include "list.h"

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

  /*bool swapped = true;
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
    } */
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

