
#ifndef BOT_HPP
#define BOT_HPP

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include "LinkedList.hpp"
#include "map.hpp"

using namespace std;

class Player;
class TaskManager;

class Bot
{
public:
    string name;
    bool isDead;
    bool isImpostor;
    int currentRoomIndex;
    int lastKillRound;

    LinkedList assignedTasks;

    Bot()
    {
        name = "Bot";
        isDead = false;
        isImpostor = false;
        currentRoomIndex = 0;
        lastKillRound = -5;
    }

    void takeTurn(Player &player, Bot bots[], int botCount, GameMap &map, TaskManager &taskManager, int roundNumber, int killCooldown);
};

#endif
