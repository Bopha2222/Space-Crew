#ifndef ANALYTICS_HPP
#define ANALYTICS_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>

using namespace std;

class Analytics
{
public:
    struct HourlyData
    {
        int hour;
        int operations;
    };

    struct FailureData
    {
        string reason;
        int count;
        float percentage;
    };

    struct DailyReport
    {
        string date;
        int totalOps;
        int signups;
        int signupSuccess;
        int signupFailed;
        int logins;
        int loginSuccess;
        int loginFailed;
        HourlyData hourlyActivity[24];
        FailureData failures[20];
        int failureCount;
        float successRate;
    };

    struct WeeklyReport
    {
        string startDate;
        string endDate;
        int totalOps;
        int dailyCounts[7];
        string dailyDates[7];
        int signups;
        int signupSuccess;
        int logins;
        int loginSuccess;
        FailureData failures[20];
        int failureCount;
        float successRate;
    };

    struct MonthlyReport
    {
        int month;
        int year;
        int totalOps;
        int signups;
        int signupSuccess;
        int logins;
        int loginSuccess;
        FailureData failures[20];
        int failureCount;
        float successRate;
        int uniqueUsers;
        int peakDay;
        int peakDayOps;
        float dailyAverage;
    };

    DailyReport processDailyData(string targetDate)
    {
        DailyReport report;
        report.date = targetDate;
        report.totalOps = 0;
        report.signups = 0;
        report.signupSuccess = 0;
        report.signupFailed = 0;
        report.logins = 0;
        report.loginSuccess = 0;
        report.loginFailed = 0;
        report.failureCount = 0;
        report.successRate = 0;

        for (int i = 0; i < 24; i++)
        {
            report.hourlyActivity[i].hour = i;
            report.hourlyActivity[i].operations = 0;
        }

        for (int i = 0; i < 20; i++)
        {
            report.failures[i].reason = "";
            report.failures[i].count = 0;
            report.failures[i].percentage = 0;
        }

        ifstream file("console_log.csv");
        if (!file)
        {
            return report;
        }

        string line;
        getline(file, line);

        while (getline(file, line))
        {
            string date, time, username, operation, status, reason;

            stringstream ss(line);
            getline(ss, date, ',');
            getline(ss, time, ',');
            getline(ss, username, ',');
            getline(ss, operation, ',');
            getline(ss, status, ',');
            getline(ss, reason, ',');

            if (date == targetDate)
            {
                report.totalOps++;

                int hour = (time[0] - '0') * 10 + (time[1] - '0');
                if (hour >= 0 && hour < 24)
                {
                    report.hourlyActivity[hour].operations++;
                }

                if (operation == "signup")
                {
                    report.signups++;
                    if (status == "success")
                    {
                        report.signupSuccess++;
                    }
                    else
                    {
                        report.signupFailed++;
                        addFailureReason(report.failures, report.failureCount, reason);
                    }
                }
                else if (operation == "login")
                {
                    report.logins++;
                    if (status == "success")
                    {
                        report.loginSuccess++;
                    }
                    else
                    {
                        report.loginFailed++;
                        addFailureReason(report.failures, report.failureCount, reason);
                    }
                }
            }
        }

        file.close();

        int totalSuccess = report.signupSuccess + report.loginSuccess;
        if (report.totalOps > 0)
        {
            report.successRate = (totalSuccess * 100.0) / report.totalOps;
        }

        int totalFailures = report.signupFailed + report.loginFailed;
        for (int i = 0; i < report.failureCount; i++)
        {
            if (totalFailures > 0)
            {
                report.failures[i].percentage = (report.failures[i].count * 100.0) / totalFailures;
            }
        }

        return report;
    }

