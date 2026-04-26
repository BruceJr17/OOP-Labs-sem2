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

List::~List() {
    clear();
}

void List::push_front(const Circle* c) {
    new Node(&Head, Head.pNext, c);
    m_size++;
}

void List::push_back(const Circle* c) {
    new Node(Tail.pPrev, &Tail, c);
    m_size++;
}

bool List::remove_first(const Circle* c) {
    for (Node* cur = Head.pNext; cur != &Tail; cur = cur->pNext) {
        if (cur->m_Data == *c) {
            delete cur;
            m_size--;
            return true;
        }
    }
    return false;
}

int List::remove_all(const Circle* c) {
    int count = 0;
    Node* cur = Head.pNext;
    while (cur != &Tail) {
        Node* next = cur->pNext;
        if (cur->m_Data == *c) {
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

void List::write_to_file(const char* filename) const {
    std::ofstream fout(filename);
    fout << m_size << "\n";
    for (Node* cur = Head.pNext; cur != &Tail; cur = cur->pNext) {
        fout << cur->m_Data << "\n";
    }
    fout.close();
}

void List::read_from_file(const char* filename) {
    std::ifstream fin(filename);
    if (!fin) return;
    clear();
    int n;
    fin >> n;
    for (int i = 0; i < n; i++) {
        Circle c;
        fin >> c;
        push_back(&c);
    }
    fin.close();
}

