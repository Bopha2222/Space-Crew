#ifndef TASK_MANAGER_HPP
#define TASK_MANAGER_HPP

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include "LinkedList.hpp"

using namespace std;

class TaskManager
{
public:
    LinkedList tasks;
    LinkedList passwords;
    LinkedList rooms;
    
    static const int TOTAL_TASK_ROOMS = 10; // Rooms 1-10 (Room 0 is Meeting)

    TaskManager()
    {
        srand(time(NULL));
        loadTasks();
    }

    void loadTasks()
    {
        tasks.clearList();
        passwords.clearList();
        rooms.clearList();

        ifstream taskFile("task.csv");
        ifstream pwFile("taskpw.csv");
        ifstream roomFile("taskroom.csv");

        string taskLine, pwLine, roomLine;

        // Skip header lines
        getline(taskFile, taskLine); // Skip "TaskName"
        getline(pwFile, pwLine);     // Skip "Password"
        getline(roomFile, roomLine); // Skip "RoomLocation"

        while (getline(taskFile, taskLine))
        {
            if (!taskLine.empty())
            {
                tasks.add(taskLine);
            }
        }

        while (getline(pwFile, pwLine))
        {
            if (!pwLine.empty())
            {
                passwords.add(pwLine);
            }
        }

        while (getline(roomFile, roomLine))
        {
            if (!roomLine.empty())
            {
                rooms.add(roomLine);
            }
        }

        taskFile.close();
        pwFile.close();
        roomFile.close();
        
        // If room assignments missing or mismatched, auto-assign
        if (rooms.length() != tasks.length())
        {
            cout << "Room assignments mismatched! Auto-balancing...\n";
            redistributeRooms();
        }
    }

    void saveTasks()
    {
        ofstream taskFile("task.csv");
        ofstream pwFile("taskpw.csv");
        ofstream roomFile("taskroom.csv");

        // Write headers
        taskFile << "TaskName\n";
        pwFile << "Password\n";
        roomFile << "RoomLocation\n";

        ListNode* taskNode = tasks.head;
        ListNode* pwNode = passwords.head;
        ListNode* roomNode = rooms.head;

        while (taskNode != nullptr && pwNode != nullptr && roomNode != nullptr)
        {
            taskFile << taskNode->data << "\n";
            pwFile << pwNode->data << "\n";
            roomFile << roomNode->data << "\n";

            taskNode = taskNode->next;
            pwNode = pwNode->next;
            roomNode = roomNode->next;
        }

        taskFile.close();
        pwFile.close();
        roomFile.close();
    }

    // Smart room assignment: distributes tasks evenly across rooms
    void redistributeRooms()
    {
        rooms.clearList();
        
        int totalTasks = tasks.length();
        if (totalTasks == 0) return;
        
        // Distribute tasks across rooms 1-10
        for (int i = 0; i < totalTasks; i++)
        {
            int roomNum = (i % TOTAL_TASK_ROOMS) + 1; // 1-10
            rooms.add("Room " + to_string(roomNum));
        }
        
        saveTasks();
    }

    void addTask(string taskName, string password)
    {
        // Add to lists
        tasks.add(taskName);
        passwords.add(password);
        
        // Smart room assignment: find room with fewest tasks
        int roomCounts[TOTAL_TASK_ROOMS + 1] = {0}; // Index 0 unused, 1-10 for rooms
        
        ListNode* roomNode = rooms.head;
        while (roomNode != nullptr)
        {
            // Extract room number from "Room X"
            string roomStr = roomNode->data;
            int roomNum = 0;
            for (int i = 0; i < roomStr.length(); i++)
            {
                if (roomStr[i] >= '0' && roomStr[i] <= '9')
                {
                    roomNum = roomNum * 10 + (roomStr[i] - '0');
                }
            }
            if (roomNum >= 1 && roomNum <= TOTAL_TASK_ROOMS)
            {
                roomCounts[roomNum]++;
            }
            roomNode = roomNode->next;
        }
        
        // Find room with minimum tasks
        int minRoom = 1;
        int minCount = roomCounts[1];
        for (int i = 2; i <= TOTAL_TASK_ROOMS; i++)
        {
            if (roomCounts[i] < minCount)
            {
                minCount = roomCounts[i];
                minRoom = i;
            }
        }
        
        // Assign new task to room with fewest tasks
        rooms.add("Room " + to_string(minRoom));
        
        saveTasks();
        cout << "Task added successfully! Assigned to Room " << minRoom 
             << " (now has " << (minCount + 1) << " tasks)\n";
    }

