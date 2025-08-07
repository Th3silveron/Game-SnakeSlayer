#include "Menu.h"
#include <iostream>
#include <sstream>
#include <iomanip>

// дай бог здоровь€ провер€ющему ;)

namespace SnakeGame
{
    // Constructor - Initializes menu with window dimensions and audio reference
    Menu::Menu(int width, int height, AudioManager& audio)
        : mSelectedIndex(0), mWidth(width), mHeight(height), mState(MenuState::Main),
        mAlphaValue(100.f), mIncreasingAlpha(true), mAudio(audio),
        mSoundState(SoundState::On), mMusicState(MusicState::On),
        mCurrentDifficulty(DifficultyLevel::Easy)
    {
        if (!mFont.loadFromFile("Resources/Fonts/Doom2016Right.ttf"))
        {
            std::cerr << "Failed to load font\n";
        }

        loadResources();
        setMenu(MenuState::Main);
        mHighScoreTable.loadFromFile();
    }

    // Sound effect wrapper methods
    void Menu::playSelectSound()
    {
        mAudio.playSound("Menu_select");
    }

    void Menu::playConfirmSound()
    {
        mAudio.playSound("Menu_confirm");
    }

    void Menu::playMoveSound()
    {
        mAudio.playSound("Menu_move");
    }

    // Load background texture and setup sprite properties
    void Menu::loadResources()
    {
        if (!mPentaTexture.loadFromFile("Resources/penta.png"))
        {
            std::cerr << "Failed to load texture\n";
        }
        mPentaSprite.setTexture(mPentaTexture);

        sf::FloatRect bounds = mPentaSprite.getLocalBounds();
        mPentaSprite.setOrigin(bounds.width / 2, bounds.height / 2);
        mPentaSprite.setPosition(mWidth / 2.f, mHeight / 2.f);
    }

