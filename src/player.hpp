#pragma once
#include <string>
#include <iostream>
#include "mission.hpp"
using namespace std;

class Player {
public:
    string name;
    bool taskCompleted;
    bool isHuman;

    Mission assignedMissions[5]; // max 5 missions per player
    int missionCount;

    // Default constructor
    Player() {
        name = "";
        taskCompleted = false;
        isHuman = true;
        missionCount = 0;
    }

    // Parameterized constructor
    Player(string playerName, bool human) {
        name = playerName;
        taskCompleted = false;
        isHuman = human;
        missionCount = 0;
    }

    // Assign a mission to player
    void assignMission(Mission m) {
        if (missionCount < 5) {
            assignedMissions[missionCount] = m;
            missionCount++;
        }
    }

    // Display all missions for this player
    void showMissions() {
        cout << "\nMissions for " << name << ":\n";
        for (int i = 0; i < missionCount; i++) {
            cout << i + 1 << ". " << assignedMissions[i].name
                 << " - " << assignedMissions[i].description
                 << " | Completed: " << (assignedMissions[i].isCompleted ? "YES" : "NO")
                 << endl;
        }
    }

    // Complete a mission by index
    void completeMission(int index) {
        if (index >= 0 && index < missionCount) {
            assignedMissions[index].isCompleted = true;
            cout << name << " completed mission: " << assignedMissions[index].name << "!\n";
        }
    }
};
