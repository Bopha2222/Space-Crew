#ifndef STACK_HPP
#define STACK_HPP

#include <iostream>
#include <string>
using namespace std;

class StackNode {
public:
    string data;
    StackNode* next;

    StackNode(string value) {
        data = value;
        next = nullptr;
    }
};

class Stack {
private:
    StackNode* topNode;

public:
    Stack() {
        topNode = nullptr;
    }

    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }

    bool isEmpty() {
        return topNode == nullptr;
    }

    void push(string value) {
        StackNode* newNode = new StackNode(value);
        newNode->next = topNode;
        topNode = newNode;
    }

    string pop() {
        if (isEmpty()) return "";

        StackNode* del = topNode;
        string value = del->data;

        topNode = topNode->next;
        delete del;

        return value;
    }

    void printStack() {
        StackNode* temp = topNode;
        while (temp != nullptr) {
            cout << temp->data;
            if (temp->next != nullptr) cout << " -> ";
            temp = temp->next;
        }
        cout << endl;
    }

};

#endif