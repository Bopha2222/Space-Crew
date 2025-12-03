#ifndef FILEIO_HPP
#define FILEIO_HPP

#include <string>
using namespace std;

//player class
class Player {
public:
    int id;
    string name;
    string role;   
    bool alive;

    Player(int i=0, string n="", string r="Crewmate", bool a=true)
        : id(i), name(n), role(r), alive(a) {}
};
 
//Task  class
class Task {
public:
    int task_id;
    string name;
    bool status; 

    Task(int id=0, string n="", bool s=false)
        : task_id(id), name(n), status(s) {}
};

//Vote class
struct Vote {
    int voter_id;
    int target_id;
};

//FileIo class
class FileIO {
public:

    static void savePlayers(Player players[],int amountofplayer );
    static int loadPlayers();

    static void saveTasks(Task tasks[],int taskcount);
    static int loadTasks();

    static void saveGameState(int round, int tasksDone, int totalTasks);
    static void loadGameState(int &round, int &tasksDone, int &totalTasks);

    static void saveVotes(Vote votes[], int votecount );
    static int loadVotes();
};
#endif
