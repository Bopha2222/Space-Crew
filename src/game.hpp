#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
#include "player.hpp"
#include "bot.hpp"
#include "map.hpp"
#include "task_manager.hpp"
#include "bot_impl.hpp"

using namespace std;

class Game
{
public:
    Player player;
    Bot bots[9];
    GameMap gameMap;
    TaskManager taskManager;

    int difficulty;
    int totalTasks;
    int killCooldown;

    bool gameOver;
    bool playerWon;
    int roundNumber;

    Game()
    {
        srand(time(NULL));
        difficulty = 1;
        totalTasks = 3;
        killCooldown = 5;
        gameOver = false;
        playerWon = false;
        roundNumber = 0;
    }

    void selectDifficulty()
    {
        int choice;
        cout << "Select Difficulty: 1. Easy 2. Medium 3. Hard\nChoice: ";
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input! Defaulting to Easy.\n";
            difficulty = 1;
            totalTasks = 3;
            killCooldown = 5;
            return;
        }

        cin.ignore();

        if (choice == 1)
        {
            difficulty = 1;
            totalTasks = 3;
            killCooldown = 5;
        }
        else if (choice == 2)
        {
            difficulty = 2;
            totalTasks = 4;
            killCooldown = 4;
        }
        else if (choice == 3)
        {
            difficulty = 3;
            totalTasks = 5;
            killCooldown = 3;
        }
        else
        {
            cout << "Invalid choice. Defaulting to Easy.\n";
            difficulty = 1;
            totalTasks = 3;
            killCooldown = 5;
        }
    }

    void assignTasks()
    {
        player.assignedTasks.clearList();
        taskManager.assignRandomTasks(player.assignedTasks, totalTasks);

        for (int i = 0; i < 9; i++)
        {
            bots[i].assignedTasks.clearList();
            taskManager.assignRandomTasks(bots[i].assignedTasks, totalTasks);
        }
    }

    void startGame()
    {
        gameOver = false;
        playerWon = false;
        roundNumber = 0;

        player.isDead = false;
        player.isImpostor = false;
        player.currentRoomIndex = 0;
        player.assignedTasks.clearList();
        player.missionHistory.clear();

        for (int i = 0; i < 9; i++)
        {
            bots[i].isDead = false;
            bots[i].isImpostor = false;
            bots[i].currentRoomIndex = 0;
            bots[i].lastKillRound = -killCooldown;
            bots[i].name = "Bot" + to_string(i + 1);
            bots[i].assignedTasks.clearList();
        }

        int impostorIndex = rand() % 10;
        if (impostorIndex == 0)
        {
            player.isImpostor = true;
            cout << "\n*** You are the IMPOSTOR! ***\n";
        }
        else
        {
            bots[impostorIndex - 1].isImpostor = true;
            cout << "\n*** You are a CREWMATE! Complete your tasks! ***\n";
        }

        assignTasks();

        cout << "\n--- Game Start ---\n";

        while (!gameOver)
        {
            roundNumber++;
            cout << "\n--- Round " << roundNumber << " ---\n";

            if (!player.isDead)
            {
                cout << "Your current room: " << gameMap.roomNames[player.currentRoomIndex] << endl;
                player.printAssignedTasks();

                cout << "Move using W/A/S/D, M move to Meeting Room and trigger meeting, T attempt task: ";
                char input;
                cin >> input;
                cin.ignore();

                if (input == 'M' || input == 'm')
                {
                    player.moveTo(0, gameMap);
                    triggerMeeting();
                }
                else if (input == 'T' || input == 't')
                {
                    if (player.isImpostor)
                    {
                        cout << "Impostors don't have tasks!\n";
                    }
                    else if (player.assignedTasks.head == nullptr)
                    {
                        cout << "No tasks to do.\n";
                    }
                    else
                    {
                        cout << "Type password for any assigned task: ";
                        string typed;
                        cin >> typed;
                        cin.ignore();

                        bool did = false;
                        ListNode* tmp = player.assignedTasks.head;
                        while (tmp != nullptr)
                        {
                            bool correct = taskManager.checkPassword(typed, tmp->data, difficulty);
                            if (correct)
                            {
                                taskManager.completeTaskPlayer(player.assignedTasks, tmp->data, player.name);
                                player.missionHistory.push(tmp->data);
                                did = true;
                                break;
                            }
                            tmp = tmp->next;
                        }
                        if (!did)
                        {
                            cout << "Incorrect password.\n";
                        }
                    }
                }
                else
                {
                    int neighbors[gameMap.roomCount];
                    int n = gameMap.getNeighbors(player.currentRoomIndex, neighbors);
                    int nextRoom = player.currentRoomIndex;

                    if ((input == 'W' || input == 'w') && n >= 1)
                    {
                        nextRoom = neighbors[0];
                    }
                    else if ((input == 'A' || input == 'a') && n >= 2)
                    {
                        nextRoom = neighbors[1];
                    }
                    else if ((input == 'S' || input == 's') && n >= 3)
                    {
                        nextRoom = neighbors[2];
                    }
                    else if ((input == 'D' || input == 'd') && n >= 4)
                    {
                        nextRoom = neighbors[3];
                    }

                    if (nextRoom != player.currentRoomIndex)
                    {
                        player.moveTo(nextRoom, gameMap);
                    }
                    else
                    {
                        cout << "Cannot move in that direction.\n";
                    }
                }
            }

            for (int i = 0; i < 9; i++)
            {
                bots[i].takeTurn(player, bots, 9, gameMap, taskManager, roundNumber, killCooldown);
            }

            checkWinCondition();
        }

        if (playerWon)
        {
            cout << "\n*** VICTORY! Crewmates win! ***\n";
        }
        else
        {
            cout << "\n*** DEFEAT! Impostors win! ***\n";
        }
    }

    void triggerMeeting()
    {
        cout << "\n--- Meeting Started ---\n";
        cout << "Press Enter to continue...";
        string dummy;
        getline(cin, dummy);

        string names[10];
        int candidateCount = 0;

        if (!player.isDead)
        {
            names[candidateCount] = player.name;
            candidateCount++;
        }

        for (int i = 0; i < 9; i++)
        {
            if (!bots[i].isDead)
            {
                names[candidateCount] = bots[i].name;
                candidateCount++;
            }
        }

        int votes[10];
        for (int i = 0; i < 10; i++)
        {
            votes[i] = 0;
        }

        for (int i = 0; i < 9; i++)
        {
            if (bots[i].isDead)
            {
                continue;
            }

            int r = rand() % (candidateCount + 1);
            if (r < candidateCount)
            {
                votes[r]++;
            }
        }

        cout << "Vote a player to eject by name or type 'skip': ";
        string v;
        getline(cin, v);

        if (v != "Skip" && v != "skip")
        {
            for (int i = 0; i < candidateCount; i++)
            {
                if (names[i] == v)
                {
                    votes[i]++;
                    break;
                }
            }
        }

        int maxVotes = 0;
        int maxIndex = -1;
        for (int i = 0; i < candidateCount; i++)
        {
            if (votes[i] > maxVotes)
            {
                maxVotes = votes[i];
                maxIndex = i;
            }
        }

        if (maxIndex == -1 || maxVotes == 0)
        {
            cout << "No one was ejected.\n";
        }
        else
        {
            string ejected = names[maxIndex];
            if (ejected == player.name)
            {
                player.isDead = true;
                cout << "Player " << player.name << " was ejected. ";
                if (player.isImpostor)
                {
                    cout << "They were an Impostor!\n";
                }
                else
                {
                    cout << "They were not an Impostor.\n";
                }
            }
            else
            {
                for (int i = 0; i < 9; i++)
                {
                    if (bots[i].name == ejected)
                    {
                        bots[i].isDead = true;
                        cout << "Bot " << bots[i].name << " was ejected. ";
                        if (bots[i].isImpostor)
                        {
                            cout << "They were an Impostor!\n";
                        }
                        else
                        {
                            cout << "They were not an Impostor.\n";
                        }
                        break;
                    }
                }
            }
        }

        cout << "--- Meeting Ended ---\n";
    }

    void checkWinCondition()
    {
        int aliveCrew = 0;
        int aliveImpostor = 0;

        if (!player.isDead)
        {
            if (player.isImpostor)
            {
                aliveImpostor++;
            }
            else
            {
                aliveCrew++;
            }
        }

        for (int i = 0; i < 9; i++)
        {
            if (!bots[i].isDead)
            {
                if (bots[i].isImpostor)
                {
                    aliveImpostor++;
                }
                else
                {
                    aliveCrew++;
                }
            }
        }

        bool allDone = true;
        if (!player.isImpostor && player.assignedTasks.head != nullptr)
        {
            allDone = false;
        }

        for (int i = 0; i < 9; i++)
        {
            if (!bots[i].isImpostor && bots[i].assignedTasks.head != nullptr)
            {
                allDone = false;
                break;
            }
        }

        if (allDone && aliveCrew > 0)
        {
            cout << "\nAll tasks completed! Crewmates win!\n";
            if (player.isImpostor)
            {
                playerWon = false;
            }
            else
            {
                playerWon = true;
            }
            gameOver = true;
            return;
        }

        if (aliveImpostor == 0)
        {
            cout << "\nAll impostors eliminated! Crewmates win!\n";
            if (player.isImpostor)
            {
                playerWon = false;
            }
            else
            {
                playerWon = true;
            }
            gameOver = true;
            return;
        }

        if (aliveImpostor >= aliveCrew)
        {
            cout << "\nImpostors equal or outnumber crewmates! Impostors win!\n";
            if (player.isImpostor)
            {
                playerWon = true;
            }
            else
            {
                playerWon = false;
            }
            gameOver = true;
        }
    }
};

#endif