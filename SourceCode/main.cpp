#include "Player.hpp"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
int main() {
    /*string player;
    int size;
    fstream f1;
    f1.open("Player.hpp",ios::in);
    f1.open("Player.cpp",ios::in);
    while(!f1.eof()){
        f1>>player;
        cout<<player<<",";
    }

   f1.close();
   cout << "[";
    for (size_t i = 0; i < player.size(); ++i) {
        if (i) cout << ", ";
        cout << player[i];
    }
    cout << "]\n";
    }*/
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
