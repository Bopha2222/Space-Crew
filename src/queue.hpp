
#include "LinkedList.hpp" 

class Queue {
private:
    LinkedList list;

public:
    Queue() {
      
    }

    void enqueue(int value) {
        list.append(value);
    }

    
    void dequeue() {
        Node* temp = list.head; 
        if (temp == nullptr) {
            cout << "Queue is empty!" << endl;
            return;
        }
        list.head = temp->next; 
        delete temp;
    }


    bool isEmpty() {
        return list.head == nullptr;
    }


    void printQueue() {
        list.printList();
    }

    int front() {
        if (list.head != nullptr)
            return list.head->data;
        else {
            cout << "Queue is empty!" << endl;
            return -1; 
        }
    }
};
