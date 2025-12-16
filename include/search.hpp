#ifndef SEARCH_HPP
#define SEARCH_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

class SearchSystem
{
public:
    void searchByUsername(string username)
    {
        ifstream file("data/console_log.csv");
        if (!file)
        {
            cout << "Log file not found!\n";
            return;
        }

        cout << "\n========================================\n";
        cout << "  SEARCH RESULTS FOR: " << username << "\n";
        cout << "========================================\n";

        string line;
        getline(file, line);

        cout << "Date\t\tTime\t\tOperation\tStatus\t\tReason\n";
        cout << "--------------------------------------------------------------------------------\n";

        int count = 0;
        while (getline(file, line))
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
                cout << date << "\t" << time << "\t" 
                     << operation << "\t\t" << status << "\t\t" 
                     << reason << "\n";
                count++;
            }
        }

        file.close();

        cout << "--------------------------------------------------------------------------------\n";
        cout << "Total matches: " << count << "\n";
        cout << "========================================\n\n";
    }

    void searchByDate(string targetDate)
    {
        ifstream file("data/console_log.csv");
        if (!file)
        {
            cout << "Log file not found!\n";
            return;
        }

        cout << "\n========================================\n";
        cout << "  SEARCH RESULTS FOR DATE: " << targetDate << "\n";
        cout << "========================================\n";

        string line;
        getline(file, line);

        cout << "Time\t\tUsername\tOperation\tStatus\t\tReason\n";
        cout << "--------------------------------------------------------------------------------\n";

        int count = 0;
        while (getline(file, line))
        {
            stringstream ss(line);
            string date, time, username, operation, status, reason;
            
            getline(ss, date, ',');
            getline(ss, time, ',');
            getline(ss, username, ',');
            getline(ss, operation, ',');
            getline(ss, status, ',');
            getline(ss, reason, ',');

            if (date == targetDate)
            {
                cout << time << "\t" << username << "\t\t" 
                     << operation << "\t\t" << status << "\t\t" 
                     << reason << "\n";
                count++;
            }
        }

        file.close();

        cout << "--------------------------------------------------------------------------------\n";
        cout << "Total matches: " << count << "\n";
        cout << "========================================\n\n";
    }

    void searchByOperation(string operationType)
    {
        ifstream file("data/console_log.csv");
        if (!file)
        {
            cout << "Log file not found!\n";
            return;
        }

        cout << "\n========================================\n";
        cout << "  SEARCH RESULTS FOR OPERATION: " << operationType << "\n";
        cout << "========================================\n";

        string line;
        getline(file, line);

        cout << "Date\t\tTime\t\tUsername\tStatus\t\tReason\n";
        cout << "--------------------------------------------------------------------------------\n";

        int count = 0;
        while (getline(file, line))
        {
            stringstream ss(line);
            string date, time, username, operation, status, reason;
            
            getline(ss, date, ',');
            getline(ss, time, ',');
            getline(ss, username, ',');
            getline(ss, operation, ',');
            getline(ss, status, ',');
            getline(ss, reason, ',');

            if (operation == operationType)
            {
                cout << date << "\t" << time << "\t" 
                     << username << "\t\t" << status << "\t\t" 
                     << reason << "\n";
                count++;
            }
        }

        file.close();

        cout << "--------------------------------------------------------------------------------\n";
        cout << "Total matches: " << count << "\n";
        cout << "========================================\n\n";
    }

    void searchByStatus(string targetStatus)
    {
        ifstream file("data/console_log.csv");
        if (!file)
        {
            cout << "Log file not found!\n";
            return;
        }

        cout << "\n========================================\n";
        cout << "  SEARCH RESULTS FOR STATUS: " << targetStatus << "\n";
        cout << "========================================\n";

        string line;
        getline(file, line);

        cout << "Date\t\tTime\t\tUsername\tOperation\tReason\n";
        cout << "--------------------------------------------------------------------------------\n";

        int count = 0;
        while (getline(file, line))
        {
            stringstream ss(line);
            string date, time, username, operation, status, reason;
            
            getline(ss, date, ',');
            getline(ss, time, ',');
            getline(ss, username, ',');
            getline(ss, operation, ',');
            getline(ss, status, ',');
            getline(ss, reason, ',');

            if (status == targetStatus)
            {
                cout << date << "\t" << time << "\t" 
                     << username << "\t\t" << operation << "\t\t" 
                     << reason << "\n";
                count++;
            }
        }

        file.close();

        cout << "--------------------------------------------------------------------------------\n";
        cout << "Total matches: " << count << "\n";
        cout << "========================================\n\n";
    }

    void searchByFailureReason(string failureReason)
    {
        ifstream file("data/console_log.csv");
        if (!file)
        {
            cout << "Log file not found!\n";
            return;
        }

        cout << "\n========================================\n";
        cout << "  SEARCH RESULTS FOR REASON: " << failureReason << "\n";
        cout << "========================================\n";

        string line;
        getline(file, line);

        cout << "Date\t\tTime\t\tUsername\tOperation\tStatus\n";
        cout << "--------------------------------------------------------------------------------\n";

        int count = 0;
        while (getline(file, line))
        {
            stringstream ss(line);
            string date, time, username, operation, status, reason;
            
            getline(ss, date, ',');
            getline(ss, time, ',');
            getline(ss, username, ',');
            getline(ss, operation, ',');
            getline(ss, status, ',');
            getline(ss, reason, ',');

            if (reason == failureReason)
            {
                cout << date << "\t" << time << "\t" 
                     << username << "\t\t" << operation << "\t\t" 
                     << status << "\n";
                count++;
            }
        }

        file.close();

        cout << "--------------------------------------------------------------------------------\n";
        cout << "Total matches: " << count << "\n";
        cout << "========================================\n\n";
    }

    void searchByDateRange(string startDate, string endDate)
    {
        ifstream file("data/console_log.csv");
        if (!file)
        {
            cout << "Log file not found!\n";
            return;
        }

        cout << "\n========================================\n";
        cout << "  SEARCH RESULTS: " << startDate << " to " << endDate << "\n";
        cout << "========================================\n";

        string line;
        getline(file, line);

        cout << "Date\t\tTime\t\tUsername\tOperation\tStatus\n";
        cout << "--------------------------------------------------------------------------------\n";

        int count = 0;
        while (getline(file, line))
        {
            stringstream ss(line);
            string date, time, username, operation, status, reason;
            
            getline(ss, date, ',');
            getline(ss, time, ',');
            getline(ss, username, ',');
            getline(ss, operation, ',');
            getline(ss, status, ',');
            getline(ss, reason, ',');

            if (date >= startDate && date <= endDate)
            {
                cout << date << "\t" << time << "\t" 
                     << username << "\t\t" << operation << "\t\t" 
                     << status << "\n";
                count++;
            }
        }

        file.close();

        cout << "--------------------------------------------------------------------------------\n";
        cout << "Total matches: " << count << "\n";
        cout << "========================================\n\n";
    }
};

#endif