#ifndef BOT_IMPL_HPP
#define BOT_IMPL_HPP

#include "bot.hpp"
#include "player.hpp"
#include "task_manager.hpp"

void Bot::takeTurn(Player &player, Bot bots[], int botCount, GameMap &map, TaskManager &taskManager, int roundNumber, int killCooldown, int difficulty)
{
    if (isDead)
    {
        return;
    }

    // Move to a random neighbor
    currentRoomIndex = map.getRandomNeighbor(currentRoomIndex);

    if (!isImpostor)
    {
        // Crewmate bot: try to complete tasks if in the right room
        if (assignedTasks.head != nullptr)
        {
            // Check each assigned task to see if any match current room
            ListNode* tmp = assignedTasks.head;
            while (tmp != nullptr)
            {
                string taskName = tmp->data;
                string taskRoom = taskManager.getTaskRoom(taskName);
                string currentRoom = map.roomNames[currentRoomIndex];
                
                // If bot is in the correct room for this task
                if (taskRoom == currentRoom)
                {
                    // Chance to complete task based on difficulty
                    // Easy (1): 80% chance (4/5)
                    // Medium (2): 60% chance (3/5) 
                    // Hard (3): 40% chance (2/5)
                    int threshold = 5;
                    if (difficulty == 1) threshold = 5; // 4/5 = 80%
                    else if (difficulty == 2) threshold = 6; // 3/6 = 50% (adjusted)
                    else if (difficulty == 3) threshold = 8; // 3/8 = 37.5%
                    
                    int randomChance = rand() % threshold;
                    if ((difficulty == 1 && randomChance < 4) ||
                        (difficulty == 2 && randomChance < 3) ||
                        (difficulty == 3 && randomChance < 3))
                    {
                        taskManager.completeTaskBot(assignedTasks, taskName, name);
                        break; // Complete only one task per turn
                    }
                }
                tmp = tmp->next;
            }
        }
    }
    else
    {
        // Impostor bot: try to kill someone (only if cooldown is ready)
        if ((roundNumber - lastKillRound) >= killCooldown)
        {
            bool killedSomeone = false;
            
            // First check if player is in same room
            if (!player.isDead && currentRoomIndex == player.currentRoomIndex)
            {
                player.isDead = true;
                lastKillRound = roundNumber;
                cout << name << " (Impostor) killed " << player.name << "!\n";
                killedSomeone = true;
            }
            
            // Only check bots if haven't killed player
            if (!killedSomeone)
            {
                for (int i = 0; i < botCount; i++)
                {
                    if (&bots[i] == this)
                    {
                        continue; // Skip self
                    }
                    if (bots[i].isDead)
                    {
                        continue; // Skip dead bots
                    }
                    if (bots[i].isImpostor)
                    {
                        continue; // Don't kill fellow impostor
                    }

                    // If bot is in same room
                    if (bots[i].currentRoomIndex == currentRoomIndex)
                    {
                        bots[i].isDead = true;
                        lastKillRound = roundNumber;
                        cout << name << " (Impostor) killed " << bots[i].name << "!\n";
                        killedSomeone = true;
                        break; // Kill only ONE person per turn
                    }
                }
            }
        }
    }
}

#endif