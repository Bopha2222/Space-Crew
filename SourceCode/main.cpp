#include "Player.hpp"
#include <iostream>
using namespace std;
int main() {
    Player p1("Alice");
    Player p2("Bob");

    p1.setRole(CREWMATE);
    p2.setRole(IMPOSTOR);

    cout << "Initial Status:\n";
    p1.displayStatus();
    p2.displayStatus();

    cout << "\nAlice completes 2 tasks...\n";
    p1.completeTask();
    p1.completeTask();

    cout << "\nBob (impostor) kills Alice...\n";
    p1.killPlayer();

    cout << "\nUpdated Status:\n";
    p1.displayStatus();
    p2.displayStatus();

    return 0;
}