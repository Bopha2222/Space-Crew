
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include "login.hpp"
#include "game.hpp"
#include "leaderboard.hpp"

using namespace std;


void showWelcome()
{
    cout << "=====================================\n";
    cout << "        AMONG-US C++ PROJECT         \n";
    cout << "=====================================\n";
    cout << " 1. Login\n";
    cout << " 2. Signup\n";
    cout << " 3. Exit\n";
    cout << "-------------------------------------\n";
    cout << "Choose option: ";
}

void showMainMenu()
{
    cout << "\n=====================================\n";
    cout << "              MAIN MENU              \n";
    cout << "=====================================\n";
    cout << " 1. Start Game\n";
    cout << " 2. Leaderboard\n";
    cout << " 3. Logout\n";
    cout << "-------------------------------------\n";
    cout << "Choose option: ";
}

void logGameResult(string username, bool playerWon, int totalRounds)
{
    ofstream file("game_results.csv", ios::app);
    if (!file)
    {
        return;
    }

    file << username << ",";
    
    if (playerWon)
    {
        file << "Win,";
    }
    else
    {
        file << "Lose,";
    }
    
    file << totalRounds << "\n";

    file.close();
}

void createCSVFiles()
{
    ifstream checkLog("console_log.csv");
    if (!checkLog)
    {
        ofstream file("console_log.csv");
        file << "Username,Action,Status,Date,Time\n";
        file.close();
    }
    else
    {
        checkLog.close();
    }

    ifstream checkGame("game_results.csv");
    if (!checkGame)
    {
        ofstream file("game_results.csv");
        file << "Username,Outcome,TotalRounds\n";
        file.close();
    }
    else
    {
        checkGame.close();
    }

    ifstream checkTask("task.csv");
    if (!checkTask)
    {
        ofstream file("task.csv");
        file << "Fix Wiring\n";
        file << "Empty Garbage\n";
        file << "Fuel Engines\n";
        file << "Align Telescope\n";
        file << "Clean O2 Filter\n";
        file << "Calibrate Distributor\n";
        file << "Download Data\n";
        file << "Inspect Sample\n";
        file.close();
    }
    else
    {
        checkTask.close();
    }

    ifstream checkTaskPw("taskpw.csv");
    if (!checkTaskPw)
    {
        ofstream file("taskpw.csv");
        file << "wire123\n";
        file << "trash456\n";
        file << "fuel789\n";
        file << "star000\n";
        file << "oxygen12\n";
        file << "power888\n";
        file << "data999\n";
        file << "sample77\n";
        file.close();
    }
    else
    {
        checkTaskPw.close();
    }
}

void playGameMenu(string username, Leaderboard &leaderboard)
{
    bool loggedIn = true;

    while (loggedIn)
    {
        showMainMenu();
        int menuChoice;
        cin >> menuChoice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input! Please enter a number (1-3).\n";
            continue;
        }

        cin.ignore();

        if (menuChoice == 1)
        {
            cout << "\nStarting game...\n";
            
            Game game;
            game.player.name = username;
            game.selectDifficulty();
            game.startGame();

            logGameResult(username, game.playerWon, game.roundNumber);
        }
        else if (menuChoice == 2)
        {
            cout << "\nShowing leaderboard...\n";
            leaderboard.show();
        }
        else if (menuChoice == 3)
        {
            cout << "\nLogged out.\n\n";
            loggedIn = false;
        }
        else
        {
            cout << "Invalid option. Try again.\n";
        }
    }
}

int main()
{
    createCSVFiles();

    LoginSystem loginSystem;
    Leaderboard leaderboard;

    string username;
    string password;
    int choice;

    while (true)
    {
        showWelcome();
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input! Please enter a number (1-3).\n\n";
            continue;
        }

        cin.ignore();

        if (choice == 1)
        {
            cout << "\nEnter username: ";
            getline(cin, username);

            cout << "Enter password: ";
            getline(cin, password);

            bool loginSuccess = loginSystem.login(username, password);

            if (loginSuccess)
            {
                cout << "\nLogin successful! Welcome, " << username << "!\n";
                playGameMenu(username, leaderboard);
            }
            else
            {
                cout << "\nLogin failed. Try again.\n\n";
            }
        }
        else if (choice == 2)
        {
            cout << "\nEnter new username: ";
            getline(cin, username);

            cout << "Enter new password: ";
            getline(cin, password);

            bool signupSuccess = loginSystem.signup(username, password);

            if (signupSuccess)
            {
                cout << "\nSignup successful! Welcome, " << username << "!\n";
                playGameMenu(username, leaderboard);
            }
        }
        else if (choice == 3)
        {
            cout << "\nExiting program.\n";
            break;
        }
        else
        {
            cout << "Invalid option. Try again.\n";
        }
    }

    return 0;
}
