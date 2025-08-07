#include "Game.h"
#include "Menu.h"
#include <iostream>
#include <iomanip>
#include <sstream>

namespace SnakeGame
{
    // Initialize game state and setup UI elements
    Game::Game(AudioManager& audio, Menu& menu)
        : mAudio(audio), mMenu(menu), mGameOver(false), mScore(0),
        mReturnToMenu(false), mEnteringName(false), mPlayerName(""),
        mNameCharsEntered(0), mGameOverMenu(), mShowingLeaderboard(false),
        mDifficulty(DifficultyLevel::Easy), mUpdateInterval(UPDATE_INTERVAL), mApplesEaten(0)
    {
        loadResources();
        mGameOverOverlay.setSize(sf::Vector2f((GRID_WIDTH + HUD_WIDTH) * CELL_SIZE, GRID_HEIGHT * CELL_SIZE));
        mGameOverOverlay.setFillColor(sf::Color(255, 0, 0, 150));
        setupGameOverText();
        setupScoreText();
        setupNameEntryText();
    }

    // Load textures, fonts and setup game field scaling
    void Game::loadResources()
    {
        if (!mFieldTexture.loadFromFile("Resources/playing field.jpg"))
            std::cerr << "texture download error\n";
        if (!mFont.loadFromFile("C:\\Source\\HW\\Apple-game-master\\ApplesGame\\Resources\\Fonts\\Doom2016Right.ttf"))
            std::cerr << "font download error\n";

        mGameOverMenu.setFont(mFont);

        mFieldSprite.setTexture(mFieldTexture);
        float scaleX = static_cast<float>(GRID_WIDTH * CELL_SIZE) / mFieldTexture.getSize().x;
        float scaleY = static_cast<float>(GRID_HEIGHT * CELL_SIZE) / mFieldTexture.getSize().y;
        mFieldSprite.setScale(scaleX, scaleY);

        mHudArea.setSize(sf::Vector2f(HUD_WIDTH * CELL_SIZE, GRID_HEIGHT * CELL_SIZE));
        mHudArea.setFillColor(sf::Color(150, 0, 0));
        mHudArea.setPosition(GRID_WIDTH * CELL_SIZE, 0);
    }

    void Game::setupGameOverText()
    {
        mGameOverText.setFont(mFont);
        mGameOverText.setString("GAME OVER");
        mGameOverText.setCharacterSize(90);
        mGameOverText.setFillColor(sf::Color::White);
        mGameOverText.setOutlineColor(sf::Color::Black);
        mGameOverText.setOutlineThickness(4.f);
        sf::FloatRect textRect = mGameOverText.getLocalBounds();
        mGameOverText.setOrigin(textRect.left + textRect.width / 2, textRect.top + textRect.height / 2);
        mGameOverText.setPosition(((GRID_WIDTH + HUD_WIDTH) * CELL_SIZE) / 2.f, (GRID_HEIGHT * CELL_SIZE) / 2.f - 60);
    }

    void Game::setupScoreText()
    {
        mScoreLabel.setFont(mFont);
        mScoreLabel.setString("SCORE");
        mScoreLabel.setCharacterSize(85);
        mScoreLabel.setFillColor(sf::Color::White);
        mScoreLabel.setPosition(GRID_WIDTH * CELL_SIZE + 10, 20);

        mScoreValue.setFont(mFont);
        mScoreValue.setCharacterSize(105);
        mScoreValue.setFillColor(sf::Color::White);
        mScoreValue.setPosition(GRID_WIDTH * CELL_SIZE + 12, 80);
        updateScoreDisplay();
    }

    void Game::setupNameEntryText()
    {
        mNameEntryText.setFont(mFont);
        mNameEntryText.setString("Enter your name (3 letters): ___");
        mNameEntryText.setCharacterSize(42);
        mNameEntryText.setFillColor(sf::Color::White);
        mNameEntryText.setOutlineColor(sf::Color::Black);
        mNameEntryText.setOutlineThickness(3.f);
        sf::FloatRect textRect = mNameEntryText.getLocalBounds();
        mNameEntryText.setOrigin(textRect.left + textRect.width / 2, textRect.top + textRect.height / 2);
        mNameEntryText.setPosition(((GRID_WIDTH + HUD_WIDTH) * CELL_SIZE) / 2.f, (GRID_HEIGHT * CELL_SIZE) / 2.f + 60);
    }

