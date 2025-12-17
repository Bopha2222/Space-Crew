
#ifndef BOT_IMPL_HPP
#define BOT_IMPL_HPP

#include "bot.hpp"
#include "player.hpp"
#include "task_manager.hpp"

void Bot::takeTurn(Player &player, Bot bots[], int botCount, GameMap &map, TaskManager &taskManager, int roundNumber, int killCooldown)
{
    if (isDead)
    {
        return;
    }

    currentRoomIndex = map.getRandomNeighbor(currentRoomIndex);

    if (!isImpostor)
    {
        if (assignedTasks.head != nullptr)
        {
            int randomChance = rand() % 2;
            if (randomChance == 0)
            {
                string taskToDo = assignedTasks.head->data;
                taskManager.completeTaskBot(assignedTasks, taskToDo, name);
            }
        }
    }
    else
    {
        if (!player.isDead && currentRoomIndex == player.currentRoomIndex)
        {
            if ((roundNumber - lastKillRound) >= killCooldown)
            {
                player.isDead = true;
                lastKillRound = roundNumber;
                cout << name << " (Impostor) killed " << player.name << "!\n";
            }
        }

        for (int i = 0; i < botCount; i++)
        {
            if (&bots[i] == this)
            {
                continue;
            }
            if (bots[i].isDead)
            {
                continue;
            }
            if (bots[i].isImpostor)
            {
                continue;
            }

            if (bots[i].currentRoomIndex == currentRoomIndex)
            {
                if ((roundNumber - lastKillRound) >= killCooldown)
                {
                    bots[i].isDead = true;
                    lastKillRound = roundNumber;
                    cout << name << " (Impostor) killed " << bots[i].name << "!\n";
                    break;
                }
            }
        }
    }
}

#endif
