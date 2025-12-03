#include "../player/player.hpp"

int main() {
    // Create players (name, color, role)
    Player red("Red", "Red", ROLE_CREWMATE);
    Player blue("Blue", "Blue", ROLE_IMPOSTER);

    red.addTask(101);
    red.addTask(102);
    red.addTask(103);

    red.printStatus();

    red.completeTask(101);  
    red.printStatus();


    red.enqueueAction(1);
    red.enqueueAction(2);
    red.printStatus();

    red.performAction();
    red.printStatus();

 
    blue.sabotage("Lights");
    blue.sabotage("Reactor");
    blue.printStatus();

    blue.fixSabotage(); 
    blue.printStatus();

    blue.kill(red); 
    red.reportBody(blue); 
    blue.reportBody(red); 

    red.printStatus();
    blue.printStatus();

    return 0;
}
