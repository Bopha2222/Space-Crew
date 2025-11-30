#pragma once
#include "Player.hpp"
#include "Mission.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

class Game {
public:
    Player players[10];      // max 10 players
    int totalPlayers;

    Mission missions[10];    // 10 unique missions
    int missionCount;

    Game() {
        totalPlayers = 4; // example: 2 humans + 2 bots
        missionCount = 10;

        // Initialize players
        players[0] = Player("You", true);
        players[1] = Player("Friend", true);
        players[2] = Player("Bot1", false);
        players[3] = Player("Bot2", false);

        // Initialize missions
        missions[0] = Mission("Clean Window", "Clean the window in Cafeteria");
        missions[1] = Mission("Swipe Card", "Swipe your ID card in Admin room");
        missions[2] = Mission("Fix Wiring", "Connect the wires in Electrical");
        missions[3] = Mission("Download Data", "Download data from Cafeteria");
        missions[4] = Mission("Upload Logs", "Upload logs in Admin room");
        missions[5] = Mission("Fuel Engines", "Refuel engines in Upper Engine");
        missions[6] = Mission("Calibrate Distributor", "Calibrate the distributor in Electrical");
        missions[7] = Mission("Align Telescope", "Align telescope in Observatory");
        missions[8] = Mission("Empty Trash", "Throw out trash from Storage");
        missions[9] = Mission("Fix Security Camera", "Repair security camera in Security");

        srand(time(0)); // seed random

        assignRandomMissions();
    }

    void assignRandomMissions() {
        // Assign 3 random missions to each player
        for (int i = 0; i < totalPlayers; i++) {
            int count = 0;
            while (count < 3) {
                int randIndex = rand() % missionCount;
                players[i].assignMission(missions[randIndex]);
                count++;
            }
        }
    }

    void showAllPlayerMissions() {
        for (int i = 0; i < totalPlayers; i++) {
            players[i].showMissions();
        }
    }

    void playerCompleteMission(int playerIndex, int missionIndex) {
        if (playerIndex >= 0 && playerIndex < totalPlayers)
            players[playerIndex].completeMission(missionIndex);
    }

    void botsDoMissions() {
        for (int i = 0; i < totalPlayers; i++) {
            if (!players[i].isHuman) {
                for (int j = 0; j < players[i].missionCount; j++) {
                    if (!players[i].assignedMissions[j].isCompleted)
                        players[i].completeMission(j);
                }
            }
        }
    }
};
