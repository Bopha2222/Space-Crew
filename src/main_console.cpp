#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include "login.hpp"
#include "game.hpp"
#include "leaderboard.hpp"
#include "analytics.hpp"
#include "user_manager.hpp"
#include "search.hpp"
#include "task_manager.hpp"

using namespace std;

void createCSVFiles()
{
    // Check and create user.csv with header
    ifstream checkUser("data/user.csv");
    if (!checkUser)
    {
        ofstream file("data/user.csv");
        file << "Username,PasswordHash,Deleted,IsAdmin\n";
        file.close();
    }
    else
    {
        checkUser.close();
    }

    // Check and create console_log.csv with header
    ifstream checkLog("data/console_log.csv");
    if (!checkLog)
    {
        ofstream file("data/console_log.csv");
        file << "Date,Time,Username,Operation,Status,Reason\n";
        file.close();
    }
    else
    {
        checkLog.close();
    }

    // Check and create game_results.csv with header
    ifstream checkGame("data/game_results.csv");
    if (!checkGame)
    {
        ofstream file("data/game_results.csv");
        file << "Username,Outcome\n";
        file.close();
    }
    else
    {
        checkGame.close();
    }

    // Check and create account_locks.csv with header
    ifstream checkLock("data/account_locks.csv");
    if (!checkLock)
    {
        ofstream file("data/account_locks.csv");
        file << "Username,LockTimestamp\n";
        file.close();
    }
    else
    {
        checkLock.close();
    }

    // Check and create task.csv with header and default tasks
    ifstream checkTask("data/task.csv");
    if (!checkTask)
    {
        ofstream file("data/task.csv");
        file << "TaskName\n";
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

    // Check and create taskpw.csv with header
    ifstream checkTaskPw("data/taskpw.csv");
    if (!checkTaskPw)
    {
        ofstream file("data/taskpw.csv");
        file << "Password\n";
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

    // Check and create taskroom.csv with header
    ifstream checkTaskRoom("data/taskroom.csv");
    if (!checkTaskRoom)
    {
        ofstream file("data/taskroom.csv");
        file << "RoomLocation\n";
        file << "Room 3\n";
        file << "Room 5\n";
        file << "Room 7\n";
        file << "Room 2\n";
        file << "Room 4\n";
        file << "Room 6\n";
        file << "Room 8\n";
        file << "Room 9\n";
        file.close();
    }
    else
    {
        checkTaskRoom.close();
    }
}

void logGameResult(string username, bool playerWon)
{
    ofstream file("data/game_results.csv", ios::app);
    if (!file) return;

    file << username << ",";
    file << (playerWon ? "Win" : "Lose") << "\n";

    file.close();
}

void showWelcome()
{
    cout << "\n";
    cout << "=====================================\n";
    cout << "       AMONG US - C++ EDITION        \n";
    cout << "=====================================\n";
    cout << " 1. Login\n";
    cout << " 2. Signup\n";
    cout << " 3. Exit\n";
    cout << "-------------------------------------\n";
    cout << "Choose option: ";
}

void showAdminMainMenu()
{
    cout << "\n=====================================\n";
    cout << "          ADMIN MAIN MENU            \n";
    cout << "=====================================\n";
    cout << " 1. Play Game\n";
    cout << " 2. Leaderboard\n";
    cout << " 3. View My Profile\n";
    cout << " 4. Analytics\n";
    cout << " 5. Admin Panel\n";
    cout << " 6. Search System\n";
    cout << " 7. Logout\n";
    cout << "-------------------------------------\n";
    cout << "Choose option: ";
}

void showGameMenu()
{
    cout << "\n=====================================\n";
    cout << "            PLAYER MENU              \n";
    cout << "=====================================\n";
    cout << " 1. Start Game\n";
    cout << " 2. Leaderboard\n";
    cout << " 3. View My Profile\n";
    cout << " 4. Logout\n";
    cout << "-------------------------------------\n";
    cout << "Choose option: ";
}

void showAnalyticsMenu()
{
    cout << "\n=====================================\n";
    cout << "          ANALYTICS MENU             \n";
    cout << "=====================================\n";
    cout << " 1. Daily Report (Today)\n";
    cout << " 2. Weekly Report (Last 7 Days)\n";
    cout << " 3. Monthly Report\n";
    cout << " 4. Back to Main Menu\n";
    cout << "-------------------------------------\n";
    cout << "Choose option: ";
}

void showAdminPanel()
{
    cout << "\n=====================================\n";
    cout << "           ADMIN PANEL               \n";
    cout << "=====================================\n";
    cout << " 1. Manage Tasks (CRUD)\n";
    cout << " 2. Manage Users\n";
    cout << " 3. System Statistics\n";
    cout << " 4. Back to Main Menu\n";
    cout << "-------------------------------------\n";
    cout << "Choose option: ";
}

void showTaskManagementMenu()
{
    cout << "\n=====================================\n";
    cout << "         TASK MANAGEMENT             \n";
    cout << "=====================================\n";
    cout << " 1. View All Tasks\n";
    cout << " 2. Add New Task (Auto-Assigned Room)\n";
    cout << " 3. Update Task (Name & Password)\n";
    cout << " 4. Delete Task\n";
    cout << " 5. Show Room Distribution\n";
    cout << " 6. Rebalance All Rooms\n";
    cout << " 7. Back\n";
    cout << "-------------------------------------\n";
    cout << "Choose option: ";
}

void showUserManagementMenu()
{
    cout << "\n=====================================\n";
    cout << "         USER MANAGEMENT             \n";
    cout << "=====================================\n";
    cout << " 1. List All Users\n";
    cout << " 2. View User Profile\n";
    cout << " 3. Soft Delete User\n";
    cout << " 4. Hard Delete User\n";
    cout << " 5. Restore User\n";
    cout << " 6. Back\n";
    cout << "-------------------------------------\n";
    cout << "Choose option: ";
}

void showSearchMenu()
{
    cout << "\n=====================================\n";
    cout << "           SEARCH SYSTEM             \n";
    cout << "=====================================\n";
    cout << " 1. Search by Username\n";
    cout << " 2. Search by Date\n";
    cout << " 3. Search by Operation\n";
    cout << " 4. Search by Status\n";
    cout << " 5. Search by Failure Reason\n";
    cout << " 6. Search by Date Range\n";
    cout << " 7. Back to Main Menu\n";
    cout << "-------------------------------------\n";
    cout << "Choose option: ";
}

string getTodayDate()
{
    time_t now = time(0);
    tm *lt = localtime(&now);
    char date[11];
    sprintf(date, "%04d-%02d-%02d", 1900 + lt->tm_year, 1 + lt->tm_mon, lt->tm_mday);
    return string(date);
}

void handleAnalytics()
{
    Analytics analytics;
    bool inAnalytics = true;

    while (inAnalytics)
    {
        showAnalyticsMenu();
        int choice;
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input!\n";
            continue;
        }

        cin.ignore();

        if (choice == 1)
        {
            // Auto-use today's date
            string todayDate = getTodayDate();
            cout << "\nShowing report for today: " << todayDate << "\n";
            analytics.showDailyReportConsole(todayDate);
            cout << "\nPress Enter to continue...";
            cin.get();
        }
        else if (choice == 2)
        {
            // Weekly report - auto last 7 days
            analytics.showWeeklyReportConsole();
            cout << "\nPress Enter to continue...";
            cin.get();
        }
        else if (choice == 3)
        {
            // Monthly report - ask for month and year
            cout << "\nEnter month (1-12): ";
            int month;
            cin >> month;
            cout << "Enter year (e.g., 2025): ";
            int year;
            cin >> year;

            if (cin.fail() || month < 1 || month > 12 || year < 2000 || year > 2100)
            {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid input!\n";
                continue;
            }

            cin.ignore();
            analytics.showMonthlyReportConsole(month, year);
            cout << "\nPress Enter to continue...";
            cin.get();
        }
        else if (choice == 4)
        {
            inAnalytics = false;
        }
        else
        {
            cout << "Invalid option!\n";
        }
    }
}

void handleAdminPanel()
{
    UserManager userManager;
    TaskManager taskManager;
    bool inPanel = true;

    while (inPanel)
    {
        showAdminPanel();
        int choice;
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input!\n";
            continue;
        }

        cin.ignore();

        if (choice == 1)
        {
            // Task Management
            bool inTaskMgmt = true;
            while (inTaskMgmt)
            {
                showTaskManagementMenu();
                int taskChoice;
                cin >> taskChoice;

                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Invalid input!\n";
                    continue;
                }

                cin.ignore();

                if (taskChoice == 1)
                {
                    taskManager.listAllTasks();
                    cout << "\nPress Enter to continue...";
                    cin.get();
                }
                else if (taskChoice == 2)
                {
                    cout << "\nEnter new task name: ";
                    string taskName;
                    getline(cin, taskName);
                    cout << "Enter task password: ";
                    string password;
                    getline(cin, password);
                    taskManager.addTask(taskName, password);
                    cout << "\nPress Enter to continue...";
                    cin.get();
                }
                else if (taskChoice == 3)
                {
                    taskManager.listAllTasks();
                    cout << "\nWhich task to update? Enter task number (1-" << taskManager.tasks.length() << "): ";
                    int taskNum;
                    cin >> taskNum;
                    cin.ignore();

                    if (taskNum < 1 || taskNum > taskManager.tasks.length())
                    {
                        cout << "Task data not found in system.\n";
                        cout << "\nPress Enter to continue...";
                        cin.get();
                        continue;
                    }

                    string oldTaskName = taskManager.tasks.getAt(taskNum - 1);
                    cout << "Enter new task name: ";
                    string newTaskName;
                    getline(cin, newTaskName);
                    cout << "Enter new password: ";
                    string newPassword;
                    getline(cin, newPassword);
                    taskManager.updateTask(oldTaskName, newTaskName, newPassword);
                    cout << "\nPress Enter to continue...";
                    cin.get();
                }
                else if (taskChoice == 4)
                {
                    taskManager.listAllTasks();
                    cout << "\nWhich task to delete? Enter task number (1-" << taskManager.tasks.length() << "): ";
                    int taskNum;
                    cin >> taskNum;
                    cin.ignore();

                    if (taskNum < 1 || taskNum > taskManager.tasks.length())
                    {
                        cout << "Task data not found in system.\n";
                        cout << "\nPress Enter to continue...";
                        cin.get();
                        continue;
                    }

                    string taskName = taskManager.tasks.getAt(taskNum - 1);
                    taskManager.deleteTask(taskName);
                    cout << "\nPress Enter to continue...";
                    cin.get();
                }
                else if (taskChoice == 5)
                {
                    taskManager.showRoomDistribution();
                    cout << "\nPress Enter to continue...";
                    cin.get();
                }
                else if (taskChoice == 6)
                {
                    taskManager.rebalanceAllRooms();
                    cout << "\nPress Enter to continue...";
                    cin.get();
                }
                else if (taskChoice == 7)
                {
                    inTaskMgmt = false;
                }
                else
                {
                    cout << "Invalid option!\n";
                }
            }
        }
        else if (choice == 2)
        {
            // User Management
            bool inUserMgmt = true;
            while (inUserMgmt)
            {
                showUserManagementMenu();
                int userChoice;
                cin >> userChoice;

                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Invalid input!\n";
                    continue;
                }

                cin.ignore();

                if (userChoice == 1)
                {
                    userManager.listAllUsers();
                    cout << "\nPress Enter to continue...";
                    cin.get();
                }
                else if (userChoice == 2)
                {
                    cout << "\nEnter username: ";
                    string username;
                    getline(cin, username);
                    userManager.viewUserProfile(username);
                    cout << "\nPress Enter to continue...";
                    cin.get();
                }
                else if (userChoice == 3)
                {
                    cout << "\nEnter username to soft delete: ";
                    string username;
                    getline(cin, username);
                    userManager.softDeleteUser(username);
                    cout << "\nPress Enter to continue...";
                    cin.get();
                }
                else if (userChoice == 4)
                {
                    cout << "\nEnter username to hard delete: ";
                    string username;
                    getline(cin, username);
                    userManager.hardDeleteUser(username);
                    cout << "\nPress Enter to continue...";
                    cin.get();
                }
                else if (userChoice == 5)
                {
                    cout << "\nEnter username to restore: ";
                    string username;
                    getline(cin, username);
                    userManager.restoreUser(username);
                    cout << "\nPress Enter to continue...";
                    cin.get();
                }
                else if (userChoice == 6)
                {
                    inUserMgmt = false;
                }
                else
                {
                    cout << "Invalid option!\n";
                }
            }
        }
        else if (choice == 3)
        {
            // System Statistics
            int activeUsers = userManager.countActiveUsers();
            int deletedUsers = userManager.countDeletedUsers();

            cout << "\n========================================\n";
            cout << "       SYSTEM STATISTICS\n";
            cout << "========================================\n";
            cout << "Active Users: " << activeUsers << "\n";
            cout << "Deleted Users: " << deletedUsers << "\n";
            cout << "Total Tasks: " << taskManager.tasks.length() << "\n";
            cout << "========================================\n\n";

            cout << "Press Enter to continue...";
            cin.get();
        }
        else if (choice == 4)
        {
            inPanel = false;
        }
        else
        {
            cout << "Invalid option!\n";
        }
    }
}

