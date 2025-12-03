#ifndef MISSION_HPP
#define MISSION_HPP

#include "../player/player.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

class MissionManager {
public:
    int allMissionIDs[10] = {101,102,103,104,105,106,107,108,109,110};
    string allMissionDescriptions[10] = {
        "Fix wiring in Engine Room",
        "Calibrate sensors in Navigation",
        "Upload data to HQ",
        "Inspect Medbay",
        "Clean O2 filter",
        "Repair Reactor",
        "Fuel engines",
        "Check communications",
        "Align telescopes",
        "Scan anomaly"
    };

    MissionManager();

    void assignTask(Player &player);
    bool isTaskCompleted(Player &player, int taskID);
    void printPlayerTasks(Player &player);
    void printPlayerStatus(Player &player);
};

#endif
