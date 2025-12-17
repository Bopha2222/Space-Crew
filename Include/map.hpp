
#ifndef MAP_HPP
#define MAP_HPP

#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

class GameMap
{
public:
    static const int roomCount = 11;        // 1 meeting + 10 task rooms
    int adjacency[roomCount][roomCount];    // adjacency matrix (0 = no connection, 1 = connected)
    string roomNames[roomCount];            // names for debugging

    GameMap()
    {
        srand(static_cast<unsigned int>(time(nullptr)));

        // Initialize adjacency matrix to 0
        for (int i = 0; i < roomCount; i++)
            for (int j = 0; j < roomCount; j++)
                adjacency[i][j] = 0;

        // Initialize room names
        roomNames[0] = "Meeting Room"; // middle
        for (int i = 1; i < roomCount; i++)
            roomNames[i] = "Room " + to_string(i);

        // Connect meeting room to all task rooms
        for (int i = 1; i < roomCount; i++)
        {
            adjacency[0][i] = 1;
            adjacency[i][0] = 1;
        }

        // Circular connections among task rooms
        for (int i = 1; i < roomCount; i++)
        {
            int next = (i % 10) + 1; // wraps around
            adjacency[i][next] = 1;
            adjacency[next][i] = 1;
        }
    }

    // Get neighbors of a room (fill array and return count)
    int getNeighbors(int roomIndex, int neighbors[])
    {
        int count = 0;
        for (int i = 0; i < roomCount; i++)
        {
            if (adjacency[roomIndex][i] == 1)
            {
                neighbors[count] = i;
                count++;
            }
        }
        return count;
    }

    // Get random neighbor
    int getRandomNeighbor(int roomIndex)
    {
        int neighbors[roomCount];
        int n = getNeighbors(roomIndex, neighbors);
        if (n == 0) return roomIndex; // no movement
        int r = rand() % n;
        return neighbors[r];
    }

    // Print map
    void printMap()
    {
        for (int i = 0; i < roomCount; i++)
        {
            cout << roomNames[i] << " connects to: ";
            for (int j = 0; j < roomCount; j++)
            {
                if (adjacency[i][j] == 1)
                    cout << roomNames[j] << " ";
            }
            cout << endl;
        }
    }
};

#endif