void handleSearch()
{
    SearchSystem search;
    bool inSearch = true;

    while (inSearch)
    {
        showSearchMenu();
        int choice;
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input!\n";
            continue;
        }

        cin.ignore();

        if (choice == 1)
        {
            cout << "\nEnter username: ";
            string username;
            getline(cin, username);
            search.searchByUsername(username);
            cout << "\nPress Enter to continue...";
            cin.get();
        }
        else if (choice == 2)
        {
            cout << "\nEnter date (YYYY-MM-DD): ";
            string date;
            getline(cin, date);
            search.searchByDate(date);
            cout << "\nPress Enter to continue...";
            cin.get();
        }
        else if (choice == 3)
        {
            cout << "\nEnter operation (login/signup): ";
            string operation;
            getline(cin, operation);
            search.searchByOperation(operation);
            cout << "\nPress Enter to continue...";
            cin.get();
        }
        else if (choice == 4)
        {
            cout << "\nEnter status (success/fail): ";
            string status;
            getline(cin, status);
            search.searchByStatus(status);
            cout << "\nPress Enter to continue...";
            cin.get();
        }
        else if (choice == 5)
        {
            cout << "\nEnter failure reason: ";
            string reason;
            getline(cin, reason);
            search.searchByFailureReason(reason);
            cout << "\nPress Enter to continue...";
            cin.get();
        }
        else if (choice == 6)
        {
            cout << "\nEnter start date (YYYY-MM-DD): ";
            string startDate;
            getline(cin, startDate);
            cout << "Enter end date (YYYY-MM-DD): ";
            string endDate;
            getline(cin, endDate);
            search.searchByDateRange(startDate, endDate);
            cout << "\nPress Enter to continue...";
            cin.get();
        }
        else if (choice == 7)
        {
            inSearch = false;
        }
        else
        {
            cout << "Invalid option!\n";
        }
    }
}

