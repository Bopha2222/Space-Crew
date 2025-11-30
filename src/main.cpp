#include "game.hpp"
#include <iostream>
using namespace std;

int main() {
    Game game;


    game.showAllPlayerMissions();

    // Human player 
    int choice;
    cout << "\nEnter mission number to complete (1-3): ";
    cin >> choice;
    game.playerCompleteMission(0, choice - 1);

    // Bots complete all missions automatically
    game.botsDoMissions();

    cout << "\nAfter completing missions:\n";
    game.showAllPlayerMissions();

    return 0;
}