    WeeklyReport processWeeklyData()
    {
        WeeklyReport report;
        report.totalOps = 0;
        report.signups = 0;
        report.signupSuccess = 0;
        report.logins = 0;
        report.loginSuccess = 0;
        report.failureCount = 0;
        report.successRate = 0;

        time_t now = time(0);
        tm *ltm = localtime(&now);

        for (int i = 0; i < 7; i++)
        {
            report.dailyCounts[i] = 0;
            
            time_t dayTime = now - (6 - i) * 86400;
            tm *dayTm = localtime(&dayTime);
            
            char dateStr[11];
            sprintf(dateStr, "%04d-%02d-%02d", 
                    1900 + dayTm->tm_year, 
                    1 + dayTm->tm_mon, 
                    dayTm->tm_mday);
            report.dailyDates[i] = dateStr;
            
            if (i == 0) report.startDate = dateStr;
            if (i == 6) report.endDate = dateStr;
        }

        for (int i = 0; i < 20; i++)
        {
            report.failures[i].reason = "";
            report.failures[i].count = 0;
            report.failures[i].percentage = 0;
        }

        ifstream file("console_log.csv");
        if (!file)
        {
            return report;
        }

        string line;
        getline(file, line);

        while (getline(file, line))
        {
            string date, time, username, operation, status, reason;

            stringstream ss(line);
            getline(ss, date, ',');
            getline(ss, time, ',');
            getline(ss, username, ',');
            getline(ss, operation, ',');
            getline(ss, status, ',');
            getline(ss, reason, ',');

            for (int i = 0; i < 7; i++)
            {
                if (date == report.dailyDates[i])
                {
                    report.dailyCounts[i]++;
                    report.totalOps++;

                    if (operation == "signup")
                    {
                        report.signups++;
                        if (status == "success")
                        {
                            report.signupSuccess++;
                        }
                        else
                        {
                            addFailureReason(report.failures, report.failureCount, reason);
                        }
                    }
                    else if (operation == "login")
                    {
                        report.logins++;
                        if (status == "success")
                        {
                            report.loginSuccess++;
                        }
                        else
                        {
                            addFailureReason(report.failures, report.failureCount, reason);
                        }
                    }
                    break;
                }
            }
        }

        file.close();

        int totalSuccess = report.signupSuccess + report.loginSuccess;
        if (report.totalOps > 0)
        {
            report.successRate = (totalSuccess * 100.0) / report.totalOps;
        }

        int totalFailures = report.signups - report.signupSuccess + report.logins - report.loginSuccess;
        for (int i = 0; i < report.failureCount; i++)
        {
            if (totalFailures > 0)
            {
                report.failures[i].percentage = (report.failures[i].count * 100.0) / totalFailures;
            }
        }

        return report;
    }

