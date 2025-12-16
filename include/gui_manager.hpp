#ifndef GUIMANAGER_HPP
#define GUIMANAGER_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <string>

using namespace std;

enum class GameState
{
    WELCOME,
    LOGIN,
    SIGNUP,
    MAIN_MENU,
    ADMIN_MENU,
    GAME_DIFFICULTY,
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
    
public:
    // Constructor
    GUIManager()
    {
        currentState = GameState::WELCOME;
        previousState = GameState::WELCOME;
        isAdmin = false;
    }
    
    // Destructor
    ~GUIManager()
    {
    }
    
    // Initialize window and load font
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
        
        cout << "GUI initialized successfully!" << endl;
        return true;
    }
    
    // Main game loop
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
    
    // Getters and Setters
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
    // Handle keyboard/mouse events
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
        }
    }
    
    // Update game logic
    void update(float deltaTime)
    {
        // Game updates go here
    }
    
    // Render everything
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
        else if (currentState == GameState::ADMIN_PANEL)
        {
            renderAdminPanel();
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
    // SCREEN RENDERING FUNCTIONS
    // ========================================
    
    void renderWelcomeScreen()
    {
        // Title
        sf::Text title(font);
        title.setString("AMONG US");
        title.setCharacterSize(80);
        title.setFillColor(sf::Color::Red);
        title.setStyle(sf::Text::Bold);
        
        sf::FloatRect bounds = title.getLocalBounds();
        title.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        title.setPosition({640, 150});
        
        // Subtitle
        sf::Text subtitle(font);
        subtitle.setString("C++ Edition - GUI Version");
        subtitle.setCharacterSize(30);
        subtitle.setFillColor(sf::Color(200, 200, 200));
        
        bounds = subtitle.getLocalBounds();
        subtitle.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        subtitle.setPosition({640, 250});
        
        // Info
        sf::Text info(font);
        info.setString("Press ESC to close\n\nGUI is working!");
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
        title.setPosition({640, 100});
        
        window.draw(title);
    }
    
    void renderSignupScreen()
    {
        sf::Text title(font);
        title.setString("SIGN UP");
        title.setCharacterSize(50);
        title.setFillColor(sf::Color::White);
        
        sf::FloatRect bounds = title.getLocalBounds();
        title.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        title.setPosition({640, 100});
        
        window.draw(title);
    }
    
    void renderMainMenu()
    {
        sf::Text title(font);
        title.setString("MAIN MENU");
        title.setCharacterSize(50);
        title.setFillColor(sf::Color::White);
        
        sf::FloatRect bounds = title.getLocalBounds();
        title.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        title.setPosition({640, 100});
        
        window.draw(title);
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
    }
    
    void renderGamePlay()
    {
        sf::Text title(font);
        title.setString("GAME IN PROGRESS");
        title.setCharacterSize(40);
        title.setFillColor(sf::Color::Cyan);
        
        sf::FloatRect bounds = title.getLocalBounds();
        title.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        title.setPosition({640, 100});
        
        window.draw(title);
    }
    
    void renderLeaderboard()
    {
        sf::Text title(font);
        title.setString("LEADERBOARD");
        title.setCharacterSize(50);
        title.setFillColor(sf::Color::Yellow);
        
        sf::FloatRect bounds = title.getLocalBounds();
        title.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        title.setPosition({640, 100});
        
        window.draw(title);
    }
    
    void renderUserProfile()
    {
        sf::Text title(font);
        title.setString("USER PROFILE");
        title.setCharacterSize(40);
        title.setFillColor(sf::Color::Cyan);
        
        sf::FloatRect bounds = title.getLocalBounds();
        title.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        title.setPosition({640, 100});
        
        window.draw(title);
    }
    
    void renderAnalytics()
    {
        sf::Text title(font);
        title.setString("ANALYTICS");
        title.setCharacterSize(50);
        title.setFillColor(sf::Color(100, 255, 100));
        
        sf::FloatRect bounds = title.getLocalBounds();
        title.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        title.setPosition({640, 100});
        
        window.draw(title);
    }
    
    void renderAdminPanel()
    {
        sf::Text title(font);
        title.setString("ADMIN PANEL");
        title.setCharacterSize(50);
        title.setFillColor(sf::Color(255, 100, 100));
        
        sf::FloatRect bounds = title.getLocalBounds();
        title.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        title.setPosition({640, 100});
        
        window.draw(title);
    }
    
    void renderTaskManagement()
    {
        sf::Text title(font);
        title.setString("TASK MANAGEMENT");
        title.setCharacterSize(40);
        title.setFillColor(sf::Color::Magenta);
        
        sf::FloatRect bounds = title.getLocalBounds();
        title.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        title.setPosition({640, 100});
        
        window.draw(title);
    }
    
    void renderUserManagement()
    {
        sf::Text title(font);
        title.setString("USER MANAGEMENT");
        title.setCharacterSize(40);
        title.setFillColor(sf::Color(150, 150, 255));
        
        sf::FloatRect bounds = title.getLocalBounds();
        title.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        title.setPosition({640, 100});
        
        window.draw(title);
    }
    
    void renderSearchSystem()
    {
        sf::Text title(font);
        title.setString("SEARCH SYSTEM");
        title.setCharacterSize(50);
        title.setFillColor(sf::Color(255, 200, 100));
        
        sf::FloatRect bounds = title.getLocalBounds();
        title.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        title.setPosition({640, 100});
        
        window.draw(title);
    }
    
    void renderMeeting()
    {
        sf::Text title(font);
        title.setString("EMERGENCY MEETING!");
        title.setCharacterSize(60);
        title.setFillColor(sf::Color::Red);
        title.setStyle(sf::Text::Bold);
        
        sf::FloatRect bounds = title.getLocalBounds();
        title.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        title.setPosition({640, 200});
        
        window.draw(title);
    }
    
    void renderGameOver()
    {
        sf::Text title(font);
        title.setString("GAME OVER");
        title.setCharacterSize(70);
        title.setFillColor(sf::Color::Red);
        title.setStyle(sf::Text::Bold);
        
        sf::FloatRect bounds = title.getLocalBounds();
        title.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        title.setPosition({640, 200});
        
        window.draw(title);
    }
};

#endif