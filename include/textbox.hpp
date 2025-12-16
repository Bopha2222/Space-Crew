#ifndef TEXTBOX_HPP
#define TEXTBOX_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

using namespace std;

class TextBox
{
private:
    sf::RectangleShape box;
    sf::Text displayText;
    sf::Text labelText;
    string inputString;
    bool isSelected;
    bool isPassword;
    float cursorBlinkTimer;
    bool showCursor;
    int maxLength;
    
    sf::Color normalColor;
    sf::Color selectedColor;
    sf::Color textColor;

public:
    // Constructor
    TextBox(float x, float y, float width, float height, 
            const string& label, sf::Font& font, bool password = false)
        : displayText(font), labelText(font),  // Initialize both texts with font
          isSelected(false), isPassword(password), cursorBlinkTimer(0), 
          showCursor(true), maxLength(50)
    {
        box.setPosition({x, y});
        box.setSize({width, height});
        
        normalColor = sf::Color(50, 50, 50);
        selectedColor = sf::Color(70, 70, 100);
        textColor = sf::Color::White;
        
        box.setFillColor(normalColor);
        box.setOutlineThickness(2);
        box.setOutlineColor(sf::Color(150, 150, 150));
        
        labelText.setString(label);
        labelText.setCharacterSize(18);
        labelText.setFillColor(sf::Color(200, 200, 200));
        labelText.setPosition({x, y - 25});
        
        displayText.setString("");
        displayText.setCharacterSize(20);
        displayText.setFillColor(textColor);
        displayText.setPosition({x + 10, y + height / 2 - 12});
        
        inputString = "";
    }
    
    void setPosition(float x, float y)
    {
        box.setPosition({x, y});
        labelText.setPosition({x, y - 25});
        float height = box.getSize().y;
        displayText.setPosition({x + 10, y + height / 2 - 12});
    }
    
    void setSize(float width, float height)
    {
        sf::Vector2f pos = box.getPosition();
        box.setSize({width, height});
        displayText.setPosition({pos.x + 10, pos.y + height / 2 - 12});
    }
    
    void setMaxLength(int length) 
    { 
        maxLength = length; 
    }
    
    void setSelected(bool selected) 
    { 
        isSelected = selected; 
    }
    
    void setPassword(bool password) 
    { 
        isPassword = password; 
    }
    
    string getText() const 
    { 
        return inputString; 
    }
    
    void setText(const string& text) 
    { 
        inputString = text; 
    }
    
    void clear() 
    { 
        inputString.clear(); 
    }
    
    bool getSelected() const 
    { 
        return isSelected; 
    }
    
    bool isMouseOver(const sf::RenderWindow& window) const
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
        return box.getGlobalBounds().contains(mousePosF);
    }
    
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window)
    {
        if (event.is<sf::Event::MouseButtonPressed>())
        {
            const auto& mouseButton = *event.getIf<sf::Event::MouseButtonPressed>();
            if (mouseButton.button == sf::Mouse::Button::Left)
            {
                isSelected = isMouseOver(window);
            }
        }
        else if (event.is<sf::Event::TextEntered>() && isSelected)
        {
            const auto& textEntered = *event.getIf<sf::Event::TextEntered>();
            if (textEntered.unicode < 128)
            {
                char enteredChar = static_cast<char>(textEntered.unicode);
                
                if (enteredChar == '\b')
                {
                    if (!inputString.empty())
                    {
                        inputString.pop_back();
                        updateDisplayText();
                    }
                }
                else if (enteredChar == '\r' || enteredChar == '\n')
                {
                    isSelected = false;
                }
                else if (enteredChar >= 32 && enteredChar < 127)
                {
                    if (inputString.length() < static_cast<size_t>(maxLength))
                    {
                        inputString += enteredChar;
                        updateDisplayText();
                    }
                }
            }
        }
    }
    
    void update(float deltaTime)
    {
        box.setFillColor(isSelected ? selectedColor : normalColor);
        box.setOutlineColor(isSelected ? sf::Color(100, 150, 255) : sf::Color(150, 150, 150));
        
        if (isSelected)
        {
            cursorBlinkTimer += deltaTime;
            if (cursorBlinkTimer >= 0.5f)
            {
                showCursor = !showCursor;
                cursorBlinkTimer = 0;
                updateDisplayText();
            }
        }
        else
        {
            showCursor = false;
        }
    }
    
    void draw(sf::RenderWindow& window)
    {
        window.draw(box);
        window.draw(labelText);
        window.draw(displayText);
    }
    
private:
    void updateDisplayText()
    {
        if (isPassword && !inputString.empty())
        {
            string masked(inputString.length(), '*');
            if (isSelected && showCursor)
            {
                displayText.setString(masked + "|");
            }
            else
            {
                displayText.setString(masked);
            }
        }
        else
        {
            if (isSelected && showCursor)
            {
                displayText.setString(inputString + "|");
            }
            else
            {
                displayText.setString(inputString);
            }
        }
    }
};

#endif