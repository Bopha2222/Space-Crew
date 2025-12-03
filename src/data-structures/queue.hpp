#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <iostream>
using namespace std;

class QueueNode {
public:
    int data;
    QueueNode* next;

    QueueNode(int value) {
        data = value;
        next = nullptr;
    }
};

class Queue {
private:
    QueueNode* frontNode;
    QueueNode* rearNode;

public:
    Queue() {
        frontNode = nullptr;
        rearNode = nullptr;
    }

    ~Queue() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    bool isEmpty() {
        return frontNode == nullptr;
    }

    void enqueue(int value) {
        QueueNode* newNode = new QueueNode(value);

        if (rearNode == nullptr) {
            frontNode = rearNode = newNode;
            return;
        }

        rearNode->next = newNode;
        rearNode = newNode;
    }

    int dequeue() {
        if (isEmpty()) return -1;

        QueueNode* del = frontNode;
        int value = del->data;

        frontNode = frontNode->next;
        if (frontNode == nullptr) {
            rearNode = nullptr;
        }

        delete del;
        return value;
    }

    void printQueue() {
        QueueNode* temp = frontNode;
        while (temp != nullptr) {
            cout << temp->data;
            if (temp->next != nullptr) cout << " -> ";
            temp = temp->next;
        }
        cout << endl;
    }

};

#endif