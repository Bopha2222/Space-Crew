#include "../data-structures/LinkedList.hpp"
#include "../data-structures/queue.hpp"
#include "../data-structures/stack.hpp"

// Role constants (no enums to keep it simple)
#define ROLE_CREWMATE 0
#define ROLE_IMPOSTER 1

class Player {
private:
    string name;
    string color;
    int role;              // 0 = Crewmate, 1 = Imposter
    bool alive;
    int emergencyMeetings;

public:
    LinkedList taskList;      
    LinkedList missionHistory;  
    Queue actionQueue;         
    Stack sabotageStack;        


    Player(string n, string c, int r) {
        name = n;
        color = c;
        role = r;
        alive = true;
        emergencyMeetings = 1; 
    }

    ~Player() {}


    string getName() { return name; }
    bool isAlive() { return alive; }
    int getRole() { return role; }


    void addTask(int taskID) {
        taskList.append(taskID);
    }

    void completeTask(int taskID) {
        if (taskList.hasValue(taskID)) {
            taskList.remove(taskID);
            missionHistory.append(taskID);
            cout << name << " completed task " << taskID << endl;
        } else {
            cout << name << " does not have task " << taskID << endl;
        }
    }


    void callEmergencyMeeting() {
        if (!alive) {
            cout << name << " cannot call a meeting (dead)." << endl;
            return;
        }
        if (emergencyMeetings > 0) {
            emergencyMeetings--;
            cout << name << " called an emergency meeting!" << endl;
        } else {
            cout << name << " has no emergency meetings left." << endl;
        }
    }

    void reportBody(Player &target) {
        if (!alive) {
            cout << name << " cannot report a body (dead)." << endl;
            return;
        }
        if (!target.isAlive()) {
            cout << name << " reported the body of " << target.getName() << "!" << endl;
        } else {
            cout << name << " cannot report " << target.getName() << " (still alive)." << endl;
        }
    }

 
    void kill(Player &target) {
        if (role != ROLE_IMPOSTER) {
            cout << name << " is not an imposter and cannot kill!" << endl;
            return;
        }
        if (!alive) {
            cout << name << " cannot kill (dead)." << endl;
            return;
        }
        if (!target.isAlive()) {
            cout << name << " cannot kill " << target.getName() << " (already dead)." << endl;
            return;
        }

        target.alive = false;
        cout << name << " has killed " << target.getName() << "!" << endl;
    }


    void sabotage(string type) {
        if (role != ROLE_IMPOSTER) {
            cout << name << " cannot sabotage (not an imposter)." << endl;
            return;
        }

        sabotageStack.push(type);
        cout << name << " initiated sabotage: " << type << endl;
    }

    void fixSabotage() {
        if (sabotageStack.isEmpty()) {
            cout << name << " has no sabotages to fix." << endl;
            return;
        }

        string fixed = sabotageStack.pop();
        cout << name << " fixed sabotage: " << fixed << endl;
    }


    void enqueueAction(int actionCode) {
        actionQueue.enqueue(actionCode);
    }

    void performAction() {
        if (actionQueue.isEmpty()) {
            cout << name << " has no actions in queue." << endl;
            return;
        }

        int action = actionQueue.dequeue();
        cout << name << " performed action " << action << endl;
    }


    void printStatus() {
        cout << "===== PLAYER INFO =====" << endl;
        cout << "Name: " << name << endl;
        cout << "Color: " << color << endl;
        cout << "Role: " << (role == ROLE_IMPOSTER ? "Imposter" : "Crewmate") << endl;
        cout << "Alive: " << (alive ? "Yes" : "No") << endl;
        cout << "Emergency Meetings: " << emergencyMeetings << endl;

        cout << "Tasks: ";
        taskList.printList();

        cout << "Mission History: ";
        missionHistory.printList();

        cout << "Action Queue: ";
        actionQueue.printQueue();

        cout << "Sabotages: ";
        sabotageStack.printStack();

        cout << "========================" << endl;
    }
};
