
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
#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <vector>
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
    int lastMeetingRound;

    Game()
    {
        srand(time(NULL));
        difficulty = 1;
        totalTasks = 3;
        killCooldown = 5;
        gameOver = false;
        playerWon = false;
        roundNumber = 0;
        lastMeetingRound = -30; // Start with -30 so first meeting is possible immediately
    }

    void selectDifficulty()
    {
        int choice;
        cout << "\nSelect Difficulty:\n";
        cout << "1. Easy (3 tasks, 5 round cooldown)\n";
        cout << "2. Medium (4 tasks, 4 round cooldown)\n";
        cout << "3. Hard (5 tasks, 3 round cooldown)\n";
        cout << "4. Custom\n";
        cout << "Choice: ";
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
        else if (choice == 4)
        {
            cout << "Enter number of tasks (1-8): ";
            cin >> totalTasks;
            if (totalTasks < 1) totalTasks = 1;
            if (totalTasks > 8) totalTasks = 8;

            cout << "Enter kill cooldown (1-10 rounds): ";
            cin >> killCooldown;
            if (killCooldown < 1) killCooldown = 1;
            if (killCooldown > 10) killCooldown = 10;

            difficulty = 1; // Custom defaults to easy difficulty for task completion
            cin.ignore();
        }
        else
        {
            cout << "Invalid choice. Defaulting to Easy.\n";
            difficulty = 1;
            totalTasks = 3;
            killCooldown = 5;
        }

        cout << "\nGame configured: " << totalTasks << " tasks, " 
             << killCooldown << " round kill cooldown\n";
    }

    void assignTasks()
    {
        player.assignedTasks.clearList();
        taskManager.assignRandomTasks(player.assignedTasks, totalTasks);

        for (int i = 0; i < 9; i++)
        {
            bots[i].assignedTasks.clearList();
            if (!bots[i].isImpostor)
            {
                taskManager.assignRandomTasks(bots[i].assignedTasks, totalTasks);
            }
        }
    }

    void printPlayerTasksWithDetails()
    {
        cout << "\n" << player.name << "'s Assigned Tasks:\n";
        
        if (player.assignedTasks.head == nullptr)
        {
            cout << "(All tasks completed!)\n";
            return;
        }

        ListNode* tmp = player.assignedTasks.head;
        int index = 1;
        while (tmp != nullptr)
        {
            string taskName = tmp->data;
            string room = taskManager.getTaskRoom(taskName);
            string password = taskManager.getTaskPassword(taskName);
            
            cout << index << ". " << taskName 
                 << " | Room: " << room 
                 << " | Password: " << password << "\n";
            
            tmp = tmp->next;
            index++;
        }
    }

    void startGame()
    {
        gameOver = false;
        playerWon = false;
        roundNumber = 0;
        lastMeetingRound = -30;

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

        // ALWAYS EXACTLY 1 IMPOSTOR (not 2)
        int impostorIndex = rand() % 10;
        if (impostorIndex == 0)
        {
            player.isImpostor = true;
            cout << "\n*** You are the IMPOSTOR! Eliminate all crewmates! ***\n";
        }
        else
        {
            bots[impostorIndex - 1].isImpostor = true;
            cout << "\n*** You are a CREWMATE! Complete your tasks to win! ***\n";
        }

        assignTasks();

        cout << "\n========================================\n";
        cout << "            GAME START\n";
        cout << "========================================\n";

        while (!gameOver)
        {
            roundNumber++;

            if (!player.isDead)
            {
                cout << "\n--- Round " << roundNumber << " ---\n";
                cout << "Current Room: " << gameMap.roomNames[player.currentRoomIndex] << "\n";
                
                if (!player.isImpostor)
                {
                    printPlayerTasksWithDetails();
                }
                else
                {
                    cout << "\n[YOU ARE THE IMPOSTOR]\n";
                    cout << "Kill cooldown: " << killCooldown << " rounds\n";
                    
                    // Show who's in the room
                    cout << "\nPlayers in this room:\n";
                    bool someoneHere = false;
                    for (int i = 0; i < 9; i++)
                    {
                        if (!bots[i].isDead && bots[i].currentRoomIndex == player.currentRoomIndex)
                        {
                            cout << "  - " << bots[i].name;
                            if (bots[i].isImpostor)
                            {
                                cout << " (Fellow Impostor)";
                            }
                            cout << "\n";
                            someoneHere = true;
                        }
                    }
                    if (!someoneHere)
                    {
                        cout << "  (No one else here)\n";
                    }
                }

                cout << "\nActions:\n";
                cout << "  W/A/S/D - Move to adjacent rooms\n";
                
                // Check if meeting is on cooldown (30 rounds)
                int meetingCooldownRemaining = 30 - (roundNumber - lastMeetingRound);
                if (meetingCooldownRemaining > 0)
                {
                    cout << "  M - Call Meeting (Cooldown: " << meetingCooldownRemaining << " rounds)\n";
                }
                else
                {
                    cout << "  M - Go to Meeting Room and call meeting\n";
                }
                
                if (!player.isImpostor)
                {
                    cout << "  T - Complete a task\n";
                }
                else
                {
                    cout << "  K - Kill someone in this room\n";
                }
                cout << "\nYour action: ";
                
                string inputLine;
                getline(cin, inputLine);
                
                if (inputLine.empty())
                {
                    cout << "No input received.\n";
                    continue;
                }
                
                char input = inputLine[0];

                if (input == 'M' || input == 'm')
                {
                    // Check if meeting cooldown has passed
                    if ((roundNumber - lastMeetingRound) < 30)
                    {
                        int remaining = 30 - (roundNumber - lastMeetingRound);
                        cout << "Meeting on cooldown! Wait " << remaining << " more rounds.\n";
                    }
                    else
                    {
                        player.moveTo(0, gameMap);
                        triggerMeeting();
                        lastMeetingRound = roundNumber; // Update last meeting time
                    }
                }
                else if (input == 'T' || input == 't')
                {
                    if (player.isImpostor)
                    {
                        cout << "Impostors don't have tasks!\n";
                    }
                    else if (player.assignedTasks.head == nullptr)
                    {
                        cout << "No tasks remaining!\n";
                    }
                    else
                    {
                        cout << "\nType password for the task you want to complete: ";
                        string typed;
                        getline(cin, typed);

                        bool completed = false;
                        ListNode* tmp = player.assignedTasks.head;
                        
                        while (tmp != nullptr)
                        {
                            if (taskManager.checkPassword(typed, tmp->data, difficulty))
                            {
                                taskManager.completeTaskPlayer(player.assignedTasks, tmp->data, player.name);
                                player.missionHistory.push(tmp->data);
                                completed = true;
                                break;
                            }
                            tmp = tmp->next;
                        }
                        
                        if (!completed)
                        {
                            cout << "Incorrect password or task not found!\n";
                        }
                    }
                }
                else if (input == 'K' || input == 'k')
                {
                    if (!player.isImpostor)
                    {
                        cout << "Only impostors can kill!\n";
                    }
                    else
                    {
                        // Check if enough rounds passed since last kill
                        static int lastPlayerKillRound = -1000; // Track player's last kill
                        
                        if (roundNumber - lastPlayerKillRound < killCooldown)
                        {
                            int remaining = killCooldown - (roundNumber - lastPlayerKillRound);
                            cout << "Kill on cooldown! Wait " << remaining << " more rounds.\n";
                        }
                        else
                        {
                            // Show ALL players in the room, mark impostors as red
                            cout << "\nPlayers in this room:\n";
                            
                            vector<int> targets;
                            bool foundAnyone = false;
                            
                            for (int i = 0; i < 9; i++)
                            {
                                if (!bots[i].isDead && bots[i].currentRoomIndex == player.currentRoomIndex)
                                {
                                    foundAnyone = true;
                                    
                                    if (bots[i].isImpostor)
                                    {
                                        // Show impostor in red - can't kill them
                                        cout << "  \033[1;31m[IMPOSTOR]\033[0m " << bots[i].name << " (Can't kill)\n";
                                    }
                                    else
                                    {
                                        // Show crewmate - can kill
                                        targets.push_back(i);
                                        cout << "  " << targets.size() << ". " << bots[i].name << "\n";
                                    }
                                }
                            }
                            
                            if (!foundAnyone)
                            {
                                cout << "  (No one else in this room)\n";
                            }
                            else if (targets.empty())
                            {
                                cout << "\n(Only fellow impostors here - can't kill them!)\n";
                            }
                            else
                            {
                                cout << "  0. Cancel\n";
                                cout << "\nChoose target to kill: ";
                                string choice;
                                getline(cin, choice);
                                
                                int targetNum = 0;
                                try {
                                    targetNum = stoi(choice);
                                } catch (...) {
                                    targetNum = 0;
                                }
                                
                                if (targetNum > 0 && targetNum <= targets.size())
                                {
                                    int botIndex = targets[targetNum - 1];
                                    bots[botIndex].isDead = true;
                                    lastPlayerKillRound = roundNumber;
                                    cout << "\n\033[1;31m*** You killed " << bots[botIndex].name << "! ***\033[0m\n";
                                }
                                else
                                {
                                    cout << "Cancelled.\n";
                                }
                            }
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
                        cout << "Cannot move in that direction!\n";
                    }
                }
            }

            // Bots take their turns
            for (int i = 0; i < 9; i++)
            {
                bots[i].takeTurn(player, bots, 9, gameMap, taskManager, roundNumber, killCooldown, difficulty);
            }

            checkWinCondition();
        }

        if (playerWon)
        {
            cout << "\n========================================\n";
            cout << "         VICTORY! YOU WIN!             \n";
            cout << "========================================\n";
        }
        else
        {
            cout << "\n========================================\n";
            cout << "         DEFEAT! YOU LOSE!             \n";
            cout << "========================================\n";
        }

        cout << "\nPress Enter to continue...";
        string dummy;
        getline(cin, dummy);
    }

    // Helper function for case-insensitive string comparison
    bool caseInsensitiveCompare(string str1, string str2)
    {
        if (str1.length() != str2.length())
            return false;
        
        for (size_t i = 0; i < str1.length(); i++)
        {
            char c1 = str1[i];
            char c2 = str2[i];
            
            // Convert to lowercase
            if (c1 >= 'A' && c1 <= 'Z')
                c1 = c1 + 32;
            if (c2 >= 'A' && c2 <= 'Z')
                c2 = c2 + 32;
            
            if (c1 != c2)
                return false;
        }
        return true;
    }

    void triggerMeeting()
    {
        cout << "\n========================================\n";
        cout << "          EMERGENCY MEETING\n";
        cout << "========================================\n";
        cout << "Press Enter to start voting...";
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

        cout << "\nAlive players:\n";
        
        if (player.isImpostor)
        {
            // If you're impostor, show who else is impostor in RED
            // First check if player is in the list
            bool playerInList = false;
            for (int i = 0; i < candidateCount; i++)
            {
                if (names[i] == player.name)
                {
                    playerInList = true;
                    cout << "  - " << names[i] << " \033[1;33m(YOU - IMPOSTOR)\033[0m\n";
                    break;
                }
            }
            
            // Then list bots
            for (int i = 0; i < 9; i++)
            {
                if (!bots[i].isDead)
                {
                    if (bots[i].isImpostor)
                    {
                        cout << "  - \033[1;31m" << bots[i].name << " [IMPOSTOR]\033[0m\n";
                    }
                    else
                    {
                        cout << "  - " << bots[i].name << "\n";
                    }
                }
            }
        }
        else
        {
            // If you're crewmate, just list everyone normally
            for (int i = 0; i < candidateCount; i++)
            {
                if (names[i] == player.name)
                {
                    cout << "  - " << names[i] << " (YOU)\n";
                }
                else
                {
                    cout << "  - " << names[i] << "\n";
                }
            }
        }

        cout << "\nVote for someone to eject (or type 'skip'): ";
        string v;
        getline(cin, v);

        // Case-insensitive check for skip
        bool isSkip = caseInsensitiveCompare(v, "skip");
        
        if (!isSkip && !v.empty())
        {
            bool foundMatch = false;
            for (int i = 0; i < candidateCount; i++)
            {
                // Case-insensitive name comparison
                if (caseInsensitiveCompare(names[i], v))
                {
                    votes[i]++;
                    foundMatch = true;
                    cout << "You voted for " << names[i] << ".\n";
                    break;
                }
            }
            
            if (!foundMatch)
            {
                cout << "Invalid name - vote not counted.\n";
            }
        }
        else if (isSkip)
        {
            cout << "You skipped voting.\n";
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
            cout << "\nNo one was ejected.\n";
        }
        else
        {
            string ejected = names[maxIndex];
            if (ejected == player.name)
            {
                player.isDead = true;
                cout << "\n" << player.name << " was ejected.\n";
                cout << (player.isImpostor ? "They were an Impostor!\n" : "They were not an Impostor.\n");
            }
            else
            {
                for (int i = 0; i < 9; i++)
                {
                    if (bots[i].name == ejected)
                    {
                        bots[i].isDead = true;
                        cout << "\n" << bots[i].name << " was ejected.\n";
                        cout << (bots[i].isImpostor ? "They were an Impostor!\n" : "They were not an Impostor.\n");
                        break;
                    }
                }
            }
        }

        cout << "========================================\n";
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

        bool allTasksDone = true;
        if (!player.isImpostor && player.assignedTasks.head != nullptr)
        {
            allTasksDone = false;
        }

        for (int i = 0; i < 9; i++)
        {
            if (!bots[i].isImpostor && bots[i].assignedTasks.head != nullptr)
            {
                allTasksDone = false;
                break;
            }
        }

        if (allTasksDone && aliveCrew > 0)
        {
            cout << "\n*** All tasks completed! Crewmates win! ***\n";
            playerWon = !player.isImpostor;
            gameOver = true;
            return;
        }

        if (aliveImpostor == 0)
        {
            cout << "\n*** All impostors eliminated! Crewmates win! ***\n";
            playerWon = !player.isImpostor;
            gameOver = true;
            return;
        }

        if (aliveImpostor >= aliveCrew)
        {
            cout << "\n*** Impostors equal or outnumber crewmates! Impostors win! ***\n";
            playerWon = player.isImpostor;
            gameOver = true;
        }
    }
};

#endif

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
