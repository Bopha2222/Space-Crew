#include "../mission-module/mission.hpp"

int main() {
    MissionManager manager;

    Player red("Red", "Red", ROLE_CREWMATE);
    Player blue("Blue", "Blue", ROLE_IMPOSTER);


    manager.assignTask(red);
    manager.assignTask(red);
    manager.assignTask(red);

    manager.assignTask(blue);
    manager.assignTask(blue);

    manager.printPlayerStatus(red);
    manager.printPlayerStatus(blue);

   
    int redTask = red.taskList.head->data;
    int blueTask = blue.taskList.head->data;

    red.completeTask(redTask);
    blue.completeTask(blueTask);

    manager.printPlayerStatus(red);
    manager.printPlayerStatus(blue);

    return 0;
}
