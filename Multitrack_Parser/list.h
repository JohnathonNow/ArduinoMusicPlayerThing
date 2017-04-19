#ifndef _LIST_H
#define _LIST_H
template <typename T>
class List {
    private:
    /* elements in the list */
    struct Node {
        Node* next;
        T val;
    };
    Node* head; /* The first element of the list */
    Node* tail; /* The last element of the list */

    public:
    List() {
        /* Initialize all of our pointers */
        head = nullptr;
        tail = nullptr;
    }
    ~List() {
        if (tail) /* Break the cycle of the list */
            tail->next = nullptr;
        while (head) { /* Delete all of our allocated nodes */
            tail = head->next;
            delete head;
            head = tail;
        }
    }
    void addNode(T val) {
        Node* n = new Node{nullptr, val}; /* Allocate a new node */
        if (tail) {  /* if it is not our first, add it to the end */
            tail->next = tail =  n;
        } else { /* otherwise it is our one and only node */
            head = tail = n;
        }
    }
};
#endif
