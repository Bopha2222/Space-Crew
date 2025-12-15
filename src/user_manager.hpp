#ifndef USER_MANAGER_HPP
#define USER_MANAGER_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

class UserManager
{
private:
    string userFile = "user.csv";

public:
    struct UserInfo
    {
        string username;
        string passwordHash;
        bool isDeleted;
        bool isAdmin;
    };

    bool accountExists(string username)
    {
        ifstream file(userFile);
        if (!file) return false;
        
        string line, savedName, savedHash, deleted, isAdmin;

        while (getline(file, line))
        {
            stringstream ss(line);
            getline(ss, savedName, ',');
            getline(ss, savedHash, ',');
            getline(ss, deleted, ',');
            getline(ss, isAdmin, ',');

            if (savedName == username)
            {
                file.close();
                return true;
            }
        }
        file.close();
        return false;
    }

    void listAllUsers()
    {
        ifstream file(userFile);
        if (!file)
        {
            cout << "No users found!\n";
            return;
        }

        cout << "\n========================================\n";
        cout << "           ALL USERS\n";
        cout << "========================================\n";
        cout << "Username\t\tRole\t\tStatus\n";
        cout << "----------------------------------------\n";

        string line;
        int activeCount = 0;
        int deletedCount = 0;
        int adminCount = 0;

        // Skip header row
        getline(file, line);

        while (getline(file, line))
        {
            stringstream ss(line);
            string username, hash, deleted, isAdmin;
            
            getline(ss, username, ',');
            getline(ss, hash, ',');
            getline(ss, deleted, ',');
            getline(ss, isAdmin, ',');

            string role = (isAdmin == "1") ? "[ADMIN]" : "[USER]";
            string status = (deleted == "1") ? "[DELETED]" : "[ACTIVE]";
            
            cout << username << "\t\t" << role << "\t" << status << "\n";
            
            if (deleted == "1")
            {
                deletedCount++;
            }
            else
            {
                activeCount++;
                if (isAdmin == "1") adminCount++;
            }
        }

        file.close();

        cout << "----------------------------------------\n";
        cout << "Active: " << activeCount << " | Deleted: " << deletedCount << " | Admins: " << adminCount << "\n";
        cout << "========================================\n\n";
    }

    void softDeleteUser(string username)
    {
        ifstream fileIn(userFile);
        if (!fileIn)
        {
            cout << "User file not found!\n";
            return;
        }

        string tempFile = "temp_users.csv";
        ofstream fileOut(tempFile);

        string line;
        bool found = false;

        while (getline(fileIn, line))
        {
            stringstream ss(line);
            string uname, hash, deleted, isAdmin;
            
            getline(ss, uname, ',');
            getline(ss, hash, ',');
            getline(ss, deleted, ',');
            getline(ss, isAdmin, ',');

            if (uname == username)
            {
                // Keep isAdmin status during soft delete
                fileOut << uname << "," << hash << ",1," << isAdmin << "\n";
                found = true;
            }
            else
            {
                fileOut << line << "\n";
            }
        }

        fileIn.close();
        fileOut.close();

        remove(userFile.c_str());
        rename(tempFile.c_str(), userFile.c_str());

        if (found)
        {
            cout << "User '" << username << "' has been soft deleted (data preserved).\n";
        }
        else
        {
            cout << "User not found!\n";
        }
    }

    void hardDeleteUser(string username)
    {
        ifstream fileIn(userFile);
        if (!fileIn)
        {
            cout << "User file not found!\n";
            return;
        }

        string tempFile = "temp_users.csv";
        ofstream fileOut(tempFile);

        string line;
        bool found = false;

        while (getline(fileIn, line))
        {
            stringstream ss(line);
            string uname, hash, deleted, isAdmin;
            
            getline(ss, uname, ',');
            getline(ss, hash, ',');
            getline(ss, deleted, ',');
            getline(ss, isAdmin, ',');

            if (uname == username)
            {
                found = true;
            }
            else
            {
                fileOut << line << "\n";
            }
        }

        fileIn.close();
        fileOut.close();

        remove(userFile.c_str());
        rename(tempFile.c_str(), userFile.c_str());

        if (found)
        {
            cout << "User '" << username << "' has been permanently deleted!\n";
        }
        else
        {
            cout << "User not found!\n";
        }
    }

