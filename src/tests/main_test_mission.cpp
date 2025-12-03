#include "../player/player.hpp"
#include "../mission-module/mission.hpp"


void assignTask(Player &player, int taskID) {
    player.addTask(taskID);
    cout << "Assigned task " << taskID << " to " << player.getName() << endl;
}

void completeTask(Player &player, int taskID) {
    player.completeTask(taskID);
}

void printPlayerStatus(Player &player) {
    cout << "===== PLAYER INFO =====" << endl;
    cout << "Name: " << player.getName() << endl;
    cout << "Color: " << player.getRole() << " Role: " 
         << (player.getRole() == ROLE_IMPOSTER ? "Imposter" : "Crewmate") << endl;
    cout << "Alive: " << (player.isAlive() ? "Yes" : "No") << endl;

    cout << "Tasks: ";
    player.taskList.printList();

    cout << "Mission History: ";
    player.missionHistory.printList();
    cout << "========================" << endl;
}

int main() {

    Player red("Red", "Red", ROLE_CREWMATE);
    Player blue("Blue", "Blue", ROLE_IMPOSTER);


    assignTask(red, 101);
    assignTask(red, 102);
    assignTask(red, 103);

    assignTask(blue, 201);
    assignTask(blue, 202);


    printPlayerStatus(red);
    printPlayerStatus(blue);


    completeTask(red, 101);
    completeTask(blue, 201);


    printPlayerStatus(red);
    printPlayerStatus(blue);

    return 0;
}
