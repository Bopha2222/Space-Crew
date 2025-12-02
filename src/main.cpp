#include "LinkedList.hpp"
#include "Queue.hpp"
#include <iostream>
using namespace std;

int main() {
    // Test LinkedList
    LinkedList list;
    list.append(5);
    list.append(10);
    list.append(15);
    list.printList(); 
    list.remove(10);
    list.printList(); 
    cout << "Has 5? " << list.hasValue(5) << endl;  
    cout << "Has 10? " << list.hasValue(10) << endl; 


    // Test Queue
    Queue q;
    q.enqueue(100);
    q.enqueue(200);
    q.enqueue(300);
    q.printQueue(); 
    q.dequeue();
    q.printQueue(); 
    cout << "Front: " << q.front() << endl; 

    return 0;
}
