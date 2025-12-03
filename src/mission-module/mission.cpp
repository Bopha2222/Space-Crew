#include "mission.hpp"


void assignTask(Player &player, int taskID) {
    player.addTask(taskID);
    cout << "Assigned task " << taskID << " to " << player.getName() << endl;
}


bool isTaskCompleted(Player &player, int taskID) {
    return player.missionHistory.hasValue(taskID);
}


void printPlayerTasks(Player &player) {
    cout << player.getName() << "'s tasks: ";
    player.taskList.printList();
}


void printPlayerStatus(Player &player) {
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