    void Game::updateScoreDisplay()
    {
        std::ostringstream oss;
        oss << std::setw(4) << std::setfill('0') << mScore;
        mScoreValue.setString(oss.str());
    }

    // Main input handler routing to apropriate subsystems
    void Game::handleInput(sf::Keyboard::Key key)
    {
        if (mShowingLeaderboard)
        {
            if (key == sf::Keyboard::Enter)
            {
                mReturnToMenu = true;
            }
            return;
        }

        if (mEnteringName)
        {
            handleNameInput(key);
            return;
        }

        if (mGameOver)
        {
            mGameOverMenu.handleInput(key);
            if (mGameOverMenu.isConfirmed())
            {
                if (mGameOverMenu.getSelected() == GameOverSelection::No)
                {
                    mReturnToMenu = true;
                    mAudio.stopAllMusic();
                    mAudio.playMenuMusic();
                }
                else
                {
                    mEnteringName = true;
                    mPlayerName.clear();
                    mNameCharsEntered = 0;
                }
            }
            return;
        }

        switch (key)
        {
        case sf::Keyboard::Up:    mPlayer.setDirection(Direction::Up); break;
        case sf::Keyboard::Down:  mPlayer.setDirection(Direction::Down); break;
        case sf::Keyboard::Left:  mPlayer.setDirection(Direction::Left); break;
        case sf::Keyboard::Right: mPlayer.setDirection(Direction::Right); break;
        default: break;
        }
    }

    // Handle 3-character name input for high scores
    void Game::handleNameInput(sf::Keyboard::Key key)
    {
        if (key >= sf::Keyboard::A && key <= sf::Keyboard::Z && mNameCharsEntered < 3)
        {
            mPlayerName += static_cast<char>('A' + (key - sf::Keyboard::A));
            mNameCharsEntered++;

            std::string displayText = "Enter your name (3 letters): ";
            for (int i = 0; i < 3; ++i)
            {
                if (i < mNameCharsEntered)
                    displayText += mPlayerName[i];
                else
                    displayText += '_';
            }
            mNameEntryText.setString(displayText);

            if (mNameCharsEntered == 3)
            {
                mHighScoreTable.addEntry({ mPlayerName, mScore });
                mMenu.refreshLeaderboard();
                mShowingLeaderboard = true;
            }
        }
        else if (key == sf::Keyboard::BackSpace && mNameCharsEntered > 0)
        {
            mPlayerName.pop_back();
            mNameCharsEntered--;

            std::string displayText = "Enter your name (3 letters): ";
            for (int i = 0; i < 3; ++i)
            {
                if (i < mNameCharsEntered)
                    displayText += mPlayerName[i];
                else
                    displayText += '_';
            }
            mNameEntryText.setString(displayText);
        }
    }

    // Main game update with movement, collisions and difficulty handling
    void Game::update()
    {
        if (mGameOver || mEnteringName || mShowingLeaderboard) return;

        mApple.setSnakeSegments(mPlayer.getSegments());
        bool ateApple = mApple.update();

        if (ateApple)
        {
            mScore = std::min(mScore + 1, 9999);
            mApplesEaten++;
            updateScoreDisplay();
            mAudio.playSound("Apple_eaten");

            if (mDifficulty == DifficultyLevel::Normal || mDifficulty == DifficultyLevel::Hell)
            {
                mUpdateInterval *= 0.95f;
            }

            if (mDifficulty == DifficultyLevel::Hell && mApplesEaten % 5 == 0)
            {
                spawnHellBarrier();
            }
        }

        mPlayer.update(ateApple);

        if (mDifficulty == DifficultyLevel::Hell)
        {
            const auto& head = mPlayer.getSegments().front();
            for (const auto& barrier : mHellBarriers)
            {
                if (head == barrier)
                {
                    mGameOver = true;
                    mAudio.stopAllMusic();
                    mAudio.playSound("Game_over");
                    return;
                }
            }
        }

        if (mPlayer.isCollidingWithWall() || mPlayer.isCollidingWithSelf())
        {
            mGameOver = true;
            mAudio.stopAllMusic();
            mAudio.playSound("Game_over");
        }
    }