    MonthlyReport processMonthlyData(int month, int year)
    {
        MonthlyReport report;
        report.month = month;
        report.year = year;
        report.totalOps = 0;
        report.signups = 0;
        report.signupSuccess = 0;
        report.logins = 0;
        report.loginSuccess = 0;
        report.failureCount = 0;
        report.successRate = 0;
        report.uniqueUsers = 0;
        report.peakDay = 0;
        report.peakDayOps = 0;
        report.dailyAverage = 0;

        for (int i = 0; i < 20; i++)
        {
            report.failures[i].reason = "";
            report.failures[i].count = 0;
            report.failures[i].percentage = 0;
        }

        int dailyOps[32];
        for (int i = 0; i < 32; i++) dailyOps[i] = 0;

        string usernames[1000];
        int userCount = 0;

        ifstream file("console_log.csv");
        if (!file)
        {
            return report;
        }

        string line;
        getline(file, line);

        while (getline(file, line))
        {
            string date, time, username, operation, status, reason;

            stringstream ss(line);
            getline(ss, date, ',');
            getline(ss, time, ',');
            getline(ss, username, ',');
            getline(ss, operation, ',');
            getline(ss, status, ',');
            getline(ss, reason, ',');

            int fileYear = (date[0] - '0') * 1000 + (date[1] - '0') * 100 + (date[2] - '0') * 10 + (date[3] - '0');
            int fileMonth = (date[5] - '0') * 10 + (date[6] - '0');
            int fileDay = (date[8] - '0') * 10 + (date[9] - '0');

            if (fileYear == year && fileMonth == month)
            {
                report.totalOps++;
                dailyOps[fileDay]++;

                bool found = false;
                for (int i = 0; i < userCount; i++)
                {
                    if (usernames[i] == username)
                    {
                        found = true;
                        break;
                    }
                }
                if (!found && userCount < 1000)
                {
                    usernames[userCount] = username;
                    userCount++;
                }

                if (operation == "signup")
                {
                    report.signups++;
                    if (status == "success")
                    {
                        report.signupSuccess++;
                    }
                    else
                    {
                        addFailureReason(report.failures, report.failureCount, reason);
                    }
                }
                else if (operation == "login")
                {
                    report.logins++;
                    if (status == "success")
                    {
                        report.loginSuccess++;
                    }
                    else
                    {
                        addFailureReason(report.failures, report.failureCount, reason);
                    }
                }
            }
        }

        file.close();

        report.uniqueUsers = userCount;

        for (int i = 1; i <= 31; i++)
        {
            if (dailyOps[i] > report.peakDayOps)
            {
                report.peakDayOps = dailyOps[i];
                report.peakDay = i;
            }
        }

        int daysInMonth = 30;
        if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
        {
            daysInMonth = 31;
        }
        else if (month == 2)
        {
            daysInMonth = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) ? 29 : 28;
        }

        if (daysInMonth > 0)
        {
            report.dailyAverage = (float)report.totalOps / daysInMonth;
        }

        int totalSuccess = report.signupSuccess + report.loginSuccess;
        if (report.totalOps > 0)
        {
            report.successRate = (totalSuccess * 100.0) / report.totalOps;
        }

        int totalFailures = report.signups - report.signupSuccess + report.logins - report.loginSuccess;
        for (int i = 0; i < report.failureCount; i++)
        {
            if (totalFailures > 0)
            {
                report.failures[i].percentage = (report.failures[i].count * 100.0) / totalFailures;
            }
        }

