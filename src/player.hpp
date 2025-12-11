#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <iostream>
#include <string>
#include "LinkedList.hpp"
#include "Stack.hpp"
#include "map.hpp"

using namespace std;

class Player
{
public:
    string name;
    bool isDead;
    bool isImpostor;
    int currentRoomIndex;

    LinkedList assignedTasks; // tasks assigned to player
    Stack missionHistory;     // completed tasks

    // Constructor
    Player(string playerName = "Player")
    {
        name = playerName;
        isDead = false;
        isImpostor = false;
        currentRoomIndex = 0; // start at meeting room
    }

    // Move player to a neighbor chosen by a mapping (W/A/S/D)
    // Note: Game controls exact neighbor choice; Player just updates index here.
    void moveTo(int nextRoom, GameMap &map)
    {
        if (nextRoom < 0 || nextRoom >= map.roomCount) return;
        cout << name << " moved from " << map.roomNames[currentRoomIndex]
             << " to " << map.roomNames[nextRoom] << endl;
        currentRoomIndex = nextRoom;
    }

    // Complete a task (remove from assignedTasks, push to missionHistory)
    void completeTask(const string &task)
    {
        if (!assignedTasks.contains(task))
        {
            cout << "Task not found or already completed.\n";
            return;
        }
        assignedTasks.remove(task);
        missionHistory.push(task);
        cout << name << " completed task: " << task << endl;
    }

    // Print assigned tasks
    void printAssignedTasks()
    {
        cout << "\n" << name << "'s Assigned Tasks:\n";
        assignedTasks.printList();
    }

    // Print completed tasks
    void printCompletedTasks()
    {
        cout << "\n" << name << "'s Completed Tasks:\n";
        missionHistory.printStack();
    }

    // Check if all tasks completed
    bool allTasksCompleted()
    {
        return assignedTasks.head == nullptr;
    }
};

#endif