void handleGameMenu(string username)
{
    Leaderboard leaderboard;
    UserManager userManager;
    LoginSystem loginSystem;
    bool loggedIn = true;
    bool isUserAdmin = loginSystem.isAdmin(username);

    while (loggedIn)
    {
        if (isUserAdmin)
        {
            showAdminMainMenu();
        }
        else
        {
            showGameMenu();
        }
        
        int choice;
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input!\n";
            continue;
        }

        cin.ignore();

        if (choice == 1)
        {
            cout << "\nStarting game...\n";
            Game game;
            game.player.name = username;
            game.selectDifficulty();
            game.startGame();

            logGameResult(username, game.playerWon);
        }
        else if (choice == 2)
        {
            cout << "\nShowing leaderboard...\n";
            leaderboard.show();
            cout << "\nPress Enter to continue...";
            cin.get();
        }
        else if (choice == 3)
        {
            userManager.viewUserProfile(username);
            cout << "\nPress Enter to continue...";
            cin.get();
        }
        else if (choice == 4)
        {
            if (isUserAdmin)
            {
                handleAnalytics();
            }
            else
            {
                cout << "\nLogged out.\n";
                loggedIn = false;
            }
        }
        else if (choice == 5)
        {
            if (isUserAdmin)
            {
                handleAdminPanel();
            }
            else
            {
                cout << "Invalid option!\n";
            }
        }
        else if (choice == 6)
        {
            if (isUserAdmin)
            {
                handleSearch();
            }
            else
            {
                cout << "Invalid option!\n";
            }
        }
        else if (choice == 7)
        {
            if (isUserAdmin)
            {
                cout << "\nLogged out.\n";
                loggedIn = false;
            }
            else
            {
                cout << "Invalid option!\n";
            }
        }
        else
        {
            cout << "Invalid option!\n";
        }
    }
}

