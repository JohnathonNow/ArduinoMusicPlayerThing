#ifndef _CLL_H
#define _CLL_H
template <typename T>
class CircularlyLinkedList {
    private:
    /* elements in the list */
    struct Node {
        Node* next;
        T val;
    };
    Node* head; /* The first element of the list */
    Node* tail; /* The last element of the list */
    Node* curr; /* The element of the list we are currently on */
    public:
    CircularlyLinkedList() {
        /* Initialize all of our pointers */
        head = NULL;
        tail = NULL;
        curr = NULL;
    }
    ~CircularlyLinkedList() {
        if (tail) /* Break the cycle of the list */
            tail->next = nullptr;
        while (head) { /* Delete all of our allocated nodes */
            curr = head->next;
            delete head;
            head = curr;
        }
    }
    void addNode(T val) {
        Node* n = new Node{head, val}; /* Allocate a new node */
        if (tail) {  /* if it is not our first, add it to the end */
            tail->next = n;
            tail =  n;
        } else { /* otherwise it is our one and only node */
            head = n;
            tail = n;
            n->next = n;
            curr = n;
        }
    }
    T getCurr() { /* return our current value */
        return curr->val;
    }
    T getTail() { /* return our tail value */
        return tail->val;
    }
    bool next() { /* advance our current pointer */
        curr = curr->next;
        return (curr == head);
    }

    void reset() { /* reset our current pointer */
        curr = head;
    }
};
#endif


