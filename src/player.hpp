#pragma once
#include <string>
#include <iostream>
using namespace std;

class Player {
public:
    string name;
    bool taskCompleted;
    bool isHuman;

    Player() {} // default constructor needed for array

    Player(string playerName, bool human) {
        name = playerName;
        taskCompleted = false;
        isHuman = human;
    }

    void performTask() {
        if (isHuman)
            cout << name << " is performing a task..." << endl;
        else
            cout << name << " (bot) is performing a task..." << endl;
        taskCompleted = true;
    }

    void showStatus() {
        cout << name << " - Task completed: "
             << (taskCompleted ? "Yes" : "No") << endl;
    }

    string getName() {
        return name;
    }
};