int main()
{
    createCSVFiles();

    LoginSystem loginSystem;

    while (true)
    {
        showWelcome();
        int choice;
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input!\n\n";
            continue;
        }

        cin.ignore();

        if (choice == 1)
        {
            cout << "\nEnter username (case-sensitive): ";
            string username;
            getline(cin, username);

            // Check if account is already locked FIRST
            if (loginSystem.isAccountLocked(username))
            {
                continue; // Skip login attempts, go back to main menu
            }

            int failedAttempts = 0;
            bool loginSuccess = false;

            while (failedAttempts < 3)
            {
                cout << "Enter password: ";
                string password = loginSystem.getHiddenPassword();

                if (loginSystem.login(username, password))
                {
                    cout << "\nLogin successful! Welcome, " << username << "!\n";
                    loginSuccess = true;
                    break;
                }
                else
                {
                    failedAttempts++;
                    if (failedAttempts < 3)
                    {
                        cout << "Failed attempts: " << failedAttempts << "/3\n";
                        cout << "Try again.\n";
                    }
                    else
                    {
                        // Reached 3 failed attempts
                        loginSystem.lockAccount(username);
                        break; // Exit the loop immediately
                    }
                }
            }

            if (loginSuccess)
            {
                handleGameMenu(username);
            }
        }
        else if (choice == 2)
        {
            cout << "\nEnter username (4+ chars, letters/numbers): ";
            string username;
            getline(cin, username);

            if (loginSystem.usernameExists(username))
            {
                cout << "Username already exists! Try another.\n\n";
                continue;
            }

            cout << "Enter password (8+ chars, mixed case, number, special char): ";
            string password = loginSystem.getHiddenPassword();

            if (loginSystem.signup(username, password))
            {
                handleGameMenu(username);
            }
        }
        else if (choice == 3)
        {
            cout << "\nExiting program. Goodbye!\n";
            break;
        }
        else
        {
            cout << "Invalid option!\n\n";
        }
    }

    return 0;
}