#include "LinkedList.hpp" 


class Stack {
private:
    LinkedList list; 

public:
    Stack() {
        
    }


    void push(int value) {
        Node* newNode = new Node(value);
        newNode->next = list.head;
        list.head = newNode;
    }


    void pop() {
        if (list.head == nullptr) {
            cout << "Stack is empty!" << endl;
            return;
        }
        Node* temp = list.head;
        list.head = list.head->next;
        delete temp;
    }


    int top() {
        if (list.head != nullptr)
            return list.head->data;
        else {
            cout << "Stack is empty!" << endl;
            return -1;
        }
    }

    bool isEmpty() {
        return list.head == nullptr;
    }


    void printStack() {
        Node* temp = list.head;
        while (temp != nullptr) {
            cout << temp->data << " -> ";
            temp = temp->next;
        }
        cout << "NULL" << endl;
    }
};
