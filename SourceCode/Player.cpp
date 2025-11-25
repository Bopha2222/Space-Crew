#include "Player.hpp"
#include <iostream>
using namespace std;
Player::Player(string name) : name(name), alive(true), tasksCompleted(0) { 
    this->role = CREWMATE;
    this->alive = true;
    this->tasksCompleted = 0;
    this->name = name;

}
void Player::setRole(Role r) {
    this->role = r;
}

Role Player::getRole() {
    return this->role;
}
void Player::killPlayer() {
    this->alive = false;
}

bool Player::isAlive() {
    return this->alive;
}
void Player::completeTask() {
    this->tasksCompleted++;
}
int Player::getTaskCount() {
    return this->tasksCompleted;
}
string Player::getName() {
    return this->name;
}
void Player::displayStatus() {
    cout << "Player: " << name << ", Role: " << (role == CREWMATE ? "CREWMATE" : "IMPOSTOR") 
         << ", Alive: " << (alive ? "Yes" : "No") 
         << ", Tasks Completed: " << tasksCompleted << endl;
}