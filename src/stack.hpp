#ifndef STACK_HPP
#define STACK_HPP

#include <iostream>
#include <string>
using namespace std;

// Node for Stack
struct StackNode
{
    string data;
    StackNode* next;

    StackNode(string val)
    {
        data = val;
        next = nullptr;
    }
};

// Stack class (LIFO)
class Stack
{
public:
    StackNode* top;

    Stack()
    {
        top = nullptr;
    }

    ~Stack()
    {
        clear();
    }

    // Push a value onto the stack
    void push(string val)
    {
        StackNode* newNode = new StackNode(val);
        newNode->next = top;
        top = newNode;
    }

    // Pop the top value from the stack
    string pop()
    {
        if (top == nullptr) return "";

        StackNode* temp = top;
        string value = top->data;
        top = top->next;
        delete temp;
        return value;
    }

    // Peek at the top value
    string peek()
    {
        if (top == nullptr) return "";
        return top->data;
    }

    // Check if stack is empty
    bool isEmpty()
    {
        return top == nullptr;
    }

    // Print all values in stack (top to bottom)
    void printStack()
    {
        StackNode* temp = top;
        int index = 1;
        while (temp != nullptr)
        {
            cout << index << ". " << temp->data << endl;
            temp = temp->next;
            index++;
        }
    }

    // Clear the stack
    void clear()
    {
        while (top != nullptr)
        {
            StackNode* temp = top;
            top = top->next;
            delete temp;
        }
    }

    // Get size of stack
    int size()
    {
        int count = 0;
        StackNode* temp = top;
        while (temp != nullptr)
        {
            count++;
            temp = temp->next;
        }
        return count;
    }
};

#endif
