#include "LinkedList.hpp"
#include "Stack.hpp"
#include "Queue.hpp"

class Player {
public:
    string name;
    bool isImpostor;

    LinkedList tasks;    
    Stack missionHistory; 
    Queue actionQueue;   

    
    Player(string playerName, bool impostor = false) {
        name = playerName;
        isImpostor = impostor;
    }

   
    void addTask(int taskID) {
        tasks.append(taskID);
    }

    
    void completeTask() {
        if (tasks.hasValue(tasks.head->data)) {  
            int task = tasks.head->data;
            missionHistory.push(task);
            tasks.remove(task);
            cout << name << " completed task " << task << endl;
        } else {
            cout << name << " has no tasks left!" << endl;
        }
    }

    
    void addAction(int actionID) {
        actionQueue.enqueue(actionID);
    }

   
    void performNextAction() {
        if (!actionQueue.isEmpty()) {
            int action = actionQueue.front();
            actionQueue.dequeue();
            cout << name << " performed action " << action << endl;
        } else {
            cout << name << " has no actions in queue!" << endl;
        }
    }

    
    void printTasks() {
        cout << name << "'s tasks: ";
        tasks.printList();
    }

    void printMissionHistory() {
        cout << name << "'s mission history: ";
        missionHistory.printStack();
    }

   
    void printActionQueue() {
        cout << name << "'s action queue: ";
        actionQueue.printQueue();
    }
};