        return report;
    }

    void showDailyReportConsole(string date)
    {
        DailyReport report = processDailyData(date);

        cout << "\n========================================\n";
        cout << "     DAILY REPORT: " << report.date << "\n";
        cout << "========================================\n\n";

        cout << "Total Operations: " << report.totalOps << "\n\n";

        cout << "Signups: " << report.signups << "\n";
        cout << "  Success: " << report.signupSuccess << " (" << (report.signups > 0 ? (report.signupSuccess * 100.0 / report.signups) : 0) << "%)\n";
        cout << "  Failed: " << report.signupFailed << "\n\n";

        cout << "Logins: " << report.logins << "\n";
        cout << "  Success: " << report.loginSuccess << " (" << (report.logins > 0 ? (report.loginSuccess * 100.0 / report.logins) : 0) << "%)\n";
        cout << "  Failed: " << report.loginFailed << "\n\n";

        cout << "Overall Success Rate: " << report.successRate << "%\n";
        drawProgressBar(report.successRate);

        if (report.failureCount > 0)
        {
            cout << "\nFailure Breakdown:\n";
            for (int i = 0; i < report.failureCount; i++)
            {
                cout << "  " << (i + 1) << ". " << report.failures[i].reason 
                     << ": " << report.failures[i].count 
                     << " (" << report.failures[i].percentage << "%)\n";
            }
        }

        cout << "\nHourly Activity:\n";
        for (int i = 0; i < 24; i++)
        {
            if (report.hourlyActivity[i].operations > 0)
            {
                cout << "  " << (i < 10 ? "0" : "") << i << ":00 | ";
                for (int j = 0; j < report.hourlyActivity[i].operations; j++)
                {
                    cout << (char)219;
                }
                cout << " (" << report.hourlyActivity[i].operations << ")\n";
            }
        }

        cout << "\n========================================\n";
    }

    void showWeeklyReportConsole()
    {
        WeeklyReport report = processWeeklyData();

        cout << "\n========================================\n";
        cout << "  WEEKLY REPORT: " << report.startDate << " to " << report.endDate << "\n";
        cout << "========================================\n\n";

        cout << "Total Operations: " << report.totalOps << "\n\n";

        cout << "Daily Breakdown:\n";
        string dayNames[7] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
        for (int i = 0; i < 7; i++)
        {
            cout << "  " << dayNames[i] << " " << report.dailyDates[i] << ": " << report.dailyCounts[i] << "\n";
        }

        cout << "\nSignups: " << report.signups << " (Success: " << report.signupSuccess << ")\n";
        cout << "Logins: " << report.logins << " (Success: " << report.loginSuccess << ")\n";
        cout << "\nOverall Success Rate: " << report.successRate << "%\n";
        drawProgressBar(report.successRate);

        if (report.failureCount > 0)
        {
            cout << "\nTop Failure Reasons:\n";
            for (int i = 0; i < report.failureCount && i < 5; i++)
            {
                cout << "  " << (i + 1) << ". " << report.failures[i].reason 
                     << ": " << report.failures[i].count 
                     << " (" << report.failures[i].percentage << "%)\n";
            }
        }

        cout << "\n========================================\n";
    }

    void showMonthlyReportConsole(int month, int year)
    {
        MonthlyReport report = processMonthlyData(month, year);

        string monthNames[12] = {"January", "February", "March", "April", "May", "June",
                                 "July", "August", "September", "October", "November", "December"};

        cout << "\n========================================\n";
        cout << "  MONTHLY REPORT: " << monthNames[month - 1] << " " << year << "\n";
        cout << "========================================\n\n";

        cout << "Total Operations: " << report.totalOps << "\n";
        cout << "Daily Average: " << report.dailyAverage << " operations/day\n";
        cout << "Peak Day: Day " << report.peakDay << " (" << report.peakDayOps << " operations)\n";
        cout << "Unique Users: " << report.uniqueUsers << "\n\n";

        cout << "Signups: " << report.signups << "\n";
        cout << "  Success: " << report.signupSuccess << " (" << (report.signups > 0 ? (report.signupSuccess * 100.0 / report.signups) : 0) << "%)\n";
        cout << "  Failed: " << (report.signups - report.signupSuccess) << "\n\n";

        cout << "Logins: " << report.logins << "\n";
        cout << "  Success: " << report.loginSuccess << " (" << (report.logins > 0 ? (report.loginSuccess * 100.0 / report.logins) : 0) << "%)\n";
        cout << "  Failed: " << (report.logins - report.loginSuccess) << "\n\n";

        cout << "Overall Success Rate: " << report.successRate << "%\n";
        drawProgressBar(report.successRate);

        if (report.failureCount > 0)
        {
            cout << "\nFailure Reasons:\n";
            for (int i = 0; i < report.failureCount; i++)
            {
                cout << "  " << (i + 1) << ". " << report.failures[i].reason 
                     << ": " << report.failures[i].count 
                     << " (" << report.failures[i].percentage << "%)\n";
            }
        }

        cout << "\n========================================\n";
    }

private:
    void addFailureReason(FailureData failures[], int &count, string reason)
    {
        if (reason == "none") return;

        for (int i = 0; i < count; i++)
        {
            if (failures[i].reason == reason)
            {
                failures[i].count++;
                return;
            }
        }

        if (count < 20)
        {
            failures[count].reason = reason;
            failures[count].count = 1;
            count++;
        }
    }

    void drawProgressBar(float percentage)
    {
        int barLength = 40;
        int filled = (int)((percentage / 100.0) * barLength);

        cout << "[";
        for (int i = 0; i < barLength; i++)
        {
            if (i < filled)
            {
                cout << (char)219;
            }
            else
            {
                cout << (char)176;
            }
        }
        cout << "] " << percentage << "%\n";
    }
};

#endif