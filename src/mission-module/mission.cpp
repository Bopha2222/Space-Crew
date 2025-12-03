#include "mission.hpp"

MissionManager::MissionManager() {
    srand((unsigned)time(0));
}

void MissionManager::assignTask(Player &player) {
    int index;
    int taskID;

    do {
        index = rand() % 10; 
        taskID = allMissionIDs[index];
    } while (player.taskList.hasValue(taskID));
    player.addTask(taskID);
    cout << "Assigned task " << taskID << ": " 
         << allMissionDescriptions[index] 
         << " to " << player.getName() << endl;
}

bool MissionManager::isTaskCompleted(Player &player, int taskID) {
    return player.missionHistory.hasValue(taskID);
}

void MissionManager::printPlayerTasks(Player &player) {
    cout << player.getName() << "'s tasks: ";
    player.taskList.printList();
}

void MissionManager::printPlayerStatus(Player &player) {
    cout << "===== PLAYER INFO =====" << endl;
    cout << "Name: " << player.getName() << endl;
    cout << "Role: " << (player.getRole() == ROLE_IMPOSTER ? "Imposter" : "Crewmate") << endl;
    cout << "Alive: " << (player.isAlive() ? "Yes" : "No") << endl;

    cout << "Tasks: ";
    player.taskList.printList();

    cout << "Mission History: ";
    player.missionHistory.printList();

    cout << "Action Queue: ";
    player.actionQueue.printQueue();

    cout << "Sabotages: ";
    player.sabotageStack.printStack();

    cout << "========================" << endl;
}