    void restoreUser(string username)
    {
        ifstream fileIn(userFile);
        if (!fileIn)
        {
            cout << "User file not found!\n";
            return;
        }

        string tempFile = "temp_users.csv";
        ofstream fileOut(tempFile);

        string line;
        bool found = false;

        while (getline(fileIn, line))
        {
            stringstream ss(line);
            string uname, hash, deleted, isAdmin;
            
            getline(ss, uname, ',');
            getline(ss, hash, ',');
            getline(ss, deleted, ',');
            getline(ss, isAdmin, ',');

            if (uname == username && deleted == "1")
            {
                // Restore with ORIGINAL isAdmin status preserved
                fileOut << uname << "," << hash << ",0," << isAdmin << "\n";
                found = true;
            }
            else
            {
                fileOut << line << "\n";
            }
        }

        fileIn.close();
        fileOut.close();

        remove(userFile.c_str());
        rename(tempFile.c_str(), userFile.c_str());

        if (found)
        {
            cout << "User '" << username << "' has been restored!\n";
        }
        else
        {
            cout << "User not found or not deleted!\n";
        }
    }

    void viewUserProfile(string username)
    {
        // First check if account exists
        if (!accountExists(username))
        {
            cout << "\nNo account found in system.\n";
            return;
        }

        ifstream logFile("console_log.csv");
        ifstream gameFile("game_results.csv");

        int totalLogins = 0;
        int failedLogins = 0;
        int totalGames = 0;
        int wins = 0;
        string firstSeen = "";
        string lastSeen = "";

        string line;
        getline(logFile, line);

        while (getline(logFile, line))
        {
            stringstream ss(line);
            string date, time, uname, operation, status, reason;
            
            getline(ss, date, ',');
            getline(ss, time, ',');
            getline(ss, uname, ',');
            getline(ss, operation, ',');
            getline(ss, status, ',');
            getline(ss, reason, ',');

            if (uname == username)
            {
                if (firstSeen.empty()) firstSeen = date;
                lastSeen = date;

                if (operation == "login")
                {
                    totalLogins++;
                    if (status == "fail")
                    {
                        failedLogins++;
                    }
                }
            }
        }
        logFile.close();

        getline(gameFile, line);
        while (getline(gameFile, line))
        {
            stringstream ss(line);
            string uname, outcome;
            
            getline(ss, uname, ',');
            getline(ss, outcome, ',');

            if (uname == username)
            {
                totalGames++;
                if (outcome == "Win")
                {
                    wins++;
                }
            }
        }
        gameFile.close();

        cout << "\n========================================\n";
        cout << "      USER PROFILE: " << username << "\n";
        cout << "========================================\n";
        cout << "First Seen: " << (firstSeen.empty() ? "Never" : firstSeen) << "\n";
        cout << "Last Seen: " << (lastSeen.empty() ? "Never" : lastSeen) << "\n";
        cout << "Total Logins: " << totalLogins << "\n";
        cout << "Failed Login Attempts: " << failedLogins << "\n";
        cout << "\nGame Statistics:\n";
        cout << "  Total Games: " << totalGames << "\n";
        cout << "  Wins: " << wins << "\n";
        cout << "  Losses: " << (totalGames - wins) << "\n";
        if (totalGames > 0)
        {
            float winRate = (wins * 100.0) / totalGames;
            cout << "  Win Rate: " << winRate << "%\n";
        }
        cout << "========================================\n\n";
    }

    int countActiveUsers()
    {
        ifstream file(userFile);
        if (!file) return 0;

        string line;
        int count = 0;

        // Skip header row
        getline(file, line);

        while (getline(file, line))
        {
            stringstream ss(line);
            string username, hash, deleted, isAdmin;
            
            getline(ss, username, ',');
            getline(ss, hash, ',');
            getline(ss, deleted, ',');
            getline(ss, isAdmin, ',');

            if (deleted != "1")
            {
                count++;
            }
        }

        file.close();
        return count;
    }

    int countDeletedUsers()
    {
        ifstream file(userFile);
        if (!file) return 0;

        string line;
        int count = 0;

        // Skip header row
        getline(file, line);

        while (getline(file, line))
        {
            stringstream ss(line);
            string username, hash, deleted, isAdmin;
            
            getline(ss, username, ',');
            getline(ss, hash, ',');
            getline(ss, deleted, ',');
            getline(ss, isAdmin, ',');

            if (deleted == "1")
            {
                count++;
            }
        }

        file.close();
        return count;
    }
};

#endif
