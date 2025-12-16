#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <functional>

using namespace std;

class Button
{
private:
    sf::RectangleShape shape;
    sf::Text text;
    sf::Color normalColor;
    sf::Color hoverColor;
    sf::Color clickColor;
    sf::Color currentColor;
    bool isHovered;
    bool isClicked;
    function<void()> callback;

public:
    // Constructor
    Button(float x, float y, float width, float height, 
           const string& label, sf::Font& font, unsigned int fontSize = 24)
        : isHovered(false), isClicked(false), text(font)  // Initialize text with font
    {
        shape.setPosition({x, y});
        shape.setSize({width, height});
        
        normalColor = sf::Color(70, 70, 70);
        hoverColor = sf::Color(100, 100, 100);
        clickColor = sf::Color(50, 50, 50);
        currentColor = normalColor;
        
        shape.setFillColor(normalColor);
        shape.setOutlineThickness(2);
        shape.setOutlineColor(sf::Color(150, 150, 150));
        
        text.setString(label);
        text.setCharacterSize(fontSize);
        text.setFillColor(sf::Color::White);
        
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setOrigin({textBounds.position.x + textBounds.size.x / 2.0f,
                        textBounds.position.y + textBounds.size.y / 2.0f});
        text.setPosition({x + width / 2.0f, y + height / 2.0f});
    }
    
    void setPosition(float x, float y)
    {
        shape.setPosition({x, y});
        sf::Vector2f size = shape.getSize();
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setOrigin({textBounds.position.x + textBounds.size.x / 2.0f,
                        textBounds.position.y + textBounds.size.y / 2.0f});
        text.setPosition({x + size.x / 2.0f, y + size.y / 2.0f});
    }
    
    void setSize(float width, float height)
    {
        sf::Vector2f pos = shape.getPosition();
        shape.setSize({width, height});
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setOrigin({textBounds.position.x + textBounds.size.x / 2.0f,
                        textBounds.position.y + textBounds.size.y / 2.0f});
        text.setPosition({pos.x + width / 2.0f, pos.y + height / 2.0f});
    }
    
    void setText(const string& label)
    {
        text.setString(label);
        sf::Vector2f pos = shape.getPosition();
        sf::Vector2f size = shape.getSize();
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setOrigin({textBounds.position.x + textBounds.size.x / 2.0f,
                        textBounds.position.y + textBounds.size.y / 2.0f});
        text.setPosition({pos.x + size.x / 2.0f, pos.y + size.y / 2.0f});
    }
    
    void setCallback(function<void()> func)
    {
        callback = func;
    }
    
    void setNormalColor(const sf::Color& color)
    {
        normalColor = color;
    }
    
    void setHoverColor(const sf::Color& color)
    {
        hoverColor = color;
    }
    
    void setClickColor(const sf::Color& color)
    {
        clickColor = color;
    }
    
    void setTextColor(const sf::Color& color)
    {
        text.setFillColor(color);
    }
    
    bool isMouseOver(const sf::RenderWindow& window) const
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
        return shape.getGlobalBounds().contains(mousePosF);
    }
    
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window)
    {
        if (event.is<sf::Event::MouseButtonPressed>())
        {
            const auto& mouseButton = *event.getIf<sf::Event::MouseButtonPressed>();
            if (mouseButton.button == sf::Mouse::Button::Left)
            {
                if (isMouseOver(window))
                {
                    isClicked = true;
                    currentColor = clickColor;
                    if (callback)
                    {
                        callback();
                    }
                }
            }
        }
        else if (event.is<sf::Event::MouseButtonReleased>())
        {
            const auto& mouseButton = *event.getIf<sf::Event::MouseButtonReleased>();
            if (mouseButton.button == sf::Mouse::Button::Left)
            {
                isClicked = false;
            }
        }
    }
    
    void update(const sf::RenderWindow& window)
    {
        isHovered = isMouseOver(window);
        
        if (isClicked)
        {
            currentColor = clickColor;
        }
        else if (isHovered)
        {
            currentColor = hoverColor;
        }
        else
        {
            currentColor = normalColor;
        }
        
        shape.setFillColor(currentColor);
    }
    
    void draw(sf::RenderWindow& window)
    {
        window.draw(shape);
        window.draw(text);
    }
    
    sf::FloatRect getBounds() const 
    { 
        return shape.getGlobalBounds(); 
    }
};

#endif