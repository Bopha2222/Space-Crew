#include "player.hpp"

int main() {
    Player p1("Red", false);

    p1.addTask(101);
    p1.addTask(102);
    p1.addTask(103);

    p1.printTasks(); 

    p1.completeTask();
    p1.printTasks();        
    p1.printMissionHistory(); 

    p1.addAction(1); 
    p1.addAction(2); 
    p1.printActionQueue();

    p1.performNextAction();
    p1.printActionQueue();

    return 0;
}
