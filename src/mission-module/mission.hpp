#include "../player/player.hpp"

class MissionManager {
public:

    void assignTask(Player &player, int taskID);


    bool isTaskCompleted(Player &player, int taskID);


    void printPlayerTasks(Player &player);

    
    void printPlayerStatus(Player &player);
};


