#include "Mission.hpp"
#include<iostream>
#include<algorithm>
#include<cstdlib>
#include<ctime>


using namespace std;


/// @brief 
/// @param n 
Room::Room(const string& n) : name(n){}
void Room ::addNeighbor(Room*r){
        neighbors.push_back(r);
    }

 
 
Task::Task(const string& n):name(n) , completed(false){}
void Task::addSubTask(Task *t)
{
    children.push_back(t);
}

MissionManager:: MissionManager(){
        srand(static_cast<unsigned int>(time(0))); // random seed
     }
MissionManager::  ~MissionManager(){
        for (Room* r : rooms) delete r;
     }
     void MissionManager:: addRoom(const string& name){
        Room*r=new Room( name);
        rooms.push_back(r);
        roomMap[name]=r;
     }
     void MissionManager:: connectRooms(const string& r1 , const string& r2){
        if(roomMap.find(r1) != roomMap.end() && roomMap.find(r2) != roomMap.end()) return;
        Room* a = roomMap[r1];
        Room* b=roomMap[r2];

        if(find(a->neighbors.begin() , a->neighbors.end(),b)== a->neighbors.end()){
            a->addNeighbor(b);
        }
        if(find(b->neighbors.begin() , b->neighbors.end() , a)== b->neighbors.end()){
            b->addNeighbor(a);
        }
    
}
void MissionManager:: showMap() const{
    cout<<"\n--Map--\n";
    for (Room* r : rooms) {
        cout << r->name << " -> ";
        for (Room* n : r->neighbors)
            cout << n->name << " ";
        cout << endl;
    }
}
bool MissionManager:: movePlayer( const string &player,const string &from,const string& to) {
    if (roomMap.find(from) == roomMap.end() || roomMap.find(to) == roomMap.end())
        return false;

    Room* fromRoom = roomMap.at(from);
    Room* toRoom = roomMap.at(to);

    auto it = find(fromRoom->neighbors.begin(), fromRoom->neighbors.end(), toRoom);
    if (it != fromRoom->neighbors.end()) {
        cout << player << " moved from " << from << " to " << to << endl;
        return true;
    } else {
        cout << "Move invalid: " << to << " is not neighbor of " << from << endl;
        return false;
    }
}
 Task*MissionManager::createTaskTree()const {
    Task* root = new Task("Main Task");

    Task* t1 = new Task("Fix Wiring");
    Task* t2 = new Task("Upload Data");
    Task* t3 = new Task("Calibrate Engine");

    root->addSubTask(t1);
    root->addSubTask(t2);
    root->addSubTask(t3);

    t1->addSubTask(new Task("Wire A"));
    t1->addSubTask(new Task("Wire B"));

    t2->addSubTask(new Task("Upload 1"));
    t2->addSubTask(new Task("Upload 2"));
    return root;
}
void MissionManager:: completeTask(Task*t) const {
    if(!t)return;
    t->completed=true;
    cout<<"Task : "<<t->name<<"completed.."<<endl;
}
void MissionManager:: reportVote(const string &playerName){
    voteQueue.push(playerName);
    cout << playerName << " reported for voting." << endl;
}

void MissionManager::countVotes()
{
    map<string, int> voteCount;
    while (!voteQueue.empty()) {
        string p = voteQueue.front();
        voteQueue.pop();
        voteCount[p]++;
    }

    cout << "\n--- Vote Count ---\n";
    for (auto& it : voteCount)
        cout << it.first << " : " << it.second << " votes\n";
}

int main() {
    MissionManager mission;

    mission.addRoom("Cafeteria");
    mission.addRoom("Engine");
    mission.addRoom("MedBay");

    mission.connectRooms("Cafeteria", "Engine");
    mission.connectRooms("Engine", "MedBay");

    mission.showMap();

    mission.movePlayer("Alice", "Cafeteria", "Engine");

    Task* tasks = mission.createTaskTree();
    mission.completeTask(tasks->children[0]->children[0]); // Complete Wire A

    mission.reportVote("Bob");
    mission.reportVote("Charlie");
    mission.reportVote("Bob");
    mission.countVotes();

    delete tasks;

    return 0;
}
