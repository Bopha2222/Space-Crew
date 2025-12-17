#ifndef MISSION_HPP
#define MISSION_HPP

#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <map>
#include <stack>



class Room {
public:
    std::string name;
    std::vector<Room*> neighbors;

    Room(const std::string& n) ;
    void addNeighbor(Room* r) ;
        
 };


class Task {
public:
    std:: string name;
    bool completed;
    std:: vector<Task*> children; //pointer to subtasks

    Task( const std:: string& n);

    void addSubTask(Task* t) ;
};

class MissionManager {
private:
   std:: vector<Room*> rooms;
   std:: map<std::string, Room*> roomMap;
   std::queue<std::string> voteQueue;
    std::vector<std::string> playerNames;

public:
    MissionManager();
    ~MissionManager();

    void addRoom(const std::string& name);
    void connectRooms(const std:: string& r1, const std:: string& r2);
    void showMap()const;
    
    bool movePlayer(const std:: string& player,const std:: string& from,const std::string& to);

    Task* createTaskTree() const;  // Returns root task
    void completeTask(Task* t) const;

    void reportVote(const std::string &playerName);
    void countVotes();

    /*void saveGameCSV(string filename);
    void loadGameCSV(string filename);*/
};

#endif