    void Game::spawnHellBarrier()
    {
        sf::Vector2i newPos;
        int attempts = 0;

        do {
            newPos.x = std::rand() % (GRID_WIDTH - 2) + 1;
            newPos.y = std::rand() % (GRID_HEIGHT - 2) + 1;
            attempts++;
        } while (!isValidBarrierPosition(newPos) && attempts < 100);

        if (attempts < 100 && mHellBarriers.size() < 8)
        {
            mHellBarriers.push_back(newPos);
        }
    }

    bool Game::isValidBarrierPosition(const sf::Vector2i& pos) const
    {
        for (const auto& segment : mPlayer.getSegments())
        {
            if (segment == pos)
                return false;
        }

        for (const auto& applePos : mApple.getPositions())
        {
            if (applePos == pos)
                return false;
        }

        for (const auto& barrier : mHellBarriers)
        {
            if (barrier == pos)
                return false;
        }

        return true;
    }

    // Render leaderboard with high scores
    void Game::drawLeaderboard(sf::RenderWindow& window)
    {
        sf::RectangleShape background(sf::Vector2f((GRID_WIDTH + HUD_WIDTH) * CELL_SIZE, GRID_HEIGHT * CELL_SIZE));
        background.setFillColor(sf::Color(0, 0, 0, 200));
        window.draw(background);

        sf::Text title;
        title.setFont(mFont);
        title.setString("LEADERBOARD");
        title.setCharacterSize(60);
        title.setFillColor(sf::Color::Yellow);
        title.setOutlineColor(sf::Color::Black);
        title.setOutlineThickness(3.f);
        sf::FloatRect titleBounds = title.getLocalBounds();
        title.setOrigin(titleBounds.left + titleBounds.width / 2.f, titleBounds.top + titleBounds.height / 2.f);
        title.setPosition(((GRID_WIDTH + HUD_WIDTH) * CELL_SIZE) / 2.f, 80.f);
        window.draw(title);

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
            entryText.setPosition(((GRID_WIDTH + HUD_WIDTH) * CELL_SIZE) / 2.f, 180.f + i * 60.f);
            window.draw(entryText);
        }

        sf::Text backText;
        backText.setFont(mFont);
        backText.setString("Press ENTER to continue");
        backText.setCharacterSize(36);
        backText.setFillColor(sf::Color::White);
        backText.setOutlineColor(sf::Color::Red);
        backText.setOutlineThickness(2.f);
        sf::FloatRect backBounds = backText.getLocalBounds();
        backText.setOrigin(backBounds.left + backBounds.width / 2.f, backBounds.top + backBounds.height / 2.f);
        backText.setPosition(((GRID_WIDTH + HUD_WIDTH) * CELL_SIZE) / 2.f, GRID_HEIGHT * CELL_SIZE - 80.f);
        window.draw(backText);
    }

    // Main render function handling all game states
    void Game::draw(sf::RenderWindow& window)
    {
        window.clear();
        window.draw(mFieldSprite);
        window.draw(mHudArea);
        mRock.draw(window);
        mApple.draw(window);

        if (mDifficulty == DifficultyLevel::Hell)
        {
            for (const auto& barrier : mHellBarriers)
            {
                mRock.drawAtPosition(window, barrier.x, barrier.y);
            }
        }

        mPlayer.draw(window);
        window.draw(mScoreLabel);
        window.draw(mScoreValue);

        if (mGameOver)
        {
            window.draw(mGameOverOverlay);
            window.draw(mGameOverText);

            if (mEnteringName && !mShowingLeaderboard)
            {
                window.draw(mNameEntryText);
            }
            else if (mShowingLeaderboard)
            {
                drawLeaderboard(window);
            }
            else
            {
                mGameOverMenu.draw(window);
            }
        }

        window.display();
    }

    bool Game::isGameOverMenuActive() const
    {
        return mGameOver && !mReturnToMenu && !mEnteringName;
    }

    bool Game::shouldReturnToMenu() const
    {
        return mReturnToMenu;
    }
}




















