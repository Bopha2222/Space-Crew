#ifndef GUIMANAGER_HPP
#define GUIMANAGER_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <string>
#include <memory>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include "Button.hpp"
#include "TextBox.hpp"
#include "login.hpp"
#include "leaderboard.hpp"
#include "game.hpp"

using namespace std;

enum class GameState
{
    WELCOME,
    LOGIN,
    SIGNUP,
    MAIN_MENU,
    ADMIN_MENU,
    GAME_DIFFICULTY,
    CUSTOM_DIFFICULTY_CONFIG,
    GAME_PLAY,
    LEADERBOARD,
    USER_PROFILE,
    ANALYTICS,
    ADMIN_PANEL,
    TASK_MANAGEMENT,
    USER_MANAGEMENT,
    SEARCH_SYSTEM,
    MEETING,
    GAME_OVER
};

class GUIManager
{
private:
    sf::RenderWindow window;
    sf::Font font;
    GameState currentState;
    GameState previousState;
    
    string currentUsername;
    bool isAdmin;
    
    const int WINDOW_WIDTH = 1280;
    const int WINDOW_HEIGHT = 720;

    // Login Screen Components
    unique_ptr<TextBox> loginUsernameBox;
    unique_ptr<TextBox> loginPasswordBox;
    unique_ptr<Button> loginButton;
    unique_ptr<Button> signupButton;
    string loginErrorMessage;

    // Signup Screen Components
    unique_ptr<TextBox> signupUsernameBox;
    unique_ptr<TextBox> signupPasswordBox;
    unique_ptr<TextBox> signupConfirmPasswordBox;
    unique_ptr<Button> signupCreateButton;
    unique_ptr<Button> signupBackButton;
    string signupErrorMessage;
    string signupSuccessMessage;

    // Main Menu Components
    unique_ptr<Button> playGameButton;
    unique_ptr<Button> leaderboardButton;
    unique_ptr<Button> profileButton;
    unique_ptr<Button> logoutButton;

    // Admin Menu Components
    unique_ptr<Button> adminAnalyticsButton;
    unique_ptr<Button> adminTaskManagementButton;
    unique_ptr<Button> adminUserManagementButton;
    unique_ptr<Button> adminSearchButton;
    unique_ptr<Button> adminPlayGameButton;
    unique_ptr<Button> adminLogoutButton;

    // Difficulty Screen Components
    unique_ptr<Button> easyButton;
    unique_ptr<Button> mediumButton;
    unique_ptr<Button> hardButton;
    unique_ptr<Button> customButton;
    unique_ptr<Button> difficultyBackButton;

    // Custom Difficulty Config Components
    unique_ptr<TextBox> customTasksBox;
    unique_ptr<TextBox> customCooldownBox;
    unique_ptr<Button> customStartButton;
    unique_ptr<Button> customBackButton;
    string customConfigError;

    // Leaderboard Screen Components
    unique_ptr<Button> leaderboardBackButton;

    // Profile Screen Components
    unique_ptr<Button> profileBackButton;

    // Analytics Screen Components
    unique_ptr<Button> analyticsBackButton;
    int analyticsPage;
    vector<string> analyticsData;

    // Task Management Components
    unique_ptr<Button> taskManagementBackButton;
    vector<string> taskList;

    // User Management Components
    unique_ptr<Button> userManagementBackButton;
    vector<string> userList;

    // Search System Components
    unique_ptr<TextBox> searchQueryBox;
    unique_ptr<Button> searchButton;
    unique_ptr<Button> searchBackButton;
    vector<string> searchResults;
    string searchStatus;

    // Gameplay Components
    unique_ptr<Game> currentGame;
    unique_ptr<Button> moveNorthButton;
    unique_ptr<Button> moveWestButton;
    unique_ptr<Button> moveSouthButton;
    unique_ptr<Button> moveEastButton;
    unique_ptr<Button> doTaskButton;
    unique_ptr<Button> killButton;
    unique_ptr<Button> meetingButton;
    unique_ptr<Button> gameQuitButton;
    int selectedDifficulty;
    int lastPlayerKillRound;

    // Dialog Components
    bool showTaskDialog;
    bool showKillDialog;
    unique_ptr<TextBox> taskPasswordBox;
    unique_ptr<Button> taskSubmitButton;
    unique_ptr<Button> taskCancelButton;
    vector<unique_ptr<Button>> killTargetButtons;
    string dialogMessage;

    // Meeting/Voting Components
    unique_ptr<TextBox> voteInputBox;
    unique_ptr<Button> voteSubmitButton;
    unique_ptr<Button> voteSkipButton;
    string votingResult;

    // Game Over Components
    unique_ptr<Button> gameOverBackButton;
    unique_ptr<Button> playAgainButton;
    
public:
    GUIManager()
    {
        currentState = GameState::WELCOME;
        previousState = GameState::WELCOME;
        isAdmin = false;
        loginErrorMessage = "";
        analyticsPage = 0;
    }
    
    ~GUIManager()
    {
    }
    
    bool initialize()
    {
        window.create(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), 
                      "Among Us - C++ Edition",
                      sf::Style::Titlebar | sf::Style::Close);
        window.setFramerateLimit(60);
        
        if (!font.openFromFile("fonts/Arial.ttf"))
        {
            cerr << "ERROR: Failed to load font!" << endl;
            return false;
        }
        
        // Initialize login screen components
        loginUsernameBox = make_unique<TextBox>(440, 280, 400, 50, "Username", font, false);
        loginPasswordBox = make_unique<TextBox>(440, 360, 400, 50, "Password", font, true);
        loginButton = make_unique<Button>(490, 450, 140, 50, "Login", font, 24);
        signupButton = make_unique<Button>(650, 450, 140, 50, "Sign Up", font, 24);
        
        loginButton->setCallback([this]() {
            handleLoginAttempt();
        });
        
        signupButton->setCallback([this]() {
            setState(GameState::SIGNUP);
        });
        
        // Initialize signup screen components
        signupUsernameBox = make_unique<TextBox>(440, 240, 400, 50, "Username", font, false);
        signupPasswordBox = make_unique<TextBox>(440, 320, 400, 50, "Password", font, true);
        signupConfirmPasswordBox = make_unique<TextBox>(440, 400, 400, 50, "Confirm Password", font, true);
        signupCreateButton = make_unique<Button>(490, 490, 140, 50, "Create", font, 24);
        signupBackButton = make_unique<Button>(650, 490, 140, 50, "Back", font, 24);

        signupCreateButton->setCallback([this]() {
            handleSignupAttempt();
        });

        signupBackButton->setCallback([this]() {
            signupErrorMessage = "";
            signupSuccessMessage = "";
            signupUsernameBox->clear();
            signupPasswordBox->clear();
            signupConfirmPasswordBox->clear();
            setState(GameState::LOGIN);
        });

        // Initialize main menu buttons
        playGameButton = make_unique<Button>(490, 250, 300, 60, "Play Game", font, 28);
        leaderboardButton = make_unique<Button>(490, 330, 300, 60, "Leaderboard", font, 28);
        profileButton = make_unique<Button>(490, 410, 300, 60, "My Profile", font, 28);
        logoutButton = make_unique<Button>(490, 490, 300, 60, "Logout", font, 28);

        playGameButton->setCallback([this]() {
            setState(GameState::GAME_DIFFICULTY);
        });

        leaderboardButton->setCallback([this]() {
            setState(GameState::LEADERBOARD);
        });

        profileButton->setCallback([this]() {
            setState(GameState::USER_PROFILE);
        });

        logoutButton->setCallback([this]() {
            currentUsername = "";
            isAdmin = false;
            loginUsernameBox->clear();
            loginPasswordBox->clear();
            loginErrorMessage = "";
            setState(GameState::LOGIN);
        });

        // Initialize admin menu buttons
        adminAnalyticsButton = make_unique<Button>(440, 200, 400, 55, "Analytics", font, 26);
        adminTaskManagementButton = make_unique<Button>(440, 270, 400, 55, "Task Management", font, 26);
        adminUserManagementButton = make_unique<Button>(440, 340, 400, 55, "User Management", font, 26);
        adminSearchButton = make_unique<Button>(440, 410, 400, 55, "Search System", font, 26);
        adminPlayGameButton = make_unique<Button>(440, 480, 400, 55, "Play Game", font, 26);
        adminLogoutButton = make_unique<Button>(440, 550, 400, 55, "Logout", font, 26);

        adminAnalyticsButton->setCallback([this]() {
            loadAnalyticsData();
            setState(GameState::ANALYTICS);
        });

        adminTaskManagementButton->setCallback([this]() {
            loadTaskList();
            setState(GameState::TASK_MANAGEMENT);
        });

        adminUserManagementButton->setCallback([this]() {
            loadUserList();
            setState(GameState::USER_MANAGEMENT);
        });

        adminSearchButton->setCallback([this]() {
            setState(GameState::SEARCH_SYSTEM);
        });

        adminPlayGameButton->setCallback([this]() {
            setState(GameState::GAME_DIFFICULTY);
        });

        adminLogoutButton->setCallback([this]() {
            currentUsername = "";
            isAdmin = false;
            loginUsernameBox->clear();
            loginPasswordBox->clear();
            loginErrorMessage = "";
            setState(GameState::LOGIN);
        });

        // Initialize difficulty screen buttons
        easyButton = make_unique<Button>(490, 220, 300, 60, "Easy", font, 28);
        mediumButton = make_unique<Button>(490, 295, 300, 60, "Medium", font, 28);
        hardButton = make_unique<Button>(490, 370, 300, 60, "Hard", font, 28);
        customButton = make_unique<Button>(490, 445, 300, 60, "Custom", font, 28);
        difficultyBackButton = make_unique<Button>(490, 540, 300, 50, "Back", font, 24);

        easyButton->setCallback([this]() {
            selectedDifficulty = 1;
            startNewGame();
        });

        mediumButton->setCallback([this]() {
            selectedDifficulty = 2;
            startNewGame();
        });

        hardButton->setCallback([this]() {
            selectedDifficulty = 3;
            startNewGame();
        });

        customButton->setCallback([this]() {
            setState(GameState::CUSTOM_DIFFICULTY_CONFIG);
        });

        difficultyBackButton->setCallback([this]() {
            if (isAdmin) {
                setState(GameState::ADMIN_MENU);
            } else {
                setState(GameState::MAIN_MENU);
            }
        });

        // Initialize custom difficulty config
        customTasksBox = make_unique<TextBox>(440, 280, 400, 50, "Number of Tasks (1-8)", font, false);
        customCooldownBox = make_unique<TextBox>(440, 360, 400, 50, "Kill Cooldown (1-10)", font, false);
        customStartButton = make_unique<Button>(490, 450, 140, 50, "Start", font, 24);
        customBackButton = make_unique<Button>(650, 450, 140, 50, "Back", font, 24);

        customStartButton->setCallback([this]() {
            handleCustomDifficultyStart();
        });

        customBackButton->setCallback([this]() {
            customTasksBox->clear();
            customCooldownBox->clear();
            customConfigError = "";
            setState(GameState::GAME_DIFFICULTY);
        });

        // Initialize leaderboard screen
        leaderboardBackButton = make_unique<Button>(490, 630, 300, 50, "Back", font, 24);

        leaderboardBackButton->setCallback([this]() {
            if (isAdmin) {
                setState(GameState::ADMIN_MENU);
            } else {
                setState(GameState::MAIN_MENU);
            }
        });

        // Initialize profile screen
        profileBackButton = make_unique<Button>(490, 630, 300, 50, "Back", font, 24);