    void deleteTask(string taskName)
    {
        int idx = tasks.indexOf(taskName);
        if (idx == -1)
        {
            cout << "Task not found!\n";
            return;
        }

        string roomName = rooms.getAt(idx);
        
        tasks.removeAt(idx);
        passwords.removeAt(idx);
        rooms.removeAt(idx);
        
        saveTasks();
        cout << "Task deleted successfully from " << roomName << "!\n";
        
        // Show updated room distribution
        showRoomDistribution();
    }

    void updateTask(string oldTaskName, string newTaskName, string newPassword)
    {
        int idx = tasks.indexOf(oldTaskName);
        if (idx == -1)
        {
            cout << "Task not found!\n";
            return;
        }

        // Get current room (keep it the same)
        string currentRoom = rooms.getAt(idx);
        
        // Remove old
        tasks.removeAt(idx);
        passwords.removeAt(idx);
        rooms.removeAt(idx);

        // Insert new at same position
        LinkedList newTasks, newPasswords, newRooms;
        
        ListNode* taskNode = tasks.head;
        ListNode* pwNode = passwords.head;
        ListNode* roomNode = rooms.head;

        int count = 0;
        
        // Copy everything before index
        while (count < idx && taskNode != nullptr)
        {
            newTasks.add(taskNode->data);
            newPasswords.add(pwNode->data);
            newRooms.add(roomNode->data);
            
            taskNode = taskNode->next;
            pwNode = pwNode->next;
            roomNode = roomNode->next;
            count++;
        }
        
        // Insert updated task
        newTasks.add(newTaskName);
        newPasswords.add(newPassword);
        newRooms.add(currentRoom);
        
        // Copy rest
        while (taskNode != nullptr)
        {
            newTasks.add(taskNode->data);
            newPasswords.add(pwNode->data);
            newRooms.add(roomNode->data);
            
            taskNode = taskNode->next;
            pwNode = pwNode->next;
            roomNode = roomNode->next;
        }

        tasks = newTasks;
        passwords = newPasswords;
        rooms = newRooms;

        saveTasks();
        cout << "Task updated successfully! Still in " << currentRoom << "\n";
    }

    void showRoomDistribution()
    {
        int roomCounts[TOTAL_TASK_ROOMS + 1] = {0};
        string roomTasks[TOTAL_TASK_ROOMS + 1][20]; // Max 20 tasks per room
        int roomTaskCounts[TOTAL_TASK_ROOMS + 1] = {0};
        
        ListNode* taskNode = tasks.head;
        ListNode* roomNode = rooms.head;
        
        while (taskNode != nullptr && roomNode != nullptr)
        {
            string roomStr = roomNode->data;
            int roomNum = 0;
            for (int i = 0; i < roomStr.length(); i++)
            {
                if (roomStr[i] >= '0' && roomStr[i] <= '9')
                {
                    roomNum = roomNum * 10 + (roomStr[i] - '0');
                }
            }
            
            if (roomNum >= 1 && roomNum <= TOTAL_TASK_ROOMS)
            {
                roomCounts[roomNum]++;
                if (roomTaskCounts[roomNum] < 20)
                {
                    roomTasks[roomNum][roomTaskCounts[roomNum]] = taskNode->data;
                    roomTaskCounts[roomNum]++;
                }
            }
            
            taskNode = taskNode->next;
            roomNode = roomNode->next;
        }
        
        cout << "\n========================================\n";
        cout << "       ROOM DISTRIBUTION\n";
        cout << "========================================\n";
        
        for (int i = 1; i <= TOTAL_TASK_ROOMS; i++)
        {
            cout << "Room " << i << ": " << roomCounts[i] << " tasks";
            if (roomCounts[i] > 0)
            {
                cout << "\n";
                for (int j = 0; j < roomTaskCounts[i]; j++)
                {
                    cout << "  - " << roomTasks[i][j] << "\n";
                }
            }
            else
            {
                cout << " (empty)\n";
            }
        }
        cout << "========================================\n\n";
    }

