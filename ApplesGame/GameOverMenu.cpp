#include "GameOverMenu.h"

namespace SnakeGame
{
    // Constructor initializes selection state
    GameOverMenu::GameOverMenu()
        : mSelectedIndex(0), mConfirmed(false)
    {
    }

    // Sets up font and text elements for the menu
    void GameOverMenu::setFont(const sf::Font& font)
    {
        mFont = font;

        // Configure prompt text
        mPrompt.setFont(mFont);
        mPrompt.setString("Enter a name");
        mPrompt.setCharacterSize(42);
        mPrompt.setFillColor(sf::Color::White);
        mPrompt.setOutlineColor(sf::Color::Black);
        mPrompt.setOutlineThickness(3.f);
        sf::FloatRect promptBounds = mPrompt.getLocalBounds();
        mPrompt.setOrigin(promptBounds.left + promptBounds.width / 2, promptBounds.top + promptBounds.height / 2);
        mPrompt.setPosition(420.f, 320.f);

        // Create Yes/No option texts
        std::vector<std::string> texts = { "No", "Yes" };
        for (int i = 0; i < 2; ++i)
        {
            sf::Text text;
            text.setFont(mFont);
            text.setString(texts[i]);
            text.setCharacterSize(40);
            text.setFillColor(sf::Color::White);
            text.setPosition(325.f + i * 150.f, 350.f);
            mOptions.push_back(text);
        }

        updateVisuals();
    }

    // Handles keyboard input for menu navigation
    void GameOverMenu::handleInput(sf::Keyboard::Key key)
    {
        if (mConfirmed) return;

        // Handle left/right navigation
        if (key == sf::Keyboard::Left)
        {
            mSelectedIndex = (mSelectedIndex + mOptions.size() - 1) % mOptions.size();
            updateVisuals();
        }
        else if (key == sf::Keyboard::Right)
        {
            mSelectedIndex = (mSelectedIndex + 1) % mOptions.size();
            updateVisuals();
        }
        else if (key == sf::Keyboard::Enter)
        {
            mConfirmed = true; 
        }
    }

    // Draws all menu elements
    void GameOverMenu::draw(sf::RenderWindow& window)
    {
        window.draw(mPrompt); 
        for (const auto& option : mOptions) 
        {
            window.draw(option);
        }
    }

    // Updates visual appearance of menu items
    void GameOverMenu::updateVisuals()
    {
        for (int i = 0; i < mOptions.size(); ++i)
        {
            // Highlight selected option
            mOptions[i].setOutlineThickness(i == mSelectedIndex ? 4.f : 0.f);
            mOptions[i].setOutlineColor(sf::Color::Red);
        }
    }

    // Returns confirmation state
    bool GameOverMenu::isConfirmed() const
    {
        return mConfirmed;
    }

    // Returns currently selected option
    GameOverSelection GameOverMenu::getSelected() const
    {
        return mSelectedIndex == 0 ? GameOverSelection::No : GameOverSelection::Yes;
    }
}





