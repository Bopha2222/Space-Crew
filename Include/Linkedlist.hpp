#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include <iostream>
#include <string>
using namespace std;

// Node for LinkedList
class ListNode
{
public:
    string data;
    ListNode* next;

    ListNode(string value)
    {
        data = value;
        next = nullptr;
    }
};

// LinkedList class
class LinkedList
{
public:
    ListNode* head;

    LinkedList()
    {
        head = nullptr;
    }

    ~LinkedList()
    {
        clearList();
    }

    // Add node at end
    void add(const string &value)
    {
        ListNode* newNode = new ListNode(value);
        if (head == nullptr)
        {
            head = newNode;
            return;
        }

        ListNode* temp = head;
        while (temp->next != nullptr)
            temp = temp->next;
        temp->next = newNode;
    }

    // Print list
    void printList()
    {
        if (head == nullptr)
        {
            cout << "(empty)\n";
            return;
        }

        ListNode* temp = head;
        while (temp != nullptr)
        {
            cout << "- " << temp->data << endl;
            temp = temp->next;
        }
    }

    // Check if value exists
    bool contains(const string &value)
    {
        ListNode* temp = head;
        while (temp != nullptr)
        {
            if (temp->data == value) return true;
            temp = temp->next;
        }
        return false;
    }

    // Clear list
    void clearList()
    {
        ListNode* temp = head;
        while (temp != nullptr)
        {
            ListNode* nextNode = temp->next;
            delete temp;
            temp = nextNode;
        }
        head = nullptr;
    }

    // Get length of list
    int length()
    {
        int count = 0;
        ListNode* temp = head;
        while (temp != nullptr)
        {
            count++;
            temp = temp->next;
        }
        return count;
    }

    // Get data at index (0-based)
    string getAt(int index)
    {
        if (index < 0) return "";
        int i = 0;
        ListNode* temp = head;
        while (temp != nullptr)
        {
            if (i == index) return temp->data;
            temp = temp->next;
            i++;
        }
        return ""; // invalid index
    }

    // Remove node at index (0-based)
    void removeAt(int index)
    {
        if (head == nullptr) return;
        if (index < 0) return;

        if (index == 0)
        {
            ListNode* toDelete = head;
            head = head->next;
            delete toDelete;
            return;
        }

        ListNode* temp = head;
        int i = 0;
        while (temp->next != nullptr && i < index - 1)
        {
            temp = temp->next;
            i++;
        }

        if (temp->next == nullptr) return; // index out of range

        ListNode* toDelete = temp->next;
        temp->next = temp->next->next;
        delete toDelete;
    }

    // Remove first node that matches value (convenience wrapper)
    void remove(const string &value)
    {
        if (head == nullptr) return;

        // if head matches
        if (head->data == value)
        {
            ListNode* toDelete = head;
            head = head->next;
            delete toDelete;
            return;
        }

        // find node before the one to remove
        ListNode* temp = head;
        while (temp->next != nullptr && temp->next->data != value)
            temp = temp->next;

        if (temp->next == nullptr) return; // not found

        ListNode* toDelete = temp->next;
        temp->next = temp->next->next;
        delete toDelete;
    }

    // Get index of a value (first occurrence), -1 if not found
    int indexOf(const string &value)
    {
        int index = 0;
        ListNode* temp = head;
        while (temp != nullptr)
        {
            if (temp->data == value) return index;
            temp = temp->next;
            index++;
        }
        return -1; // not found
    }
};

#endif