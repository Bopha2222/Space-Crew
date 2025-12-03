#include "FileIO.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

// Player state
void savePlayers( Player players[],int  amountofplayer)
{
    ofstream file("players.csv");
    if (!file)
    {
        cout << "Error saving players!\n";
        return;
    }

    file << "id,name,role,alive\n";

    for (int i=0;i<amountofplayer;i++)
    {
        file << players[i].id << ","
             << players[i].name << ","
             << players[i].role << ","
             << players[i].alive << "\n";
    }

    cout << "Players saved successfully!\n";
}

int loadPlayers(Player players[],int maxplayer)
{
    ifstream file("players.csv");
    if (!file)
    {
        cout << "Error loading players!\n";
        return 0;
    }

    string line;
    getline(file, line); // skip header
   int  count=0;
    while (getline(file, line)&& count<maxplayer)
    {
        stringstream ss(line);
        string id, name, role, alive;

        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, role, ',');
        getline(ss, alive, ',');
        players[count].id    = stoi(id);
        players[count].name  = name;
        players[count].role  = role;
        players[count].alive = (alive == "1");

        count++;
      
    }

    cout << "Players loaded successfully!\n";
    return count;
}

void saveTasks(Task tasks[], int taskcount)
{
    ofstream file("tasks.csv");
    if (!file)
    {
        cout << "Error saving tasks!\n";
        return;
    }

    file << "task_id,name,status\n";

    for (int i=0;i<taskcount;i++)
    {
        file << tasks[i].task_id << ","
             << tasks[i].name << ","
             << tasks[i].status << "\n";
             
    }

    cout << "Tasks saved!\n";
}

int loadTasks(Task tasks[], int taskcount)
{
    ifstream file("tasks.csv");
    if (!file)
    {
        cout << "Error loading tasks!\n";
        return 0;
    }

    string line;
    getline(file, line);
    int  count=0;
    while (getline(file, line))
    {
        stringstream ss(line);
        string id, name, status;

        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, status, ',');
        tasks[count].task_id = stoi(id);
        tasks[count].name    = name;
        tasks[count].status  = (status == "1");
        count++;

    }

    cout << "Tasks loaded!\n";
    return count;
}

// Game state
void saveGameState(int round, int tasksDone, int totalTasks)
{
    ofstream file("game_state.csv");
    if (!file)
    {
        cout << "Error saving game state!\n";
        return;
    }

    file << "round,tasks_done,total_tasks\n";
    file << round << "," << tasksDone << "," << totalTasks << "\n";

    cout << "Game state saved!\n";
}

void loadGameState(int &round, int &tasksDone, int &totalTasks)
{
    ifstream file("game_state.csv");
    if (!file)
    {
        cout << "Error loading game state!\n";
        return;
    }

    string line;
    getline(file, line);
    getline(file, line);

    stringstream ss(line);
    string r, td, tt;

    getline(ss, r, ',');
    getline(ss, td, ',');
    getline(ss, tt, ',');

    round = stoi(r);
    tasksDone = stoi(td);
    totalTasks = stoi(tt);

    cout << "Game state loaded!\n";
}

// Vote

void saveVotes(Vote votes[] ,int  votecount)
{
    ofstream file("votes.csv");
    if (!file)
    {
        cout << "Error saving votes!\n";
        return ;
    }

    file << "voter,target\n";


   for (int i=0;i<votecount;i++)
    {
        file << votes[i].voter_id << ","
             << votes[i].target_id << "\n";
    }

    cout << "Votes saved!\n";
}
int Votes(Vote votes[] ,int  votecount)
{
    ifstream file("votes.csv");

    if (!file)
    {
        cout << "Error loading votes!\n";
        return 0;
    }

    string line;
    getline(file, line); // skip header
    int  count=0;
    while (getline(file, line)&&count<votecount)
    {
        stringstream ss(line);
        string v, t;
        getline(ss, v, ',');
        getline(ss, t, ',');
       votes[count].voter_id = stoi(v);
        votes[count].target_id = stoi(t);
        count++;
    }
    cout << "Votes loaded!\n";
    return count;
}
