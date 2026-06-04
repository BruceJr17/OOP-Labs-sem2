#pragma once
#include "circle.h"
#include <iostream>
#include <fstream>

class List {
private:
    struct Node {
        Node* pPrev;
        Node* pNext;
        Circle m_Data;

        Node();                              // guard node constructor
        Node(Node* prev, Node* next, const Circle* pc); // real node
        ~Node();                             // unlinks itself
    };

    Node Head;
    Node Tail;
    size_t m_size;

public:
    List();
    ~List();
    // TODO: implement copy and move c-tors and operators

// Copy constructor: deep copies all nodes from other list.
    // Move constructor: steals Head/Tail links, leaves other empty.
    // Without these the compiler generates shallow copies which would
    // share node pointers and cause double-free on destruction.
    List(const List& other);
    List(List&& other);
    List& operator=(const List& other);
    List& operator=(List&& other);

    // TODO: use const references here
    
    // Using const Circle& instead of const Circle* is safer and more
    // idiomatic C++ — references cannot be null, no pointer syntax needed.
    void push_front(const Circle& c);
    void push_back(const Circle& c);
    bool remove_first(const Circle& c);
    int  remove_all(const Circle& c);
    void clear();
    size_t size() const { return m_size; }

    // TODO: provide an efficient API to get access to elements (e.g. iterator)
     
    // An iterator wraps a Node* and supports ++, *, != so the list
    // can be used in range-based for loops: for (auto& c : list)
    // This is O(1) per step vs O(n) if you used index-based access.
    struct Iterator {
        Node* node;
        Iterator(Node* n) : node(n) {}
        Circle& operator*()  { return node->m_Data; }
        Circle* operator->() { return &node->m_Data; }
        Iterator& operator++() { node = node->pNext; return *this; }
        bool operator!=(const Iterator& o) const { return node != o.node; }
    };
    Iterator begin() { return Iterator(Head.pNext); }
    Iterator end()   { return Iterator(&Tail); }

    void sort_by_area();           // insertion sort (current)
    void sort_bubble();            // bubble sort (old)
    void sort_insertion_ptr();     // insertion sort by pointer manipulation
    void sort_merge();             // merge sort O(n log n)
    void print() const;

    friend std::ostream& operator<<(std::ostream& os, const List& l);

    // TODO: implement versions of such methods, that works with std::ostream&
    //       and std::istream& instead of file names

    // Stream versions are more flexible — they work with any stream
    // (file, network, string buffer) not just files by name.
    // operator<< already handles ostream. For istream:
    void write_to_stream(std::ostream& os) const;
    void read_from_stream(std::istream& is);
};