    // Update animation for pulsing background effect
    void Menu::update()
    {
        float deltaTime = mAnimationClock.restart().asSeconds();
        float alphaChange = deltaTime * 100.f;

        if (mIncreasingAlpha)
        {
            mAlphaValue += alphaChange;
            if (mAlphaValue >= 220.f)
            {
                mAlphaValue = 220.f;
                mIncreasingAlpha = false;
            }
        }
        else
        {
            mAlphaValue -= alphaChange;
            if (mAlphaValue <= 100.f)
            {
                mAlphaValue = 100.f;
                mIncreasingAlpha = true;
            }
        }

        mPentaSprite.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(mAlphaValue)));
    }

    // Reload and refresh leaderboard display
    void Menu::refreshLeaderboard()
    {
        mHighScoreTable.loadFromFile();
        if (mState == MenuState::Leaderboard)
        {
            setupLeaderboard();
        }
    }

    // Setup options menu with sound/music toggles
    void Menu::setupOptionsMenu()
    {
        mOptionsItems.clear();

        std::vector<std::string> options = {
            "Sound: " + getSoundStateText(),
            "Music: " + getMusicStateText(),
            "Back"
        };

        for (size_t i = 0; i < options.size(); ++i)
        {
            sf::Text text;
            text.setFont(mFont);
            text.setString(options[i]);
            text.setCharacterSize(48);
            text.setFillColor(sf::Color::White);
            sf::FloatRect bounds = text.getLocalBounds();
            text.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
            text.setPosition(mWidth / 2.f, mHeight / 3.5f + i * 70.f);
            mOptionsItems.push_back(text);
        }

        mSelectedIndex = 0;
        updateVisuals();
    }

    // Helper methods for state text display
    std::string Menu::getSoundStateText() const
    {
        return mSoundState == SoundState::On ? "ON" : "OFF";
    }

    std::string Menu::getMusicStateText() const
    {
        return mMusicState == MusicState::On ? "ON" : "OFF";
    }

    std::string Menu::getDifficultyText() const
    {
        switch (mCurrentDifficulty)
        {
        case DifficultyLevel::Easy:   return "EASY";
        case DifficultyLevel::Normal: return "NORMAL";
        case DifficultyLevel::Hell:   return "HELL";
        default: return "EASY";
        }
    }

    // Setup difficulty selection menu
    void Menu::setupDifficultyMenu()
    {
        mDifficultyItems.clear();

        mDifficultyText.setFont(mFont);
        mDifficultyText.setString("Difficulty: " + getDifficultyText());
        mDifficultyText.setCharacterSize(48);
        mDifficultyText.setFillColor(sf::Color::White);

        sf::FloatRect bounds = mDifficultyText.getLocalBounds();
        mDifficultyText.setOrigin(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);
        mDifficultyText.setPosition(mWidth / 2.f, mHeight / 3.5f);

        mDifficultyItems.push_back(mDifficultyText);

        sf::Text backText;
        backText.setFont(mFont);
        backText.setString("Back");
        backText.setCharacterSize(48);
        backText.setFillColor(sf::Color::White);

        sf::FloatRect backBounds = backText.getLocalBounds();
        backText.setOrigin(backBounds.left + backBounds.width / 2, backBounds.top + backBounds.height / 2);
        backText.setPosition(mWidth / 2.f, mHeight / 3.5f + 70.f);

        mDifficultyItems.push_back(backText);

        mSelectedIndex = 0;
        updateVisuals();
    }

    // Configure menu items based on current state
    void Menu::setMenu(MenuState state)
    {
        mState = state;
        mItems.clear();

        if (state == MenuState::Main)
        {
            std::vector<std::string> options = {
                "Start Game",
                "Options",
                "Difficulty Level",
                "Leaderboard",
                "Exit"
            };

            for (size_t i = 0; i < options.size(); ++i)
            {
                sf::Text text;
                text.setFont(mFont);
                text.setString(options[i]);
                text.setCharacterSize(48);
                text.setFillColor(sf::Color::White);
                sf::FloatRect bounds = text.getLocalBounds();
                text.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
                text.setPosition(mWidth / 2.f, mHeight / 3.5f + i * 70.f);
                mItems.push_back(text);
            }
        }
        else if (state == MenuState::Options)
        {
            setupOptionsMenu();
            return;
        }
        else if (state == MenuState::Difficulty)
        {
            setupDifficultyMenu();
            return;
        }
        else if (state == MenuState::Leaderboard)
        {
            setupLeaderboard();
            return;
        }

        mSelectedIndex = 0;
        updateVisuals();
    }

    // Setup leaderboard display with high scores
    void Menu::setupLeaderboard()
    {
        mItems.clear();
        mLeaderboardItems.clear();

        sf::Text title;
        title.setFont(mFont);
        title.setString("LEADERBOARD");
        title.setCharacterSize(60);
        title.setFillColor(sf::Color::Yellow);
        title.setOutlineColor(sf::Color::Black);
        title.setOutlineThickness(3.f);
        sf::FloatRect titleBounds = title.getLocalBounds();
        title.setOrigin(titleBounds.left + titleBounds.width / 2.f, titleBounds.top + titleBounds.height / 2.f);
        title.setPosition(mWidth / 2.f, 80.f);
        mItems.push_back(title);

        const auto& entries = mHighScoreTable.getEntries();
        for (size_t i = 0; i < entries.size(); ++i)
        {
            std::ostringstream oss;
            oss << (i + 1) << ". " << std::setw(3) << std::left << entries[i].name
                << " " << std::setw(4) << std::right << entries[i].score;

            sf::Text entryText;
            entryText.setFont(mFont);
            entryText.setString(oss.str());
            entryText.setCharacterSize(48);
            entryText.setFillColor(sf::Color::White);
            entryText.setOutlineColor(sf::Color::Black);
            entryText.setOutlineThickness(2.f);
            sf::FloatRect entryBounds = entryText.getLocalBounds();
            entryText.setOrigin(entryBounds.left + entryBounds.width / 2.f, entryBounds.top + entryBounds.height / 2.f);
            entryText.setPosition(mWidth / 2.f, 180.f + i * 60.f);
            mLeaderboardItems.push_back(entryText);
        }

        sf::Text backText;
        backText.setFont(mFont);
        backText.setString("Back");
        backText.setCharacterSize(48);
        backText.setFillColor(sf::Color::White);
        backText.setOutlineColor(sf::Color::Red);
        backText.setOutlineThickness(4.f);
        sf::FloatRect backBounds = backText.getLocalBounds();
        backText.setOrigin(backBounds.left + backBounds.width / 2.f, backBounds.top + backBounds.height / 2.f);
        backText.setPosition(mWidth / 2.f, mHeight - 100.f);
        mItems.push_back(backText);

        mSelectedIndex = mItems.size() - 1;
        updateVisuals();
    }

    // Handle keyboard input for menu navgation
    bool Menu::handleInput(sf::Keyboard::Key key)
    {
        if (mState == MenuState::Options)
        {
            if (key == sf::Keyboard::Up)
            {
                mSelectedIndex = (mSelectedIndex + mOptionsItems.size() - 1) % mOptionsItems.size();
                updateVisuals();
                playMoveSound();
            }
            else if (key == sf::Keyboard::Down)
            {
                mSelectedIndex = (mSelectedIndex + 1) % mOptionsItems.size();
                updateVisuals();
                playMoveSound();
            }
            else if (key == sf::Keyboard::Left || key == sf::Keyboard::Right)
            {
                if (mSelectedIndex == 0)
                {
                    mSoundState = (mSoundState == SoundState::On) ? SoundState::Off : SoundState::On;
                    mAudio.setSoundEnabled(mSoundState == SoundState::On);
                    mOptionsItems[0].setString("Sound: " + getSoundStateText());
                    playSelectSound();
                }
                else if (mSelectedIndex == 1)
                {
                    mMusicState = (mMusicState == MusicState::On) ? MusicState::Off : MusicState::On;
                    mAudio.setMusicEnabled(mMusicState == MusicState::On);
                    mOptionsItems[1].setString("Music: " + getMusicStateText());
                    playSelectSound();
                }
            }
            else if (key == sf::Keyboard::Enter)
            {
                if (mOptionsItems[mSelectedIndex].getString().find("Back") != std::string::npos)
                {
                    playConfirmSound();
                    setMenu(MenuState::Main);
                }
            }
            return false;
        }
        else if (mState == MenuState::Difficulty)
        {
            if (key == sf::Keyboard::Left || key == sf::Keyboard::Right)
            {
                if (mSelectedIndex == 0)
                {
                    DifficultyLevel previous = mCurrentDifficulty;

                    if (key == sf::Keyboard::Left)
                    {
                        mCurrentDifficulty = static_cast<DifficultyLevel>(
                            (static_cast<int>(mCurrentDifficulty) + 2) % 3);
                    }
                    else
                    {
                        mCurrentDifficulty = static_cast<DifficultyLevel>(
                            (static_cast<int>(mCurrentDifficulty) + 1) % 3);
                    }

                    if (previous != mCurrentDifficulty)
                    {
                        mDifficultyText.setString("Difficulty: " + getDifficultyText());
                        mDifficultyItems[0] = mDifficultyText;
                        updateVisuals();
                        playSelectSound();
                    }
                }
            }
            else if (key == sf::Keyboard::Up || key == sf::Keyboard::Down)
            {
                mSelectedIndex = (mSelectedIndex + 1) % mDifficultyItems.size();
                updateVisuals();
                playMoveSound();
            }
            else if (key == sf::Keyboard::Enter)
            {
                if (mDifficultyItems[mSelectedIndex].getString() == "Back")
                {
                    playConfirmSound();
                    setMenu(MenuState::Main);
                }
            }
            return false;
        }

        if (key == sf::Keyboard::Up)
        {
            mSelectedIndex = (mSelectedIndex + mItems.size() - 1) % mItems.size();
            updateVisuals();
            playMoveSound();
        }
        else if (key == sf::Keyboard::Down)
        {
            mSelectedIndex = (mSelectedIndex + 1) % mItems.size();
            updateVisuals();
            playMoveSound();
        }
        else if (key == sf::Keyboard::Enter)
        {
            if (mState == MenuState::Main)
            {
                int selected = getSelectedItem();
                playConfirmSound();
                if (selected == 1)
                    setMenu(MenuState::Options);
                else if (selected == 2)
                    setMenu(MenuState::Difficulty);
                else if (selected == 3)
                    setMenu(MenuState::Leaderboard);
                else
                    return true;
            }
            else if (mState == MenuState::Leaderboard)
            {
                if (mItems[mSelectedIndex].getString() == "Back")
                {
                    playConfirmSound();
                    setMenu(MenuState::Main);
                }
            }
            else
            {
                if (mItems[mSelectedIndex].getString() == "Back")
                {
                    playConfirmSound();
                    setMenu(MenuState::Main);
                }
            }
        }

        return false;
    }

    // Render current menu state
    void Menu::draw(sf::RenderWindow& window)
    {
        window.clear(sf::Color::Black);

        window.draw(mPentaSprite);

        if (mState == MenuState::Leaderboard)
        {
            for (const auto& item : mLeaderboardItems)
            {
                window.draw(item);
            }

            for (const auto& item : mItems)
            {
                if (item.getString() == "Back")
                {
                    window.draw(item);
                    break;
                }
            }
        }
        else if (mState == MenuState::Options)
        {
            for (const auto& item : mOptionsItems)
            {
                window.draw(item);
            }
        }
        else if (mState == MenuState::Difficulty)
        {
            for (const auto& item : mDifficultyItems)
            {
                window.draw(item);
            }
        }
        else
        {
            for (const auto& item : mItems)
            {
                window.draw(item);
            }
        }

        window.display();
    }

    // Get currently selected menu item index
    int Menu::getSelectedItem() const
    {
        return mSelectedIndex;
    }

    // Get current menu state
    MenuState Menu::getState() const
    {
        return mState;
    }

    // Update visual selection indicators
    void Menu::updateVisuals()
    {
        if (mState == MenuState::Options)
        {
            for (size_t i = 0; i < mOptionsItems.size(); ++i)
            {
                mOptionsItems[i].setOutlineThickness(i == mSelectedIndex ? 4.f : 0.f);
                mOptionsItems[i].setOutlineColor(sf::Color::Red);
            }
        }
        else if (mState == MenuState::Difficulty)
        {
            for (size_t i = 0; i < mDifficultyItems.size(); ++i)
            {
                mDifficultyItems[i].setOutlineThickness(i == mSelectedIndex ? 4.f : 0.f);
                mDifficultyItems[i].setOutlineColor(sf::Color::Red);
            }
        }
        else
        {
            for (size_t i = 0; i < mItems.size(); ++i)
            {
                mItems[i].setOutlineThickness(i == mSelectedIndex ? 4.f : 0.f);
                mItems[i].setOutlineColor(sf::Color::Red);
            }
        }
    }
}

















