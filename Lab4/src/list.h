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

    void push_front(const Circle* c);
    void push_back(const Circle* c);
    bool remove_first(const Circle* c);
    int  remove_all(const Circle* c);
    void clear();
    size_t size() const { return m_size; }

    void sort_by_area();
    void print() const;

    friend std::ostream& operator<<(std::ostream& os, const List& l);

    void write_to_file(const char* filename) const;
    void read_from_file(const char* filename);
};
