#ifndef LEADERBOARD_HPP
#define LEADERBOARD_HPP

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Leaderboard
{
public:
    void show()
    {
        ifstream file("game_results.csv");
        if (!file)
        {
            cout << "No game results found.\n";
            return;
        }

        string playerNames[100];
        int winCounts[100];
        int totalPlayers = 0;

        string line;
        getline(file, line);

        while (getline(file, line))
        {
            if (line.empty())
            {
                continue;
            }

            string playerName = "";
            string outcome = "";
            string roundNum = "";

            int commaCount = 0;
            for (int i = 0; i < line.length(); i++)
            {
                if (line[i] == ',')
                {
                    commaCount++;
                }
                else
                {
                    if (commaCount == 0)
                    {
                        playerName = playerName + line[i];
                    }
                    else if (commaCount == 1)
                    {
                        outcome = outcome + line[i];
                    }
                    else if (commaCount == 2)
                    {
                        roundNum = roundNum + line[i];
                    }
                }
            }

            if (outcome == "Win")
            {
                bool found = false;
                for (int i = 0; i < totalPlayers; i++)
                {
                    if (playerNames[i] == playerName)
                    {
                        winCounts[i]++;
                        found = true;
                        break;
                    }
                }

                if (!found)
                {
                    playerNames[totalPlayers] = playerName;
                    winCounts[totalPlayers] = 1;
                    totalPlayers++;
                }
            }
        }
        file.close();

        if (totalPlayers == 0)
        {
            cout << "No wins recorded yet.\n";
            return;
        }

        cout << "\n=== Leaderboard ===\n";
        cout << "Player\t\tWinPts\n";
        cout << "------------------------------\n";

        for (int i = 0; i < totalPlayers; i++)
        {
            int winPts = winCounts[i] * 1000;
            cout << playerNames[i] << "\t\t" << winPts << "\n";
        }
        cout << "\n";
    }
};

#endif