#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <iostream>
#include <string>
using namespace std;

// Node for queue
class QueueNode
{
public:
    string data;
    QueueNode* next;

    QueueNode(string d)
    {
        data = d;
        next = nullptr;
    }
};

// Simple queue (FIFO) for actions or tasks
class Queue
{
public:
    QueueNode* front;
    QueueNode* rear;

    // Constructor
    Queue()
    {
        front = nullptr;
        rear = nullptr;
    }

    // Enqueue a new task/action
    void enqueue(string task)
    {
        QueueNode* newNode = new QueueNode(task);

        if (rear == nullptr) // empty queue
        {
            front = rear = newNode;
        }
        else
        {
            rear->next = newNode;
            rear = newNode;
        }
    }

    // Dequeue a task/action
    string dequeue()
    {
        if (front == nullptr)
            return ""; // empty queue

        QueueNode* temp = front;
        string value = front->data;
        front = front->next;

        if (front == nullptr) // queue is now empty
            rear = nullptr;

        delete temp;
        return value;
    }

    // Peek at the front task/action
    string peek()
    {
        if (front == nullptr)
            return "";
        return front->data;
    }

    // Check if queue is empty
    bool isEmpty()
    {
        return front == nullptr;
    }

    // Print all tasks/actions
    void printQueue()
    {
        if (front == nullptr)
        {
            cout << "Queue is empty.\n";
            return;
        }

        QueueNode* temp = front;
        int index = 1;
        cout << "Queue tasks:\n";
        while (temp != nullptr)
        {
            cout << index << ". " << temp->data << endl;
            temp = temp->next;
            index++;
        }
    }

    // Destructor
    ~Queue()
    {
        while (front != nullptr)
        {
            QueueNode* temp = front;
            front = front->next;
            delete temp;
        }
        rear = nullptr;
    }
};

#endif