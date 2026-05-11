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

    // TODO: use const references here
    void push_front(const Circle* c);
    void push_back(const Circle* c);
    bool remove_first(const Circle* c);
    int  remove_all(const Circle* c);
    void clear();
    size_t size() const { return m_size; }

    // TODO: provide an efficient API to get access to elements (e.g. iterator)
     
    void sort_by_area();
    void print() const;

    friend std::ostream& operator<<(std::ostream& os, const List& l);

    // TODO: implement versions of such methods, that works with std::ostream&
    //       and std::istream& instead of file names
    void write_to_file(const char* filename) const;
    void read_from_file(const char* filename);
};
