#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

namespace SnakeGame
{
    enum class GameOverSelection
    {
        No,
        Yes
    };

    class GameOverMenu
    {
    public:
        GameOverMenu();
        void setFont(const sf::Font& font);
        void draw(sf::RenderWindow& window);
        void handleInput(sf::Keyboard::Key key);
        bool isConfirmed() const;
        GameOverSelection getSelected() const;

    private:
        std::vector<sf::Text> mOptions;
        int mSelectedIndex;
        bool mConfirmed;
        sf::Text mPrompt;
        sf::Font mFont;

        void updateVisuals();
    };
}