        profileBackButton->setCallback([this]() {
            if (isAdmin) {
                setState(GameState::ADMIN_MENU);
            } else {
                setState(GameState::MAIN_MENU);
            }
        });

        // Initialize analytics screen
        analyticsBackButton = make_unique<Button>(490, 640, 300, 50, "Back", font, 24);

        analyticsBackButton->setCallback([this]() {
            setState(GameState::ADMIN_MENU);
        });

        // Initialize task management screen
        taskManagementBackButton = make_unique<Button>(490, 640, 300, 50, "Back", font, 24);

        taskManagementBackButton->setCallback([this]() {
            setState(GameState::ADMIN_MENU);
        });

        // Initialize user management screen
        userManagementBackButton = make_unique<Button>(490, 640, 300, 50, "Back", font, 24);

        userManagementBackButton->setCallback([this]() {
            setState(GameState::ADMIN_MENU);
        });

        // Initialize search system
        searchQueryBox = make_unique<TextBox>(340, 150, 600, 50, "Enter username or date to search", font, false);
        searchButton = make_unique<Button>(540, 220, 200, 50, "Search", font, 24);
        searchBackButton = make_unique<Button>(490, 640, 300, 50, "Back", font, 24);

        searchButton->setCallback([this]() {
            performSearch();
        });

        searchBackButton->setCallback([this]() {
            searchQueryBox->clear();
            searchResults.clear();
            searchStatus = "";
            setState(GameState::ADMIN_MENU);
        });

        // Initialize gameplay buttons
        selectedDifficulty = 1;
        lastPlayerKillRound = -1000;

        moveNorthButton = make_unique<Button>(540, 500, 200, 50, "Move North (W)", font, 20);
        moveWestButton = make_unique<Button>(320, 560, 200, 50, "Move West (A)", font, 20);
        moveSouthButton = make_unique<Button>(540, 560, 200, 50, "Move South (S)", font, 20);
        moveEastButton = make_unique<Button>(760, 560, 200, 50, "Move East (D)", font, 20);
        doTaskButton = make_unique<Button>(100, 500, 180, 50, "Do Task (T)", font, 20);
        killButton = make_unique<Button>(100, 560, 180, 50, "Kill (K)", font, 20);
        meetingButton = make_unique<Button>(100, 620, 180, 50, "Meeting (M)", font, 18);
        gameQuitButton = make_unique<Button>(980, 620, 180, 50, "Quit Game", font, 20);

        moveNorthButton->setCallback([this]() { handleMove('W'); });
        moveWestButton->setCallback([this]() { handleMove('A'); });
        moveSouthButton->setCallback([this]() { handleMove('S'); });
        moveEastButton->setCallback([this]() { handleMove('D'); });
        doTaskButton->setCallback([this]() { handleDoTask(); });
        killButton->setCallback([this]() { handleKill(); });
        meetingButton->setCallback([this]() { handleMeeting(); });
        gameQuitButton->setCallback([this]() {
            currentGame.reset();
            if (isAdmin) {
                setState(GameState::ADMIN_MENU);
            } else {
                setState(GameState::MAIN_MENU);
            }
        });

        // Initialize dialog components
        showTaskDialog = false;
        showKillDialog = false;

        taskPasswordBox = make_unique<TextBox>(440, 320, 400, 50, "Enter Task Password", font, false);
        taskSubmitButton = make_unique<Button>(490, 400, 140, 50, "Submit", font, 24);
        taskCancelButton = make_unique<Button>(650, 400, 140, 50, "Cancel", font, 24);

        taskSubmitButton->setCallback([this]() {
            submitTaskPassword();
        });

        taskCancelButton->setCallback([this]() {
            showTaskDialog = false;
            taskPasswordBox->clear();
            dialogMessage = "";
        });

        // Initialize meeting/voting components
        voteInputBox = make_unique<TextBox>(440, 450, 400, 50, "Vote for player name (or 'skip')", font, false);
        voteSubmitButton = make_unique<Button>(490, 530, 140, 50, "Vote", font, 24);
        voteSkipButton = make_unique<Button>(650, 530, 140, 50, "Skip", font, 24);

        voteSubmitButton->setCallback([this]() {
            submitVote();
        });

        voteSkipButton->setCallback([this]() {
            voteInputBox->setText("skip");
            submitVote();
        });

        // Initialize game over components
        gameOverBackButton = make_unique<Button>(440, 500, 180, 60, "Back to Menu", font, 24);
        playAgainButton = make_unique<Button>(660, 500, 180, 60, "Play Again", font, 24);

        gameOverBackButton->setCallback([this]() {
            currentGame.reset();
            if (isAdmin) {
                setState(GameState::ADMIN_MENU);
            } else {
                setState(GameState::MAIN_MENU);
            }
        });

        playAgainButton->setCallback([this]() {
            setState(GameState::GAME_DIFFICULTY);
        });

        cout << "GUI initialized successfully!" << endl;
        return true;
    }
    
    void run()
    {
        sf::Clock clock;
        
        while (window.isOpen())
        {
            float deltaTime = clock.restart().asSeconds();
            
            handleEvents();
            update(deltaTime);
            render();
        }
    }
    
    void setState(GameState newState)
    {
        previousState = currentState;
        currentState = newState;
    }
    
    GameState getState() const 
    { 
        return currentState; 
    }
    
    void setUsername(const string& username) 
    { 
        currentUsername = username; 
    }
    
    string getUsername() const 
    { 
        return currentUsername; 
    }
    
    void setAdmin(bool admin) 
    { 
        isAdmin = admin; 
    }
    
    bool getAdmin() const 
    { 
        return isAdmin; 
    }
    
    sf::RenderWindow& getWindow() 
    { 
        return window; 
    }
    
    sf::Font& getFont() 
    { 
        return font; 
    }
    