    void rebalanceAllRooms()
    {
        cout << "Rebalancing all tasks across rooms...\n";
        redistributeRooms();
        cout << "Done!\n";
        showRoomDistribution();
    }

    void listAllTasks()
    {
        cout << "\n========================================\n";
        cout << "           ALL TASKS\n";
        cout << "========================================\n";
        
        ListNode* taskNode = tasks.head;
        ListNode* pwNode = passwords.head;
        ListNode* roomNode = rooms.head;

        int index = 1;
        while (taskNode != nullptr && pwNode != nullptr && roomNode != nullptr)
        {
            cout << index << ". " << taskNode->data 
                 << "\n   Password: " << pwNode->data 
                 << "\n   Location: " << roomNode->data << "\n\n";
            
            taskNode = taskNode->next;
            pwNode = pwNode->next;
            roomNode = roomNode->next;
            index++;
        }
        
        cout << "Total Tasks: " << tasks.length() << "\n";
        cout << "========================================\n\n";
    }

    string getTaskRoom(string taskName)
    {
        int idx = tasks.indexOf(taskName);
        if (idx == -1)
        {
            return "Unknown";
        }
        return rooms.getAt(idx);
    }

    string getTaskPassword(string taskName)
    {
        int idx = tasks.indexOf(taskName);
        if (idx == -1)
        {
            return "";
        }
        return passwords.getAt(idx);
    }

    void assignRandomTasks(LinkedList &assignedTasks, int numTasks)
    {
        int total = tasks.length();
        if (total == 0)
        {
            return;
        }

        LinkedList temp;
        for (int i = 0; i < total; i++)
        {
            temp.add(tasks.getAt(i));
        }

        int attempts = 0;
        while (assignedTasks.length() < numTasks && temp.length() > 0 && attempts < total * 3)
        {
            int r = rand() % temp.length();
            string candidate = temp.getAt(r);

            if (!assignedTasks.contains(candidate))
            {
                assignedTasks.add(candidate);
            }

            temp.removeAt(r);
            attempts++;
        }
    }

    bool checkPassword(string typed, string taskName, int difficulty)
    {
        int idx = tasks.indexOf(taskName);
        if (idx == -1)
        {
            return false;
        }

        string correct = passwords.getAt(idx);

        if (difficulty == 1)
        {
            if (typed.size() != correct.size())
            {
                return false;
            }

            for (int i = 0; i < typed.size(); i++)
            {
                char typedChar = typed[i];
                char correctChar = correct[i];

                if (typedChar >= 'A' && typedChar <= 'Z')
                {
                    typedChar = typedChar + 32;
                }

                if (correctChar >= 'A' && correctChar <= 'Z')
                {
                    correctChar = correctChar + 32;
                }

                if (typedChar != correctChar)
                {
                    return false;
                }
            }

            return true;
        }
        else
        {
            return typed == correct;
        }
    }

    void completeTaskPlayer(LinkedList &assignedTasks, string taskName, string playerName)
    {
        if (!assignedTasks.contains(taskName))
        {
            return;
        }

        assignedTasks.remove(taskName);
        cout << playerName << " completed task: " << taskName << "\n";
    }

    void completeTaskBot(LinkedList &assignedTasks, string taskName, string botName)
    {
        if (!assignedTasks.contains(taskName))
        {
            return;
        }

        assignedTasks.remove(taskName);
    }
};

#endif