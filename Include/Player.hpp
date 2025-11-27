#include <iostream>
#include <string>
#ifndef PLAYER_HPP
#define PLAYER_HPP
using namespace std;

enum Role { CREWMATE, IMPOSTOR };
class Player {
public:    
    Player(string name);
    void setRole(Role r);
    Role getRole();

    void killPlayer();
    bool isAlive();

    void completeTask();
    int getTaskCount();

    string getName();
    void displayStatus();

private:
    string name;
    Role role;
    bool alive;
    int tasksCompleted;
    int level;
    int energy;
    

};
#endif