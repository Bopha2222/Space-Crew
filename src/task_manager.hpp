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

    TaskManager()
    {
        srand(time(NULL));
        loadTasks();
    }

    void loadTasks()
    {
        tasks.clearList();
        passwords.clearList();

        ifstream taskFile("task.csv");
        ifstream pwFile("taskpw.csv");

        string taskLine;
        string pwLine;

        while (getline(taskFile, taskLine))
        {
            tasks.add(taskLine);
        }

        while (getline(pwFile, pwLine))
        {
            passwords.add(pwLine);
        }

        taskFile.close();
        pwFile.close();
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
        cout << playerName << " completed task: " << taskName << endl;
    }

    void completeTaskBot(LinkedList &assignedTasks, string taskName, string botName)
    {
        if (!assignedTasks.contains(taskName))
        {
            return;
        }

        assignedTasks.remove(taskName);
        cout << "Bot " << botName << " completed task: " << taskName << endl;
    }
};

#endif