#pragma once
#include "player.hpp"
#include <iostream>
using namespace std;

class Game {
public:
    Game() {
        currentState = 0;

        totalPlayers = 4; // example
        players[0] = Player("You", true);
        players[1] = Player("Friend", true);
        players[2] = Player("Bot1", false);
        players[3] = Player("Bot2", false);
    }

    void gameRun() {
        while (true) {
            if (currentState == 0)
                lobby();
            else if (currentState == 1)
                playing();
            else if (currentState == 2)
                meeting();
            else if (currentState == 3)
                gameOver();
        }
    }

private:
    void lobby() {
        cout << "Lobby: Waiting for players..." << endl;
        currentState = 1;
    }

    void playing() {
        cout << "Playing: Players performing tasks..." << endl;
        for (int i = 0; i < totalPlayers; i++) {
            if (players[i].isHuman) {
                cout << players[i].name << ", press Enter to do your task...";
                cin.ignore();
            }
            players[i].performTask();
            players[i].showStatus();
        }
        currentState = 2;
    }

    void meeting() {
        cout << "Meeting: Discussing..." << endl;
        currentState = 3;
    }

    void gameOver() {
        cout << "Game Over" << endl;
        exit(0);
    }

private:
    int currentState;
    Player players[10];   // max 10 players
    int totalPlayers;     // current number of players
};