private:
    void handleEvents()
    {
        while (const optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            
            if (event->is<sf::Event::KeyPressed>())
            {
                const auto& keyPress = *event->getIf<sf::Event::KeyPressed>();
                if (keyPress.code == sf::Keyboard::Key::Escape)
                {
                    window.close();
                }
            }
            
            if (currentState == GameState::WELCOME)
            {
                if (event->is<sf::Event::MouseButtonPressed>())
                {
                    setState(GameState::LOGIN);
                }
            }
            else if (currentState == GameState::LOGIN)
            {
                if (loginUsernameBox) loginUsernameBox->handleEvent(*event, window);
                if (loginPasswordBox) loginPasswordBox->handleEvent(*event, window);
                if (loginButton) loginButton->handleEvent(*event, window);
                if (signupButton) signupButton->handleEvent(*event, window);
            }
            else if (currentState == GameState::SIGNUP)
            {
                if (signupUsernameBox) signupUsernameBox->handleEvent(*event, window);
                if (signupPasswordBox) signupPasswordBox->handleEvent(*event, window);
                if (signupConfirmPasswordBox) signupConfirmPasswordBox->handleEvent(*event, window);
                if (signupCreateButton) signupCreateButton->handleEvent(*event, window);
                if (signupBackButton) signupBackButton->handleEvent(*event, window);
            }
            else if (currentState == GameState::MAIN_MENU)
            {
                if (playGameButton) playGameButton->handleEvent(*event, window);
                if (leaderboardButton) leaderboardButton->handleEvent(*event, window);
                if (profileButton) profileButton->handleEvent(*event, window);
                if (logoutButton) logoutButton->handleEvent(*event, window);
            }
            else if (currentState == GameState::ADMIN_MENU)
            {
                if (adminAnalyticsButton) adminAnalyticsButton->handleEvent(*event, window);
                if (adminTaskManagementButton) adminTaskManagementButton->handleEvent(*event, window);
                if (adminUserManagementButton) adminUserManagementButton->handleEvent(*event, window);
                if (adminSearchButton) adminSearchButton->handleEvent(*event, window);
                if (adminPlayGameButton) adminPlayGameButton->handleEvent(*event, window);
                if (adminLogoutButton) adminLogoutButton->handleEvent(*event, window);
            }
            else if (currentState == GameState::GAME_DIFFICULTY)
            {
                if (easyButton) easyButton->handleEvent(*event, window);
                if (mediumButton) mediumButton->handleEvent(*event, window);
                if (hardButton) hardButton->handleEvent(*event, window);
                if (customButton) customButton->handleEvent(*event, window);
                if (difficultyBackButton) difficultyBackButton->handleEvent(*event, window);
            }
            else if (currentState == GameState::CUSTOM_DIFFICULTY_CONFIG)
            {
                if (customTasksBox) customTasksBox->handleEvent(*event, window);
                if (customCooldownBox) customCooldownBox->handleEvent(*event, window);
                if (customStartButton) customStartButton->handleEvent(*event, window);
                if (customBackButton) customBackButton->handleEvent(*event, window);
            }
            else if (currentState == GameState::LEADERBOARD)
            {
                if (leaderboardBackButton) leaderboardBackButton->handleEvent(*event, window);
            }
            else if (currentState == GameState::USER_PROFILE)
            {
                if (profileBackButton) profileBackButton->handleEvent(*event, window);
            }
            else if (currentState == GameState::ANALYTICS)
            {
                if (analyticsBackButton) analyticsBackButton->handleEvent(*event, window);
            }
            else if (currentState == GameState::TASK_MANAGEMENT)
            {
                if (taskManagementBackButton) taskManagementBackButton->handleEvent(*event, window);
            }
            else if (currentState == GameState::USER_MANAGEMENT)
            {
                if (userManagementBackButton) userManagementBackButton->handleEvent(*event, window);
            }
            else if (currentState == GameState::SEARCH_SYSTEM)
            {
                if (searchQueryBox) searchQueryBox->handleEvent(*event, window);
                if (searchButton) searchButton->handleEvent(*event, window);
                if (searchBackButton) searchBackButton->handleEvent(*event, window);
            }
            else if (currentState == GameState::GAME_PLAY)
            {
                if (showTaskDialog)
                {
                    if (taskPasswordBox) taskPasswordBox->handleEvent(*event, window);
                    if (taskSubmitButton) taskSubmitButton->handleEvent(*event, window);
                    if (taskCancelButton) taskCancelButton->handleEvent(*event, window);
                }
                else if (showKillDialog)
                {
                    for (auto& btn : killTargetButtons)
                    {
                        if (btn) btn->handleEvent(*event, window);
                    }
                }
                else if (!currentGame || currentGame->player.isDead || currentGame->gameOver)
                {
                    if (gameQuitButton) gameQuitButton->handleEvent(*event, window);
                }
                else
                {
                    if (moveNorthButton) moveNorthButton->handleEvent(*event, window);
                    if (moveWestButton) moveWestButton->handleEvent(*event, window);
                    if (moveSouthButton) moveSouthButton->handleEvent(*event, window);
                    if (moveEastButton) moveEastButton->handleEvent(*event, window);
                    
                    if (!currentGame->player.isImpostor && doTaskButton) {
                        doTaskButton->handleEvent(*event, window);
                    }
                    
                    if (currentGame->player.isImpostor && killButton) {
                        killButton->handleEvent(*event, window);
                    }
                    
                    if (meetingButton) meetingButton->handleEvent(*event, window);
                    if (gameQuitButton) gameQuitButton->handleEvent(*event, window);
                }
            }
            else if (currentState == GameState::MEETING)
            {
                if (voteInputBox) voteInputBox->handleEvent(*event, window);
                if (voteSubmitButton) voteSubmitButton->handleEvent(*event, window);
                if (voteSkipButton) voteSkipButton->handleEvent(*event, window);
            }
            else if (currentState == GameState::GAME_OVER)
            {
                if (gameOverBackButton) gameOverBackButton->handleEvent(*event, window);
                if (playAgainButton) playAgainButton->handleEvent(*event, window);
            }
        }
    }
    
    void update(float deltaTime)
    {
        if (currentState == GameState::LOGIN)
        {
            if (loginUsernameBox) loginUsernameBox->update(deltaTime);
            if (loginPasswordBox) loginPasswordBox->update(deltaTime);
            if (loginButton) loginButton->update(window);
            if (signupButton) signupButton->update(window);
        }
        else if (currentState == GameState::SIGNUP)
        {
            if (signupUsernameBox) signupUsernameBox->update(deltaTime);
            if (signupPasswordBox) signupPasswordBox->update(deltaTime);
            if (signupConfirmPasswordBox) signupConfirmPasswordBox->update(deltaTime);
            if (signupCreateButton) signupCreateButton->update(window);
            if (signupBackButton) signupBackButton->update(window);
        }
        else if (currentState == GameState::MAIN_MENU)
        {
            if (playGameButton) playGameButton->update(window);
            if (leaderboardButton) leaderboardButton->update(window);
            if (profileButton) profileButton->update(window);
            if (logoutButton) logoutButton->update(window);
        }
        else if (currentState == GameState::ADMIN_MENU)
        {
            if (adminAnalyticsButton) adminAnalyticsButton->update(window);
            if (adminTaskManagementButton) adminTaskManagementButton->update(window);
            if (adminUserManagementButton) adminUserManagementButton->update(window);
            if (adminSearchButton) adminSearchButton->update(window);
            if (adminPlayGameButton) adminPlayGameButton->update(window);
            if (adminLogoutButton) adminLogoutButton->update(window);
        }
        else if (currentState == GameState::GAME_DIFFICULTY)
        {
            if (easyButton) easyButton->update(window);
            if (mediumButton) mediumButton->update(window);
            if (hardButton) hardButton->update(window);
            if (customButton) customButton->update(window);
            if (difficultyBackButton) difficultyBackButton->update(window);
        }
        else if (currentState == GameState::CUSTOM_DIFFICULTY_CONFIG)
        {
            if (customTasksBox) customTasksBox->update(deltaTime);
            if (customCooldownBox) customCooldownBox->update(deltaTime);
            if (customStartButton) customStartButton->update(window);
            if (customBackButton) customBackButton->update(window);
        }
        else if (currentState == GameState::LEADERBOARD)
        {
            if (leaderboardBackButton) leaderboardBackButton->update(window);
        }
        else if (currentState == GameState::USER_PROFILE)
        {
            if (profileBackButton) profileBackButton->update(window);
        }
        else if (currentState == GameState::ANALYTICS)
        {
            if (analyticsBackButton) analyticsBackButton->update(window);
        }
        else if (currentState == GameState::TASK_MANAGEMENT)
        {
            if (taskManagementBackButton) taskManagementBackButton->update(window);
        }
        else if (currentState == GameState::USER_MANAGEMENT)
        {
            if (userManagementBackButton) userManagementBackButton->update(window);
        }
        else if (currentState == GameState::SEARCH_SYSTEM)
        {
            if (searchQueryBox) searchQueryBox->update(deltaTime);
            if (searchButton) searchButton->update(window);
            if (searchBackButton) searchBackButton->update(window);
        }
        else if (currentState == GameState::GAME_PLAY)
        {
            if (showTaskDialog)
            {
                if (taskPasswordBox) taskPasswordBox->update(deltaTime);
                if (taskSubmitButton) taskSubmitButton->update(window);
                if (taskCancelButton) taskCancelButton->update(window);
            }
            else if (showKillDialog)
            {
                for (auto& btn : killTargetButtons)
                {
                    if (btn) btn->update(window);
                }
            }
            else if (!currentGame || currentGame->player.isDead || currentGame->gameOver)
            {
                if (gameQuitButton) gameQuitButton->update(window);
            }
            else
            {
                if (moveNorthButton) moveNorthButton->update(window);
                if (moveWestButton) moveWestButton->update(window);
                if (moveSouthButton) moveSouthButton->update(window);
                if (moveEastButton) moveEastButton->update(window);
                if (doTaskButton) doTaskButton->update(window);
                if (killButton) killButton->update(window);
                if (meetingButton) meetingButton->update(window);
                if (gameQuitButton) gameQuitButton->update(window);
            }
        }
        else if (currentState == GameState::MEETING)
        {
            if (voteInputBox) voteInputBox->update(deltaTime);
            if (voteSubmitButton) voteSubmitButton->update(window);
            if (voteSkipButton) voteSkipButton->update(window);
        }
        else if (currentState == GameState::GAME_OVER)
        {
            if (gameOverBackButton) gameOverBackButton->update(window);
            if (playAgainButton) playAgainButton->update(window);
        }
    }
    
    void render()
    {
        window.clear(sf::Color(20, 20, 30));
        
        if (currentState == GameState::WELCOME)
        {
            renderWelcomeScreen();
        }
        else if (currentState == GameState::LOGIN)
        {
            renderLoginScreen();
        }
        else if (currentState == GameState::SIGNUP)
        {
            renderSignupScreen();
        }
        else if (currentState == GameState::MAIN_MENU)
        {
            renderMainMenu();
        }
        else if (currentState == GameState::ADMIN_MENU)
        {
            renderAdminMenu();
        }
        else if (currentState == GameState::GAME_DIFFICULTY)
        {
            renderDifficultyScreen();
        }
        else if (currentState == GameState::CUSTOM_DIFFICULTY_CONFIG)
        {
            renderCustomDifficultyConfig();
        }
        else if (currentState == GameState::GAME_PLAY)
        {
            renderGamePlay();
        }
        else if (currentState == GameState::LEADERBOARD)
        {
            renderLeaderboard();
        }
        else if (currentState == GameState::USER_PROFILE)
        {
            renderUserProfile();
        }
        else if (currentState == GameState::ANALYTICS)
        {
            renderAnalytics();
        }
        else if (currentState == GameState::TASK_MANAGEMENT)
        {
            renderTaskManagement();
        }
        else if (currentState == GameState::USER_MANAGEMENT)
        {
            renderUserManagement();
        }
        else if (currentState == GameState::SEARCH_SYSTEM)
        {
            renderSearchSystem();
        }
        else if (currentState == GameState::MEETING)
        {
            renderMeeting();
        }
        else if (currentState == GameState::GAME_OVER)
        {
            renderGameOver();
        }
        
        window.display();
    }
    
    // ========================================
    // HANDLER FUNCTIONS
    // ========================================
    
    void handleLoginAttempt()
    {
        string username = loginUsernameBox->getText();
        string password = loginPasswordBox->getText();
        
        if (username.empty() || password.empty())
        {
            loginErrorMessage = "Please enter username and password";
            return;
        }
        
        LoginSystem loginSystem;
        bool isAdminUser = false;
        
        if (loginSystem.login(username, password))
        {
            isAdminUser = loginSystem.isAdmin(username);
            
            currentUsername = username;
            isAdmin = isAdminUser;
            loginErrorMessage = "";
            
            loginPasswordBox->clear();
            
            if (isAdmin)
            {
                setState(GameState::ADMIN_MENU);
            }
            else
            {
                setState(GameState::MAIN_MENU);
            }
            
            cout << "Login successful: " << username << endl;
        }
        else
        {
            loginErrorMessage = "Invalid username or password";
            loginPasswordBox->clear();
        }
    }

    void handleSignupAttempt()
    {
        string username = signupUsernameBox->getText();
        string password = signupPasswordBox->getText();
        string confirmPassword = signupConfirmPasswordBox->getText();
        
        signupErrorMessage = "";
        signupSuccessMessage = "";
        
        if (username.empty() || password.empty() || confirmPassword.empty())
        {
            signupErrorMessage = "Please fill all fields";
            return;
        }
        
        if (password != confirmPassword)
        {
            signupErrorMessage = "Passwords do not match";
            signupPasswordBox->clear();
            signupConfirmPasswordBox->clear();
            return;
        }
        
        LoginSystem loginSystem;
        
        if (loginSystem.signup(username, password))
        {
            signupSuccessMessage = "Account created! You can now login.";
            signupErrorMessage = "";
            
            signupUsernameBox->clear();
            signupPasswordBox->clear();
            signupConfirmPasswordBox->clear();
            
            cout << "Signup successful: " << username << endl;
        }
        else
        {
            signupErrorMessage = "Signup failed - check requirements";
            signupPasswordBox->clear();
            signupConfirmPasswordBox->clear();
        }
    }

    void handleCustomDifficultyStart()
    {
        string tasksStr = customTasksBox->getText();
        string cooldownStr = customCooldownBox->getText();
        
        if (tasksStr.empty() || cooldownStr.empty())
        {
            customConfigError = "Please fill both fields";
            return;
        }
        
        try
        {
            int tasks = stoi(tasksStr);
            int cooldown = stoi(cooldownStr);
            
            if (tasks < 1 || tasks > 8)
            {
                customConfigError = "Tasks must be between 1 and 8";
                return;
            }
            
            if (cooldown < 1 || cooldown > 10)
            {
                customConfigError = "Cooldown must be between 1 and 10";
                return;
            }
            
            selectedDifficulty = 4;
            
            currentGame = make_unique<Game>();
            currentGame->totalTasks = tasks;
            currentGame->killCooldown = cooldown;
            currentGame->difficulty = 2;
            
            customTasksBox->clear();
            customCooldownBox->clear();
            customConfigError = "";
            
            initializeGame();
            setState(GameState::GAME_PLAY);
        }
        catch (...)
        {
            customConfigError = "Invalid input - enter numbers only";
        }
    }

    void startNewGame()
    {
        cout << "Starting new game with difficulty: " << selectedDifficulty << endl;
        
        currentGame = make_unique<Game>();
        lastPlayerKillRound = -1000;
        
        if (selectedDifficulty == 1) {
            currentGame->totalTasks = 3;
            currentGame->killCooldown = 5;
            currentGame->difficulty = 1;
        }
        else if (selectedDifficulty == 2) {
            currentGame->totalTasks = 4;
            currentGame->killCooldown = 4;
            currentGame->difficulty = 2;
        }
        else if (selectedDifficulty == 3) {
            currentGame->totalTasks = 5;
            currentGame->killCooldown = 3;
            currentGame->difficulty = 3;
        }
        
        initializeGame();
        setState(GameState::GAME_PLAY);
    }

    void initializeGame()
    {
        currentGame->player.name = currentUsername;
        currentGame->gameOver = false;
        currentGame->playerWon = false;
        currentGame->roundNumber = 0;
        currentGame->lastMeetingRound = -30;
        
        currentGame->player.isDead = false;
        currentGame->player.isImpostor = false;
        currentGame->player.currentRoomIndex = 0;
        currentGame->player.assignedTasks.clearList();
        currentGame->player.missionHistory.clear();
        
        for (int i = 0; i < 9; i++) {
            currentGame->bots[i].isDead = false;
            currentGame->bots[i].isImpostor = false;
            currentGame->bots[i].currentRoomIndex = 0;
            currentGame->bots[i].lastKillRound = -currentGame->killCooldown;
            currentGame->bots[i].name = "Bot" + to_string(i + 1);
            currentGame->bots[i].assignedTasks.clearList();
        }
        
        int impostorIndex = rand() % 10;
        if (impostorIndex == 0) {
            currentGame->player.isImpostor = true;
            cout << "You are the IMPOSTOR!" << endl;
        } else {
            currentGame->bots[impostorIndex - 1].isImpostor = true;
            cout << "You are a CREWMATE!" << endl;
        }
        
        currentGame->assignTasks();
        
        cout << "Game initialized successfully!" << endl;
    }

    void handleMove(char direction)
    {
        if (!currentGame || currentGame->player.isDead) return;
        
        int neighbors[currentGame->gameMap.roomCount];
        int n = currentGame->gameMap.getNeighbors(currentGame->player.currentRoomIndex, neighbors);
        int nextRoom = currentGame->player.currentRoomIndex;
        
        if (direction == 'W' && n >= 1) {
            nextRoom = neighbors[0];
        }
        else if (direction == 'A' && n >= 2) {
            nextRoom = neighbors[1];
        }
        else if (direction == 'S' && n >= 3) {
            nextRoom = neighbors[2];
        }
        else if (direction == 'D' && n >= 4) {
            nextRoom = neighbors[3];
        }
        
        if (nextRoom != currentGame->player.currentRoomIndex) {
            currentGame->player.moveTo(nextRoom, currentGame->gameMap);
            cout << "Moved to: " << currentGame->gameMap.roomNames[nextRoom] << endl;
            advanceRound();
        }
    }

    void handleDoTask()
    {
        if (!currentGame || currentGame->player.isImpostor) return;
        
        if (currentGame->player.assignedTasks.head == nullptr)
        {
            dialogMessage = "No tasks remaining!";
            return;
        }
        
        showTaskDialog = true;
        dialogMessage = "";
        taskPasswordBox->clear();
    }

    void handleKill()
    {
        if (!currentGame || !currentGame->player.isImpostor) return;
        
        if (currentGame->roundNumber - lastPlayerKillRound < currentGame->killCooldown)
        {
            int remaining = currentGame->killCooldown - (currentGame->roundNumber - lastPlayerKillRound);
            dialogMessage = "Kill on cooldown! Wait " + to_string(remaining) + " rounds";
            return;
        }
        
        vector<int> targets;
        for (int i = 0; i < 9; i++)
        {
            if (!currentGame->bots[i].isDead && 
                currentGame->bots[i].currentRoomIndex == currentGame->player.currentRoomIndex &&
                !currentGame->bots[i].isImpostor)
            {
                targets.push_back(i);
            }
        }
        
        if (targets.empty())
        {
            dialogMessage = "No targets in this room!";
            return;
        }
        
        killTargetButtons.clear();
        for (size_t i = 0; i < targets.size(); i++)
        {
            int botIndex = targets[i];
            auto btn = make_unique<Button>(
                440, 
                static_cast<float>(250 + i * 70), 
                400, 
                60, 
                currentGame->bots[botIndex].name, 
                font, 
                24
            );
            
            btn->setCallback([this, botIndex]() {
                currentGame->bots[botIndex].isDead = true;
                lastPlayerKillRound = currentGame->roundNumber;
                dialogMessage = "You killed " + currentGame->bots[botIndex].name + "!";
                showKillDialog = false;
                killTargetButtons.clear();
                advanceRound();
            });
            
            killTargetButtons.push_back(move(btn));
        }
        
        showKillDialog = true;
    }

    void handleMeeting()
    {
        if (!currentGame) return;
        
        int cooldown = 30 - (currentGame->roundNumber - currentGame->lastMeetingRound);
        if (cooldown > 0)
        {
            dialogMessage = "Meeting on cooldown! Wait " + to_string(cooldown) + " rounds";
            return;
        }
        
        currentGame->player.moveTo(0, currentGame->gameMap);
        currentGame->lastMeetingRound = currentGame->roundNumber;
        
        votingResult = "";
        voteInputBox->clear();
        
        setState(GameState::MEETING);
    }

    void submitTaskPassword()
    {
        if (!currentGame) return;
        
        string password = taskPasswordBox->getText();
        
        if (password.empty())
        {
            dialogMessage = "Please enter a password!";
            return;
        }
        
        bool completed = false;
        ListNode* tmp = currentGame->player.assignedTasks.head;
        
        while (tmp != nullptr)
        {
            if (currentGame->taskManager.checkPassword(password, tmp->data, currentGame->difficulty))
            {
                currentGame->taskManager.completeTaskPlayer(
                    currentGame->player.assignedTasks, 
                    tmp->data, 
                    currentGame->player.name
                );
                currentGame->player.missionHistory.push(tmp->data);
                
                dialogMessage = "Task completed: " + tmp->data;
                completed = true;
                showTaskDialog = false;
                taskPasswordBox->clear();
                advanceRound();
                break;
            }
            tmp = tmp->next;
        }
        
        if (!completed)
        {
            dialogMessage = "Incorrect password!";
        }
    }

    void submitVote()
    {
        if (!currentGame) return;
        
        string voteName = voteInputBox->getText();
        
        if (voteName.empty())
        {
            votingResult = "Please enter a name or 'skip'";
            return;
        }
        
        vector<string> candidates;
        vector<int> votes;
        
        if (!currentGame->player.isDead)
        {
            candidates.push_back(currentGame->player.name);
            votes.push_back(0);
        }
        
        for (int i = 0; i < 9; i++)
        {
            if (!currentGame->bots[i].isDead)
            {
                candidates.push_back(currentGame->bots[i].name);
                votes.push_back(0);
            }
        }
        
        for (int i = 0; i < 9; i++)
        {
            if (currentGame->bots[i].isDead) continue;
            
            int r = rand() % (candidates.size() + 1);
            if (r < candidates.size())
            {
                votes[r]++;
            }
        }
        
        bool validVote = false;
        for (size_t i = 0; i < candidates.size(); i++)
        {
            if (candidates[i] == voteName)
            {
                votes[i]++;
                validVote = true;
                votingResult = "You voted for " + voteName;
                break;
            }
        }
        
        if (!validVote && voteName != "skip")
        {
            votingResult = "Invalid name! Vote counted as skip.";
        }
        else if (voteName == "skip")
        {
            votingResult = "You skipped voting";
        }
        
        int maxVotes = 0;
        int maxIndex = -1;
        
        for (size_t i = 0; i < votes.size(); i++)
        {
            if (votes[i] > maxVotes)
            {
                maxVotes = votes[i];
                maxIndex = i;
            }
        }
        
        if (maxIndex == -1 || maxVotes == 0)
        {
            votingResult += "\n\nNo one was ejected.";
        }
        else
        {
            string ejected = candidates[maxIndex];
            bool wasImpostor = false;
            
            if (ejected == currentGame->player.name)
            {
                currentGame->player.isDead = true;
                wasImpostor = currentGame->player.isImpostor;
            }
            else
            {
                for (int i = 0; i < 9; i++)
                {
                    if (currentGame->bots[i].name == ejected)
                    {
                        currentGame->bots[i].isDead = true;
                        wasImpostor = currentGame->bots[i].isImpostor;
                        break;
                    }
                }
            }
            
            votingResult += "\n\n" + ejected + " was ejected.\n";
            votingResult += wasImpostor ? "They were an Impostor!" : "They were not an Impostor.";
        }
        
        currentGame->checkWinCondition();
        
        if (currentGame->gameOver)
        {
            saveGameResult();
            setState(GameState::GAME_OVER);
        }
        else
        {
            voteInputBox->clear();
            advanceRound();
            setState(GameState::GAME_PLAY);
        }
    }

    void advanceRound()
    {
        if (!currentGame || currentGame->gameOver) return;
        
        currentGame->roundNumber++;
        
        for (int i = 0; i < 9; i++) {
            currentGame->bots[i].takeTurn(
                currentGame->player, 
                currentGame->bots, 
                9, 
                currentGame->gameMap, 
                currentGame->taskManager, 
                currentGame->roundNumber, 
                currentGame->killCooldown, 
                currentGame->difficulty
            );
        }
        
        currentGame->checkWinCondition();
        
        if (currentGame->gameOver) {
            saveGameResult();
            setState(GameState::GAME_OVER);
        }
    }

    void saveGameResult()
    {
        if (!currentGame) return;
        
        ofstream file("data/game_results.csv", ios::app);
        if (!file)
        {
            cerr << "Failed to save game result!" << endl;
            return;
        }
        
        time_t now = time(0);
        tm* lt = localtime(&now);
        
        char date[11];
        sprintf(date, "%04d-%02d-%02d", 1900 + lt->tm_year, 1 + lt->tm_mon, lt->tm_mday);
        
        string outcome = currentGame->playerWon ? "Win" : "Loss";
        string role = currentGame->player.isImpostor ? "Impostor" : "Crewmate";
        
        file << currentUsername << ","
             << outcome << ","
             << currentGame->roundNumber << ","
             << date << ","
             << currentGame->totalTasks << ","
             << currentGame->killCooldown << ","
             << currentGame->difficulty << ","
             << "0,"
             << "0,"
             << role << "\n";
        
        file.close();
        
        cout << "Game result saved!" << endl;
    }

    void loadAnalyticsData()
    {
        analyticsData.clear();
        
        ifstream file("data/console_log.csv");
        if (!file)
        {
            analyticsData.push_back("No analytics data available");
            return;
        }
        
        string line;
        getline(file, line);
        
        while (getline(file, line))
        {
            if (!line.empty())
            {
                analyticsData.push_back(line);
            }
        }
        
        file.close();
        
        if (analyticsData.empty())
        {
            analyticsData.push_back("No analytics data available");
        }
    }

    void loadTaskList()
    {
        taskList.clear();
        
        ifstream file("data/task.csv");
        if (!file)
        {
            taskList.push_back("No tasks found");
            return;
        }
        
        string line;
        getline(file, line);
        
        while (getline(file, line))
        {
            if (!line.empty())
            {
                taskList.push_back(line);
            }
        }
        
        file.close();
        
        if (taskList.empty())
        {
            taskList.push_back("No tasks found");
        }
    }

    void loadUserList()
    {
        userList.clear();
        
        ifstream file("data/user.csv");
        if (!file)
        {
            userList.push_back("No users found");
            return;
        }
        
        string line;
        getline(file, line);
        
        while (getline(file, line))
        {
            if (!line.empty())
            {
                stringstream ss(line);
                string username, hash, deleted, isAdmin;
                
                getline(ss, username, ',');
                getline(ss, hash, ',');
                getline(ss, deleted, ',');
                getline(ss, isAdmin, ',');
                
                if (deleted != "1")
                {
                    string role = (isAdmin == "1") ? " [ADMIN]" : " [USER]";
                    userList.push_back(username + role);
                }
            }
        }
        
        file.close();
        
        if (userList.empty())
        {
            userList.push_back("No users found");
        }
    }

    void performSearch()
    {
        searchResults.clear();
        string query = searchQueryBox->getText();
        
        if (query.empty())
        {
            searchStatus = "Please enter a search query";
            return;
        }
        
        ifstream file("data/console_log.csv");
        if (!file)
        {
            searchStatus = "No log data available";
            return;
        }
        
        string line;
        getline(file, line);
        
        int count = 0;
        while (getline(file, line))
        {
            if (line.find(query) != string::npos)
            {
                searchResults.push_back(line);
                count++;
            }
        }
        
        file.close();
        
        if (count == 0)
        {
            searchStatus = "No results found for: " + query;
        }
        else
        {
            searchStatus = "Found " + to_string(count) + " results";
        }
    }

    // ========================================
    // RENDERING FUNCTIONS
    // ========================================

    void renderWelcomeScreen()
    {
        sf::Text title(font);
        title.setString("AMONG US");
        title.setCharacterSize(80);
        title.setFillColor(sf::Color::Red);
        title.setStyle(sf::Text::Bold);
        
        sf::FloatRect bounds = title.getLocalBounds();
        title.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        title.setPosition({640, 150});
        
        sf::Text subtitle(font);
        subtitle.setString("C++ Edition - GUI Version");
        subtitle.setCharacterSize(30);
        subtitle.setFillColor(sf::Color(200, 200, 200));
        
        bounds = subtitle.getLocalBounds();
        subtitle.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        subtitle.setPosition({640, 250});
        
        sf::Text info(font);
        info.setString("Click anywhere to start\n\nPress ESC to close");
        info.setCharacterSize(20);
        info.setFillColor(sf::Color(150, 150, 150));
        
        bounds = info.getLocalBounds();
        info.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        info.setPosition({640, 400});
        
        window.draw(title);
        window.draw(subtitle);
        window.draw(info);
    }
    
    void renderLoginScreen()
    {
        sf::Text title(font);
        title.setString("LOGIN");
        title.setCharacterSize(50);
        title.setFillColor(sf::Color::White);
        
        sf::FloatRect bounds = title.getLocalBounds();
        title.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        title.setPosition({640, 150});
        
        window.draw(title);
        
        if (loginUsernameBox) loginUsernameBox->draw(window);
        if (loginPasswordBox) loginPasswordBox->draw(window);
        if (loginButton) loginButton->draw(window);
        if (signupButton) signupButton->draw(window);
        
        if (!loginErrorMessage.empty())
        {
            sf::Text errorText(font);
            errorText.setString(loginErrorMessage);
            errorText.setCharacterSize(20);
            errorText.setFillColor(sf::Color::Red);
            
            bounds = errorText.getLocalBounds();
            errorText.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
            errorText.setPosition({640, 530});
            
            window.draw(errorText);
        }
    }
    
    void renderSignupScreen()
    {
        sf::Text title(font);
        title.setString("SIGN UP");
        title.setCharacterSize(50);
        title.setFillColor(sf::Color::White);
        
        sf::FloatRect bounds = title.getLocalBounds();
        title.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        title.setPosition({640, 120});
        
        window.draw(title);
        
        sf::Text instructions(font);
        instructions.setString("Password: 8+ chars, uppercase, lowercase, digit, special");
        instructions.setCharacterSize(14);
        instructions.setFillColor(sf::Color(180, 180, 180));
        
        bounds = instructions.getLocalBounds();
        instructions.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        instructions.setPosition({640, 180});
        
        window.draw(instructions);
        
        if (signupUsernameBox) signupUsernameBox->draw(window);
        if (signupPasswordBox) signupPasswordBox->draw(window);
        if (signupConfirmPasswordBox) signupConfirmPasswordBox->draw(window);
        if (signupCreateButton) signupCreateButton->draw(window);
        if (signupBackButton) signupBackButton->draw(window);
        
        if (!signupErrorMessage.empty())
        {
            sf::Text errorText(font);
            errorText.setString(signupErrorMessage);
            errorText.setCharacterSize(18);
            errorText.setFillColor(sf::Color::Red);
            
            bounds = errorText.getLocalBounds();
            errorText.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
            errorText.setPosition({640, 570});
            
            window.draw(errorText);
        }
        
        if (!signupSuccessMessage.empty())
        {
            sf::Text successText(font);
            successText.setString(signupSuccessMessage);
            successText.setCharacterSize(18);
            successText.setFillColor(sf::Color::Green);
            
            bounds = successText.getLocalBounds();
            successText.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
            successText.setPosition({640, 570});
            
            window.draw(successText);
        }
    }
    
    void renderMainMenu()
    {
        sf::Text title(font);
        title.setString("MAIN MENU");
        title.setCharacterSize(50);
        title.setFillColor(sf::Color::White);
        
        sf::FloatRect bounds = title.getLocalBounds();
        title.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        title.setPosition({640, 120});
        
        window.draw(title);
        
        sf::Text welcome(font);
        welcome.setString("Welcome, " + currentUsername + "!");
        welcome.setCharacterSize(24);
        welcome.setFillColor(sf::Color(150, 255, 150));
        
        bounds = welcome.getLocalBounds();
        welcome.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        welcome.setPosition({640, 180});
        
        window.draw(welcome);
        
        if (playGameButton) playGameButton->draw(window);
        if (leaderboardButton) leaderboardButton->draw(window);
        if (profileButton) profileButton->draw(window);
        if (logoutButton) logoutButton->draw(window);
    }
    
    void renderAdminMenu()
    {
        sf::Text title(font);
        title.setString("ADMIN MENU");
        title.setCharacterSize(50);
        title.setFillColor(sf::Color(255, 215, 0));
        
        sf::FloatRect bounds = title.getLocalBounds();
        title.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        title.setPosition({640, 100});
        
        window.draw(title);
        
        sf::Text welcome(font);
        welcome.setString("Admin: " + currentUsername);
        welcome.setCharacterSize(22);
        welcome.setFillColor(sf::Color(255, 215, 0));
        
        bounds = welcome.getLocalBounds();
        welcome.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        welcome.setPosition({640, 150});
        
        window.draw(welcome);
        
        if (adminAnalyticsButton) adminAnalyticsButton->draw(window);
        if (adminTaskManagementButton) adminTaskManagementButton->draw(window);
        if (adminUserManagementButton) adminUserManagementButton->draw(window);
        if (adminSearchButton) adminSearchButton->draw(window);
        if (adminPlayGameButton) adminPlayGameButton->draw(window);
        if (adminLogoutButton) adminLogoutButton->draw(window);
    }
    
    void renderDifficultyScreen()
    {
        sf::Text title(font);
        title.setString("SELECT DIFFICULTY");
        title.setCharacterSize(50);
        title.setFillColor(sf::Color::White);
        
        sf::FloatRect bounds = title.getLocalBounds();
        title.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        title.setPosition({640, 100});
        
        window.draw(title);
        
        sf::Text easyDesc(font);
        easyDesc.setString("3 tasks | 5-round cooldown | 80% bot completion");
        easyDesc.setCharacterSize(15);
        easyDesc.setFillColor(sf::Color(150, 150, 150));
        
        bounds = easyDesc.getLocalBounds();
        easyDesc.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        easyDesc.setPosition({640, 155});
        
        sf::Text mediumDesc(font);
        mediumDesc.setString("4 tasks | 4-round cooldown | 50% bot completion");
        mediumDesc.setCharacterSize(15);
        mediumDesc.setFillColor(sf::Color(150, 150, 150));
        
        bounds = mediumDesc.getLocalBounds();
        mediumDesc.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        mediumDesc.setPosition({640, 175});
        
        sf::Text hardDesc(font);
        hardDesc.setString("5 tasks | 3-round cooldown | 37% bot completion");
        hardDesc.setCharacterSize(15);
        hardDesc.setFillColor(sf::Color(150, 150, 150));
        
        bounds = hardDesc.getLocalBounds();
        hardDesc.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        hardDesc.setPosition({640, 195});
        
        window.draw(easyDesc);
        window.draw(mediumDesc);
        window.draw(hardDesc);
        
        if (easyButton) 
        {
            easyButton->setNormalColor(sf::Color(50, 150, 50));
            easyButton->setHoverColor(sf::Color(70, 200, 70));
            easyButton->draw(window);
        }
        if (mediumButton)
        {
            mediumButton->setNormalColor(sf::Color(200, 150, 50));
            mediumButton->setHoverColor(sf::Color(255, 200, 70));
            mediumButton->draw(window);
        }
        if (hardButton)
        {
            hardButton->setNormalColor(sf::Color(180, 50, 50));
            hardButton->setHoverColor(sf::Color(220, 70, 70));
            hardButton->draw(window);
        }
        if (customButton)
        {
            customButton->setNormalColor(sf::Color(100, 100, 180));
            customButton->setHoverColor(sf::Color(130, 130, 220));
            customButton->draw(window);
        }
        if (difficultyBackButton) difficultyBackButton->draw(window);
    }

    void renderCustomDifficultyConfig()
    {
        sf::Text title(font);
        title.setString("CUSTOM DIFFICULTY");
        title.setCharacterSize(50);
        title.setFillColor(sf::Color::Cyan);
        
        sf::FloatRect bounds = title.getLocalBounds();
        title.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        title.setPosition({640, 150});
        
        window.draw(title);
        
        sf::Text instructions(font);
        instructions.setString("Configure your custom game settings:");
        instructions.setCharacterSize(20);
        instructions.setFillColor(sf::Color(200, 200, 200));
        
        bounds = instructions.getLocalBounds();
        instructions.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        instructions.setPosition({640, 220});
        
        window.draw(instructions);
        
        if (customTasksBox) customTasksBox->draw(window);
        if (customCooldownBox) customCooldownBox->draw(window);
        if (customStartButton) customStartButton->draw(window);
        if (customBackButton) customBackButton->draw(window);
        
        if (!customConfigError.empty())
        {
            sf::Text errorText(font);
            errorText.setString(customConfigError);
            errorText.setCharacterSize(18);
            errorText.setFillColor(sf::Color::Red);
            
            bounds = errorText.getLocalBounds();
            errorText.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
            errorText.setPosition({640, 530});
            
            window.draw(errorText);
        }
    }
    
    void renderLeaderboard()
    {
        sf::Text title(font);
        title.setString("LEADERBOARD");
        title.setCharacterSize(50);
        title.setFillColor(sf::Color::Yellow);
        
        sf::FloatRect bounds = title.getLocalBounds();
        title.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        title.setPosition({640, 60});
        
        window.draw(title);
        
        sf::Text headers(font);
        headers.setString("Rank        Player                Win Points");
        headers.setCharacterSize(20);
        headers.setFillColor(sf::Color(200, 200, 200));
        headers.setPosition({320, 140});
        
        window.draw(headers);
        
        sf::RectangleShape line;
        line.setSize({600, 2});
        line.setPosition({340, 170});
        line.setFillColor(sf::Color(100, 100, 100));
        window.draw(line);
        
        ifstream file("data/game_results.csv");
        if (!file)
        {
            sf::Text noData(font);
            noData.setString("No leaderboard data available");
            noData.setCharacterSize(20);
            noData.setFillColor(sf::Color(150, 150, 150));
            
            bounds = noData.getLocalBounds();
            noData.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
            noData.setPosition({640, 300});
            
            window.draw(noData);
            
            if (leaderboardBackButton) leaderboardBackButton->draw(window);
            return;
        }
        
        string line_str;
        getline(file, line_str);
        
        map<string, int> winCounts;
        
        while (getline(file, line_str))
        {
            if (line_str.empty()) continue;
            
            stringstream ss(line_str);
            string playerName, outcome;
            
            getline(ss, playerName, ',');
            getline(ss, outcome, ',');
            
            if (outcome == "Win")
            {
                winCounts[playerName]++;
            }
        }
        file.close();
        
        if (winCounts.empty())
        {
            sf::Text noWins(font);
            noWins.setString("No wins recorded yet");
            noWins.setCharacterSize(20);
            noWins.setFillColor(sf::Color(150, 150, 150));
            
            bounds = noWins.getLocalBounds();
            noWins.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
            noWins.setPosition({640, 300});
            
            window.draw(noWins);
            
            if (leaderboardBackButton) leaderboardBackButton->draw(window);
            return;
        }
        
        vector<pair<string, int>> leaderboardData;
        for (const auto& entry : winCounts)
        {
            leaderboardData.push_back({entry.first, entry.second});
        }
        
        sort(leaderboardData.begin(), leaderboardData.end(),
             [](const auto& a, const auto& b) {
                 return a.second > b.second;
             });
        
        int yPos = 190;
        int rank = 1;
        
        for (size_t i = 0; i < min(leaderboardData.size(), size_t(10)); i++)
        {
            const auto& entry = leaderboardData[i];
            int winPoints = entry.second * 1000;
            
            stringstream entryText;
            entryText << " " << rank << "          " 
                      << setw(20) << left << entry.first
                      << setw(10) << right << winPoints;
            
            sf::Text entry_sf(font);
            entry_sf.setString(entryText.str());
            entry_sf.setCharacterSize(18);
            entry_sf.setFillColor(sf::Color::White);
            entry_sf.setPosition({340, static_cast<float>(yPos)});
            
            if (entry.first == currentUsername)
            {
                entry_sf.setFillColor(sf::Color(255, 215, 0));
                entry_sf.setStyle(sf::Text::Bold);
            }
            else if (rank == 1)
            {
                entry_sf.setFillColor(sf::Color(255, 215, 0));
            }
            else if (rank == 2)
            {
                entry_sf.setFillColor(sf::Color(192, 192, 192));
            }
            else if (rank == 3)
            {
                entry_sf.setFillColor(sf::Color(205, 127, 50));
            }
            
            window.draw(entry_sf);
            
            yPos += 40;
            rank++;
        }
        
        if (leaderboardBackButton) leaderboardBackButton->draw(window);
    }
    
    void renderUserProfile()
    {
        sf::Text title(font);
        title.setString("USER PROFILE");
        title.setCharacterSize(50);
        title.setFillColor(sf::Color::Cyan);
        
        sf::FloatRect bounds = title.getLocalBounds();
        title.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        title.setPosition({640, 60});
        
        window.draw(title);
        
        sf::Text usernameText(font);
        usernameText.setString("Username: " + currentUsername);
        usernameText.setCharacterSize(28);
        usernameText.setFillColor(sf::Color::White);
        usernameText.setStyle(sf::Text::Bold);
        usernameText.setPosition({340, 130});
        
        window.draw(usernameText);
        
        ifstream file("data/game_results.csv");
        if (!file)
        {
            sf::Text noData(font);
            noData.setString("No game history found");
            noData.setCharacterSize(20);
            noData.setFillColor(sf::Color(150, 150, 150));
            noData.setPosition({340, 250});
            
            window.draw(noData);
            
            if (profileBackButton) profileBackButton->draw(window);
            return;
        }
        
        string line;
        getline(file, line);
        
        int totalGames = 0;
        int wins = 0;
        int losses = 0;
        vector<tuple<string, string, string>> recentGames;
        
        while (getline(file, line))
        {
            if (line.empty()) continue;
            
            stringstream ss(line);
            string username, outcome, rounds, role;
            
            getline(ss, username, ',');
            getline(ss, outcome, ',');
            getline(ss, rounds, ',');
            
            for (int i = 0; i < 6; i++)
            {
                string temp;
                getline(ss, temp, ',');
            }
            getline(ss, role, ',');
            
            if (username == currentUsername)
            {
                totalGames++;
                if (outcome == "Win")
                {
                    wins++;
                }
                else
                {
                    losses++;
                }
                
                recentGames.push_back({outcome, role, rounds});
            }
        }
        file.close();
        
        float winRate = (totalGames > 0) ? (wins * 100.0f / totalGames) : 0.0f;
        
        sf::Text statsText(font);
        stringstream stats;
        stats << "Total Games: " << totalGames << "\n\n"
              << "Wins: " << wins << "\n\n"
              << "Losses: " << losses << "\n\n"
              << "Win Rate: " << fixed << setprecision(1) << winRate << "%";
        
        statsText.setString(stats.str());
        statsText.setCharacterSize(22);
        statsText.setFillColor(sf::Color::White);
        statsText.setPosition({340, 190});
        
        window.draw(statsText);
        
        sf::Text recentTitle(font);
        recentTitle.setString("Recent Games:");
        recentTitle.setCharacterSize(24);
        recentTitle.setFillColor(sf::Color(150, 200, 255));
        recentTitle.setPosition({340, 380});
        
        window.draw(recentTitle);
        
        if (recentGames.empty())
        {
            sf::Text noGames(font);
            noGames.setString("No games played yet");
            noGames.setCharacterSize(18);
            noGames.setFillColor(sf::Color(150, 150, 150));
            noGames.setPosition({340, 420});
            
            window.draw(noGames);
        }
        else
        {
            int yPos = 420;
            int count = 0;
            
            for (int i = recentGames.size() - 1; i >= 0 && count < 5; i--, count++)
            {
                const auto& game = recentGames[i];
                
                stringstream gameText;
                gameText << (count + 1) << ". " 
                         << get<0>(game) << " - " 
                         << get<1>(game) << " - " 
                         << get<2>(game) << " rounds";
                
                sf::Text gameEntry(font);
                gameEntry.setString(gameText.str());
                gameEntry.setCharacterSize(16);
                
                if (get<0>(game) == "Win")
                {
                    gameEntry.setFillColor(sf::Color::Green);
                }
                else
                {
                    gameEntry.setFillColor(sf::Color(255, 100, 100));
                }
                
                gameEntry.setPosition({360, static_cast<float>(yPos)});
                window.draw(gameEntry);
                
                yPos += 30;
            }
        }
        
        if (profileBackButton) profileBackButton->draw(window);
    }
    
    void renderAnalytics()
    {
        sf::Text title(font);
        title.setString("ANALYTICS DASHBOARD");
        title.setCharacterSize(45);
        title.setFillColor(sf::Color(100, 255, 100));
        
        sf::FloatRect bounds = title.getLocalBounds();
        title.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        title.setPosition({640, 60});
        
        window.draw(title);
        
        sf::Text subtitle(font);
        subtitle.setString("Console Log Activity");
        subtitle.setCharacterSize(20);
        subtitle.setFillColor(sf::Color(180, 180, 180));
        
        bounds = subtitle.getLocalBounds();
        subtitle.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        subtitle.setPosition({640, 110});
        
        window.draw(subtitle);
        
        sf::RectangleShape panel;
        panel.setSize({1100, 450});
        panel.setPosition({90, 140});
        panel.setFillColor(sf::Color(25, 25, 35));
        panel.setOutlineThickness(2);
        panel.setOutlineColor(sf::Color(60, 60, 70));
        window.draw(panel);
        
        int yPos = 160;
        int displayCount = 0;
        
        for (size_t i = 0; i < analyticsData.size() && displayCount < 12; i++)
        {
            sf::Text logEntry(font);
            
            string entry = analyticsData[i];
            if (entry.length() > 100)
            {
                entry = entry.substr(0, 97) + "...";
            }
            
            logEntry.setString(entry);
            logEntry.setCharacterSize(14);
            logEntry.setFillColor(sf::Color(200, 200, 200));
            logEntry.setPosition({110, static_cast<float>(yPos)});
            
            window.draw(logEntry);
            
            yPos += 35;
            displayCount++;
        }
        
        if (analyticsData.empty() || analyticsData[0] == "No analytics data available")
        {
            sf::Text noData(font);
            noData.setString("No analytics data available");
            noData.setCharacterSize(20);
            noData.setFillColor(sf::Color(150, 150, 150));
            
            bounds = noData.getLocalBounds();
            noData.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
            noData.setPosition({640, 350});
            
            window.draw(noData);
        }
        
        if (analyticsBackButton) analyticsBackButton->draw(window);
    }
    
    void renderTaskManagement()
    {
        sf::Text title(font);
        title.setString("TASK MANAGEMENT");
        title.setCharacterSize(45);
        title.setFillColor(sf::Color::Magenta);
        
        sf::FloatRect bounds = title.getLocalBounds();
        title.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        title.setPosition({640, 60});
        
        window.draw(title);
        
        sf::Text subtitle(font);
        subtitle.setString("Available Tasks");
        subtitle.setCharacterSize(20);
        subtitle.setFillColor(sf::Color(180, 180, 180));
        
        bounds = subtitle.getLocalBounds();
        subtitle.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        subtitle.setPosition({640, 110});
        
        window.draw(subtitle);
        
        sf::RectangleShape panel;
        panel.setSize({1100, 450});
        panel.setPosition({90, 140});
        panel.setFillColor(sf::Color(25, 25, 35));
        panel.setOutlineThickness(2);
        panel.setOutlineColor(sf::Color(60, 60, 70));
        window.draw(panel);
        
        int yPos = 160;
        int displayCount = 0;
        
        for (size_t i = 0; i < taskList.size() && displayCount < 12; i++)
        {
            sf::Text taskEntry(font);
            
            string entry = taskList[i];
            if (entry.length() > 100)
            {
                entry = entry.substr(0, 97) + "...";
            }
            
            taskEntry.setString(entry);
            taskEntry.setCharacterSize(14);
            taskEntry.setFillColor(sf::Color(200, 200, 200));
            taskEntry.setPosition({110, static_cast<float>(yPos)});
            
            window.draw(taskEntry);
            
            yPos += 35;
            displayCount++;
        }
        
        if (taskList.empty() || taskList[0] == "No tasks found")
        {
            sf::Text noData(font);
            noData.setString("No tasks found");
            noData.setCharacterSize(20);
            noData.setFillColor(sf::Color(150, 150, 150));
            
            bounds = noData.getLocalBounds();
            noData.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
            noData.setPosition({640, 350});
            
            window.draw(noData);
        }
        
        if (taskManagementBackButton) taskManagementBackButton->draw(window);
    }
    
    void renderUserManagement()
    {
        sf::Text title(font);
        title.setString("USER MANAGEMENT");
        title.setCharacterSize(45);
        title.setFillColor(sf::Color(150, 150, 255));
        
        sf::FloatRect bounds = title.getLocalBounds();
        title.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        title.setPosition({640, 60});
        
        window.draw(title);
        
        sf::Text subtitle(font);
        subtitle.setString("Registered Users");
        subtitle.setCharacterSize(20);
        subtitle.setFillColor(sf::Color(180, 180, 180));
        
        bounds = subtitle.getLocalBounds();
        subtitle.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        subtitle.setPosition({640, 110});
        
        window.draw(subtitle);
        
        sf::RectangleShape panel;
        panel.setSize({1100, 450});
        panel.setPosition({90, 140});
        panel.setFillColor(sf::Color(25, 25, 35));
        panel.setOutlineThickness(2);
        panel.setOutlineColor(sf::Color(60, 60, 70));
        window.draw(panel);
        
        int yPos = 160;
        int displayCount = 0;
        
        for (size_t i = 0; i < userList.size() && displayCount < 12; i++)
        {
            sf::Text userEntry(font);
            userEntry.setString(userList[i]);
            userEntry.setCharacterSize(18);
            
            if (userList[i].find("[ADMIN]") != string::npos)
            {
                userEntry.setFillColor(sf::Color(255, 215, 0));
            }
            else
            {
                userEntry.setFillColor(sf::Color(200, 200, 200));
            }
            
            userEntry.setPosition({110, static_cast<float>(yPos)});
            
            window.draw(userEntry);
            
            yPos += 35;
            displayCount++;
        }
        
        if (userList.empty() || userList[0] == "No users found")
        {
            sf::Text noData(font);
            noData.setString("No users found");
            noData.setCharacterSize(20);
            noData.setFillColor(sf::Color(150, 150, 150));
            
            bounds = noData.getLocalBounds();
            noData.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
            noData.setPosition({640, 350});
            
            window.draw(noData);
        }
        
        if (userManagementBackButton) userManagementBackButton->draw(window);
    }
    
    void renderSearchSystem()
    {
        sf::Text title(font);
        title.setString("SEARCH SYSTEM");
        title.setCharacterSize(45);
        title.setFillColor(sf::Color(255, 200, 100));
        
        sf::FloatRect bounds = title.getLocalBounds();
        title.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        title.setPosition({640, 70});
        
        window.draw(title);
        
        if (searchQueryBox) searchQueryBox->draw(window);
        if (searchButton) searchButton->draw(window);
        
        if (!searchStatus.empty())
        {
            sf::Text statusText(font);
            statusText.setString(searchStatus);
            statusText.setCharacterSize(18);
            statusText.setFillColor(sf::Color::Yellow);
            
            bounds = statusText.getLocalBounds();
            statusText.setOrigin({bounds.size.x / 2.0f, 0});
            statusText.setPosition({640, 290});
            
            window.draw(statusText);
        }
        
        sf::RectangleShape panel;
        panel.setSize({1100, 300});
        panel.setPosition({90, 320});
        panel.setFillColor(sf::Color(25, 25, 35));
        panel.setOutlineThickness(2);
        panel.setOutlineColor(sf::Color(60, 60, 70));
        window.draw(panel);
        
        int yPos = 340;
        int displayCount = 0;
        
        for (size_t i = 0; i < searchResults.size() && displayCount < 8; i++)
        {
            sf::Text resultEntry(font);
            
            string entry = searchResults[i];
            if (entry.length() > 110)
            {
                entry = entry.substr(0, 107) + "...";
            }
            
            resultEntry.setString(entry);
            resultEntry.setCharacterSize(13);
            resultEntry.setFillColor(sf::Color(200, 200, 200));
            resultEntry.setPosition({110, static_cast<float>(yPos)});
            
            window.draw(resultEntry);
            
            yPos += 35;
            displayCount++;
        }
        
        if (searchBackButton) searchBackButton->draw(window);
    }

    void renderGamePlay()
    {
        if (!currentGame)
        {
            sf::Text error(font);
            error.setString("Game not initialized");
            error.setCharacterSize(30);
            error.setFillColor(sf::Color::Red);
            error.setPosition({400, 300});
            window.draw(error);
            return;
        }
        
        sf::RectangleShape bg;
        bg.setSize({1280, 720});
        bg.setFillColor(sf::Color(15, 15, 25));
        window.draw(bg);
        
        sf::RectangleShape titleBar;
        titleBar.setSize({1280, 60});
        titleBar.setFillColor(sf::Color(30, 30, 40));
        window.draw(titleBar);
        
        sf::Text roundText(font);
        roundText.setString("Round " + to_string(currentGame->roundNumber));
        roundText.setCharacterSize(24);
        roundText.setFillColor(sf::Color::White);
        roundText.setPosition({20, 15});
        window.draw(roundText);
        
        sf::Text roomText(font);
        string roomName = currentGame->gameMap.roomNames[currentGame->player.currentRoomIndex];
        roomText.setString("Room: " + roomName);
        roomText.setCharacterSize(24);
        roomText.setFillColor(sf::Color::Cyan);
        roomText.setPosition({400, 15});
        window.draw(roomText);
        
        sf::Text roleText(font);
        if (currentGame->player.isImpostor) {
            roleText.setString("IMPOSTOR");
            roleText.setFillColor(sf::Color::Red);
        } else {
            roleText.setString("CREWMATE");
            roleText.setFillColor(sf::Color::Green);
        }
        roleText.setCharacterSize(28);
        roleText.setStyle(sf::Text::Bold);
        sf::FloatRect bounds = roleText.getLocalBounds();
        roleText.setOrigin({bounds.size.x / 2.0f, 0});
        roleText.setPosition({750, 12});
        window.draw(roleText);
        
        sf::Text nameText(font);
        nameText.setString(currentGame->player.name);
        nameText.setCharacterSize(20);
        nameText.setFillColor(sf::Color(200, 200, 200));
        nameText.setPosition({1000, 18});
        window.draw(nameText);
        
        if (currentGame->player.isDead)
        {
            sf::Text deadText(font);
            deadText.setString("YOU ARE DEAD\n\nSpectating...");
            deadText.setCharacterSize(40);
            deadText.setFillColor(sf::Color::Red);
            deadText.setStyle(sf::Text::Bold);
            
            bounds = deadText.getLocalBounds();
            deadText.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
            deadText.setPosition({640, 300});
            window.draw(deadText);
            
            if (gameQuitButton) gameQuitButton->draw(window);
            return;
        }
        
        sf::RectangleShape leftPanel;
        leftPanel.setSize({350, 450});
        leftPanel.setPosition({20, 80});
        leftPanel.setFillColor(sf::Color(25, 25, 35));
        leftPanel.setOutlineThickness(2);
        leftPanel.setOutlineColor(sf::Color(60, 60, 70));
        window.draw(leftPanel);
        
        if (currentGame->player.isImpostor)
        {
            sf::Text impostorTitle(font);
            impostorTitle.setString("IMPOSTOR MISSION");
            impostorTitle.setCharacterSize(22);
            impostorTitle.setFillColor(sf::Color::Red);
            impostorTitle.setStyle(sf::Text::Bold);
            impostorTitle.setPosition({40, 95});
            window.draw(impostorTitle);
            
            sf::Text objective(font);
            objective.setString("Eliminate all crewmates!");
            objective.setCharacterSize(16);
            objective.setFillColor(sf::Color(200, 200, 200));
            objective.setPosition({40, 130});
            window.draw(objective);
            
            sf::Text cooldownText(font);
            cooldownText.setString("Kill Cooldown: " + to_string(currentGame->killCooldown) + " rounds");
            cooldownText.setCharacterSize(16);
            cooldownText.setFillColor(sf::Color::Yellow);
            cooldownText.setPosition({40, 160});
            window.draw(cooldownText);
            
            sf::Text roomPlayersTitle(font);
            roomPlayersTitle.setString("Players in this room:");
            roomPlayersTitle.setCharacterSize(18);
            roomPlayersTitle.setFillColor(sf::Color::Cyan);
            roomPlayersTitle.setPosition({40, 200});
            window.draw(roomPlayersTitle);
            
            int yPos = 230;
            bool someoneHere = false;
            for (int i = 0; i < 9; i++)
            {
                if (!currentGame->bots[i].isDead && 
                    currentGame->bots[i].currentRoomIndex == currentGame->player.currentRoomIndex)
                {
                    sf::Text playerName(font);
                    playerName.setString("- " + currentGame->bots[i].name);
                    playerName.setCharacterSize(16);
                    
                    if (currentGame->bots[i].isImpostor) {
                        playerName.setFillColor(sf::Color::Red);
                        playerName.setString(playerName.getString() + " (Impostor)");
                    } else {
                        playerName.setFillColor(sf::Color::White);
                    }
                    
                    playerName.setPosition({50, static_cast<float>(yPos)});
                    window.draw(playerName);
                    yPos += 25;
                    someoneHere = true;
                }
            }
            
            if (!someoneHere)
            {
                sf::Text noOne(font);
                noOne.setString("(No one else here)");
                noOne.setCharacterSize(16);
                noOne.setFillColor(sf::Color(100, 100, 100));
                noOne.setPosition({50, 230});
                window.draw(noOne);
            }
        }
        else
        {
            sf::Text tasksTitle(font);
            tasksTitle.setString("YOUR TASKS");
            tasksTitle.setCharacterSize(22);
            tasksTitle.setFillColor(sf::Color::Green);
            tasksTitle.setStyle(sf::Text::Bold);
            tasksTitle.setPosition({40, 95});
            window.draw(tasksTitle);
            
            int yPos = 130;
            int taskNum = 1;
            
            if (currentGame->player.assignedTasks.head == nullptr)
            {
                sf::Text allDone(font);
                allDone.setString("All tasks completed!");
                allDone.setCharacterSize(18);
                allDone.setFillColor(sf::Color::Green);
                allDone.setPosition({40, 130});
                window.draw(allDone);
            }
            else
            {
                ListNode* tmp = currentGame->player.assignedTasks.head;
                while (tmp != nullptr)
                {
                    string taskName = tmp->data;
                    string room = currentGame->taskManager.getTaskRoom(taskName);
                    string password = currentGame->taskManager.getTaskPassword(taskName);
                    
                    sf::Text taskText(font);
                    taskText.setString(to_string(taskNum) + ". " + taskName);
                    taskText.setCharacterSize(15);
                    taskText.setFillColor(sf::Color::White);
                    taskText.setPosition({40, static_cast<float>(yPos)});
                    window.draw(taskText);
                    
                    sf::Text taskDetails(font);
                    taskDetails.setString("   Room: " + room + " | PW: " + password);
                    taskDetails.setCharacterSize(13);
                    taskDetails.setFillColor(sf::Color(150, 150, 150));
                    taskDetails.setPosition({40, static_cast<float>(yPos + 18)});
                    window.draw(taskDetails);
                    
                    yPos += 45;
                    taskNum++;
                    tmp = tmp->next;
                }
            }
        }
        
        sf::RectangleShape rightPanel;
        rightPanel.setSize({350, 450});
        rightPanel.setPosition({910, 80});
        rightPanel.setFillColor(sf::Color(25, 25, 35));
        rightPanel.setOutlineThickness(2);
        rightPanel.setOutlineColor(sf::Color(60, 60, 70));
        window.draw(rightPanel);
        
        sf::Text statusTitle(font);
        statusTitle.setString("GAME STATUS");
        statusTitle.setCharacterSize(22);
        statusTitle.setFillColor(sf::Color::Yellow);
        statusTitle.setStyle(sf::Text::Bold);
        statusTitle.setPosition({930, 95});
        window.draw(statusTitle);
        
        int aliveCrew = 0;
        int aliveImpostor = 0;
        
        if (!currentGame->player.isDead) {
            if (currentGame->player.isImpostor) aliveImpostor++;
            else aliveCrew++;
        }
        
        for (int i = 0; i < 9; i++) {
            if (!currentGame->bots[i].isDead) {
                if (currentGame->bots[i].isImpostor) aliveImpostor++;
                else aliveCrew++;
            }
        }
        
        sf::Text aliveText(font);
        aliveText.setString("Alive Crewmates: " + to_string(aliveCrew));
        aliveText.setCharacterSize(18);
        aliveText.setFillColor(sf::Color::Green);
        aliveText.setPosition({930, 135});
        window.draw(aliveText);
        
        sf::Text impostorText(font);
        impostorText.setString("Alive Impostors: " + to_string(aliveImpostor));
        impostorText.setCharacterSize(18);
        impostorText.setFillColor(sf::Color::Red);
        impostorText.setPosition({930, 165});
        window.draw(impostorText);
        
        int meetingCooldown = 30 - (currentGame->roundNumber - currentGame->lastMeetingRound);
        sf::Text meetingCooldownText(font);
        if (meetingCooldown > 0) {
            meetingCooldownText.setString("Meeting Cooldown: " + to_string(meetingCooldown) + " rounds");
            meetingCooldownText.setFillColor(sf::Color(150, 150, 150));
        } else {
            meetingCooldownText.setString("Meeting Available!");
            meetingCooldownText.setFillColor(sf::Color::Cyan);
        }
        meetingCooldownText.setCharacterSize(16);
        meetingCooldownText.setPosition({930, 200});
        window.draw(meetingCooldownText);
        
        if (!currentGame->player.isImpostor && doTaskButton) {
            doTaskButton->setNormalColor(sf::Color(50, 150, 50));
            doTaskButton->setHoverColor(sf::Color(70, 200, 70));
            doTaskButton->draw(window);
        }
        
        if (currentGame->player.isImpostor && killButton) {
            killButton->setNormalColor(sf::Color(180, 50, 50));
            killButton->setHoverColor(sf::Color(220, 70, 70));
            killButton->draw(window);
        }
        
        if (meetingButton) {
            if (meetingCooldown > 0) {
                meetingButton->setNormalColor(sf::Color(80, 80, 80));
            } else {
                meetingButton->setNormalColor(sf::Color(100, 150, 200));
            }
            meetingButton->draw(window);
        }
        
        if (moveNorthButton) moveNorthButton->draw(window);
        if (moveWestButton) moveWestButton->draw(window);
        if (moveSouthButton) moveSouthButton->draw(window);
        if (moveEastButton) moveEastButton->draw(window);
        
        if (gameQuitButton) {
            gameQuitButton->setNormalColor(sf::Color(100, 50, 50));
            gameQuitButton->draw(window);
        }
        
        if (showTaskDialog)
        {
            sf::RectangleShape overlay;
            overlay.setSize({1280, 720});
            overlay.setFillColor(sf::Color(0, 0, 0, 180));
            window.draw(overlay);
            
            sf::RectangleShape dialogBox;
            dialogBox.setSize({600, 300});
            dialogBox.setPosition({340, 200});
            dialogBox.setFillColor(sf::Color(30, 30, 40));
            dialogBox.setOutlineThickness(3);
            dialogBox.setOutlineColor(sf::Color(100, 150, 200));
            window.draw(dialogBox);
            
            sf::Text title(font);
            title.setString("Complete Task");
            title.setCharacterSize(30);
            title.setFillColor(sf::Color::White);
            title.setStyle(sf::Text::Bold);
            
            bounds = title.getLocalBounds();
            title.setOrigin({bounds.size.x / 2.0f, 0});
            title.setPosition({640, 220});
            window.draw(title);
            
            sf::Text instructions(font);
            instructions.setString("Enter the password for the task you want to complete:");
            instructions.setCharacterSize(16);
            instructions.setFillColor(sf::Color(200, 200, 200));
            
            bounds = instructions.getLocalBounds();
            instructions.setOrigin({bounds.size.x / 2.0f, 0});
            instructions.setPosition({640, 270});
            window.draw(instructions);
            
            if (taskPasswordBox) taskPasswordBox->draw(window);
            if (taskSubmitButton) taskSubmitButton->draw(window);
            if (taskCancelButton) taskCancelButton->draw(window);
            
            if (!dialogMessage.empty())
            {
                sf::Text msg(font);
                msg.setString(dialogMessage);
                msg.setCharacterSize(18);
                msg.setFillColor(sf::Color::Yellow);
                
                bounds = msg.getLocalBounds();
                msg.setOrigin({bounds.size.x / 2.0f, 0});
                msg.setPosition({640, 470});
                window.draw(msg);
            }
        }
        else if (showKillDialog)
        {
            sf::RectangleShape overlay;
            overlay.setSize({1280, 720});
            overlay.setFillColor(sf::Color(0, 0, 0, 180));
            window.draw(overlay);
            
            sf::RectangleShape dialogBox;
            dialogBox.setSize({600, static_cast<float>(200 + killTargetButtons.size() * 70)});
            dialogBox.setPosition({340, 150});
            dialogBox.setFillColor(sf::Color(40, 20, 20));
            dialogBox.setOutlineThickness(3);
            dialogBox.setOutlineColor(sf::Color(200, 50, 50));
            window.draw(dialogBox);
            
            sf::Text title(font);
            title.setString("SELECT TARGET TO KILL");
            title.setCharacterSize(28);
            title.setFillColor(sf::Color::Red);
            title.setStyle(sf::Text::Bold);
            
            bounds = title.getLocalBounds();
            title.setOrigin({bounds.size.x / 2.0f, 0});
            title.setPosition({640, 170});
            window.draw(title);
            
            for (auto& btn : killTargetButtons)
            {
                if (btn)
                {
                    btn->setNormalColor(sf::Color(150, 40, 40));
                    btn->setHoverColor(sf::Color(200, 60, 60));
                    btn->draw(window);
                }
            }
        }
        
        if (!dialogMessage.empty() && !showTaskDialog && !showKillDialog)
        {
            sf::Text msg(font);
            msg.setString(dialogMessage);
            msg.setCharacterSize(20);
            msg.setFillColor(sf::Color::Yellow);
            
            bounds = msg.getLocalBounds();
            msg.setOrigin({bounds.size.x / 2.0f, 0});
            msg.setPosition({640, 650});
            window.draw(msg);
        }
    }
    
    void renderMeeting()
    {
        if (!currentGame)
        {
            setState(GameState::MAIN_MENU);
            return;
        }
        
        sf::RectangleShape bg;
        bg.setSize({1280, 720});
        bg.setFillColor(sf::Color(20, 15, 15));
        window.draw(bg);
        
        sf::RectangleShape banner;
        banner.setSize({1280, 100});
        banner.setFillColor(sf::Color(150, 30, 30));
        window.draw(banner);
        
        sf::Text title(font);
        title.setString("EMERGENCY MEETING!");
        title.setCharacterSize(60);
        title.setFillColor(sf::Color::White);
        title.setStyle(sf::Text::Bold);
        
        sf::FloatRect bounds = title.getLocalBounds();
        title.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        title.setPosition({640, 50});
        window.draw(title);
        
        sf::Text playersTitle(font);
        playersTitle.setString("Alive Players:");
        playersTitle.setCharacterSize(28);
        playersTitle.setFillColor(sf::Color::Yellow);
        playersTitle.setPosition({100, 130});
        window.draw(playersTitle);
        
        int yPos = 180;
        
        if (!currentGame->player.isDead)
        {
            sf::Text playerName(font);
            playerName.setString("- " + currentGame->player.name + " (YOU)");
            playerName.setCharacterSize(20);
            
            if (currentGame->player.isImpostor)
            {
                playerName.setFillColor(sf::Color(255, 100, 100));
                playerName.setString(playerName.getString() + " [IMPOSTOR]");
            }
            else
            {
                playerName.setFillColor(sf::Color::White);
            }
            
            playerName.setPosition({120, static_cast<float>(yPos)});
            window.draw(playerName);
            yPos += 30;
        }
        
        for (int i = 0; i < 9; i++)
        {
            if (!currentGame->bots[i].isDead)
            {
                sf::Text botName(font);
                botName.setString("- " + currentGame->bots[i].name);
                botName.setCharacterSize(20);
                
                if (currentGame->player.isImpostor && currentGame->bots[i].isImpostor)
                {
                    botName.setFillColor(sf::Color::Red);
                    botName.setString(botName.getString() + " [IMPOSTOR]");
                }
                else
                {
                    botName.setFillColor(sf::Color::White);
                }
                
                botName.setPosition({120, static_cast<float>(yPos)});
                window.draw(botName);
                yPos += 30;
            }
        }
        
        sf::RectangleShape votePanel;
        votePanel.setSize({700, 280});
        votePanel.setPosition({290, 350});
        votePanel.setFillColor(sf::Color(30, 30, 40));
        votePanel.setOutlineThickness(3);
        votePanel.setOutlineColor(sf::Color(100, 100, 150));
        window.draw(votePanel);
        
        sf::Text voteTitle(font);
        voteTitle.setString("CAST YOUR VOTE");
        voteTitle.setCharacterSize(32);
        voteTitle.setFillColor(sf::Color::Cyan);
        voteTitle.setStyle(sf::Text::Bold);
        
        bounds = voteTitle.getLocalBounds();
        voteTitle.setOrigin({bounds.size.x / 2.0f, 0});
        voteTitle.setPosition({640, 370});
        window.draw(voteTitle);
        
        sf::Text instructions(font);
        instructions.setString("Type the name of who you want to eject, or click Skip");
        instructions.setCharacterSize(16);
        instructions.setFillColor(sf::Color(180, 180, 180));
        
        bounds = instructions.getLocalBounds();
        instructions.setOrigin({bounds.size.x / 2.0f, 0});
        instructions.setPosition({640, 415});
        window.draw(instructions);
        
        if (voteInputBox) voteInputBox->draw(window);
        if (voteSubmitButton) voteSubmitButton->draw(window);
        if (voteSkipButton) voteSkipButton->draw(window);
        
        if (!votingResult.empty())
        {
            sf::Text result(font);
            result.setString(votingResult);
            result.setCharacterSize(18);
            result.setFillColor(sf::Color::Yellow);
            
            bounds = result.getLocalBounds();
            result.setOrigin({bounds.size.x / 2.0f, 0});
            result.setPosition({640, 600});
            window.draw(result);
        }
    }
    
    void renderGameOver()
    {
        if (!currentGame)
        {
            setState(GameState::MAIN_MENU);
            return;
        }
        
        sf::RectangleShape bg;
        bg.setSize({1280, 720});
        
        if (currentGame->playerWon)
        {
            bg.setFillColor(sf::Color(20, 40, 20));
        }
        else
        {
            bg.setFillColor(sf::Color(40, 20, 20));
        }
        window.draw(bg);
        
        sf::RectangleShape banner;
        banner.setSize({1000, 150});
        banner.setPosition({140, 100});
        
        if (currentGame->playerWon)
        {
            banner.setFillColor(sf::Color(50, 150, 50));
        }
        else
        {
            banner.setFillColor(sf::Color(150, 50, 50));
        }
        
        banner.setOutlineThickness(5);
        banner.setOutlineColor(sf::Color::White);
        window.draw(banner);
        
        sf::Text resultText(font);
        
        if (currentGame->playerWon)
        {
            resultText.setString("VICTORY!");
            resultText.setFillColor(sf::Color::Green);
        }
        else
        {
            resultText.setString("DEFEAT!");
            resultText.setFillColor(sf::Color::Red);
        }
        
        resultText.setCharacterSize(80);
        resultText.setStyle(sf::Text::Bold);
        
        sf::FloatRect bounds = resultText.getLocalBounds();
        resultText.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        resultText.setPosition({640, 175});
        window.draw(resultText);
        
        sf::Text roleText(font);
        
        if (currentGame->player.isImpostor)
        {
            roleText.setString("You were the IMPOSTOR");
            roleText.setFillColor(sf::Color(255, 100, 100));
        }
        else
        {
            roleText.setString("You were a CREWMATE");
            roleText.setFillColor(sf::Color(100, 255, 100));
        }
        
        roleText.setCharacterSize(32);
        
        bounds = roleText.getLocalBounds();
        roleText.setOrigin({bounds.size.x / 2.0f, 0});
        roleText.setPosition({640, 280});
        window.draw(roleText);
        
        sf::Text statsText(font);
        statsText.setString("Rounds Survived: " + to_string(currentGame->roundNumber));
        statsText.setCharacterSize(24);
        statsText.setFillColor(sf::Color::White);
        
        bounds = statsText.getLocalBounds();
        statsText.setOrigin({bounds.size.x / 2.0f, 0});
        statsText.setPosition({640, 350});
        window.draw(statsText);
        
        if (playAgainButton)
        {
            playAgainButton->setNormalColor(sf::Color(50, 150, 50));
            playAgainButton->setHoverColor(sf::Color(70, 200, 70));
            playAgainButton->draw(window);
        }
        
        if (gameOverBackButton)
        {
            gameOverBackButton->setNormalColor(sf::Color(70, 70, 70));
            gameOverBackButton->setHoverColor(sf::Color(100, 100, 100));
            gameOverBackButton->draw(window);
        }
    }
};

#endif