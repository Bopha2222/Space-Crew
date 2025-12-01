#include "FileIO.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;


//Player syate
void FileIO::savePlayers(const vector<Player>& players) {
    ofstream file("players.csv");
    if (!file) {
        cout << "Error saving players!\n";
        return;
    }

    file << "id,name,role,alive\n";

    for (auto &p : players) {
        file << p.id << ","
             << p.name << ","
             << p.role << ","
             << p.alive << "\n";
    }

   cout << "Players saved successfully!\n";
}

vector<Player> FileIO::loadPlayers() {
    vector<Player> players;
    ifstream file("players.csv");
    if (!file) {
      cout << "Error loading players!\n";
        return players;
    }

    string line;
    getline(file, line); // skip header

    while (getline(file, line)) {
       stringstream ss(line);
       string id, name, role, alive;

        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, role, ',');
        getline(ss, alive, ',');

        players.emplace_back(stoi(id), name, role, alive == "1");
    }

    cout << "Players loaded successfully!\n";
    return players;
}


void FileIO::saveTasks(const vector<Task>& tasks) {
    ofstream file("tasks.csv");
    if (!file) {
        cout << "Error saving tasks!\n";
        return;
    }

    file << "task_id,name,status\n";

    for (auto &t : tasks) {
        file << t.task_id << ","
             << t.name << ","
             << t.status << "\n";
    }

   cout << "Tasks saved!\n";
}

vector<Task> FileIO::loadTasks() {
    vector<Task> tasks;
    ifstream file("tasks.csv");
    if (!file) {
        cout << "Error loading tasks!\n";
        return tasks;
    }

    string line;
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string id, name, status;

        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, status, ',');

        tasks.emplace_back(stoi(id), name, status == "1");
    }

    cout << "Tasks loaded!\n";
    return tasks;
}

//Game state
void FileIO::saveGameState(int round, int tasksDone, int totalTasks) {
    ofstream file("game_state.csv");
    if (!file) {
       cout << "Error saving game state!\n";
        return;
    }

    file << "round,tasks_done,total_tasks\n";
    file << round << "," << tasksDone << "," << totalTasks << "\n";

    cout << "Game state saved!\n";
}

void FileIO::loadGameState(int &round, int &tasksDone, int &totalTasks) {
   ifstream file("game_state.csv");
    if (!file) {
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
    tasksDone =stoi(td);
    totalTasks =stoi(tt);

    cout << "Game state loaded!\n";
}

//Vote

void FileIO::saveVotes(const queue<Vote>& votes) {
    ofstream file("votes.csv");
    if (!file) {
        cout << "Error saving votes!\n";
        return;
    }

    file << "voter,target\n";

    queue<Vote> copy = votes;

    while (!copy.empty()) {
        file << copy.front().voter_id << ","
             << copy.front().target_id << "\n";
        copy.pop();
    }

   cout << "Votes saved!\n";
}
queue<Vote> FileIO::loadVotes() {
    queue<Vote> votes;
   ifstream file("votes.csv");

    if (!file) {
       cout << "Error loading votes!\n";
        return votes;
    }

    string line;
    getline(file, line); // skip header

    while (getline(file, line)) {
       stringstream ss(line);
        string v, t;
        getline(ss, v, ',');
        getline(ss, t, ',');
        votes.push({stoi(v),stoi(t)});
    }
    cout << "Votes loaded!\n";
    return votes;
}
int main() {
    FileIO io;

    // Test players
    vector<Player> players = {
        Player(1, "Alice", "Crewmate", true),
        Player(2, "Bob", "Impostor", true)
    };

    io.savePlayers(players);
    auto loadedPlayers = io.loadPlayers();
    cout << "Loaded Players:\n";
    for (auto &p : loadedPlayers) {
        cout << p.id << " | " << p.name << " | "
                  << p.role << " | " << p.alive << "\n";
    }

    return 0;
}