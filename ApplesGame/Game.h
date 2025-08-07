#pragma once

#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Rock.h"
#include "Apple.h"
#include "Constants.h"
#include "AudioManager.h"
#include "GameOverMenu.h"
#include "HighScoreEntry.h"
#include "HighScoreTable.h"

namespace SnakeGame
{
    class Menu;

    // Main game class handling game state, rendering and logic
    class Game
    {
    public:
        Game(AudioManager& audio, Menu& menu);
        void update();
        void draw(sf::RenderWindow& window);
        void handleInput(sf::Keyboard::Key key);
        bool isGameOverMenuActive() const;
        bool shouldReturnToMenu() const;
        bool isShowingLeaderboard() const { return mShowingLeaderboard; }
        int getScore() const { return mScore; }

        // Difficulty management methods
        void setDifficulty(DifficultyLevel difficulty) { mDifficulty = difficulty; }
        DifficultyLevel getDifficulty() const { return mDifficulty; }
        float getUpdateInterval() const { return mUpdateInterval; }

    private:
        // Game entities and components
        Player mPlayer;
        Rock mRock;
        Apple mApple;
        bool mGameOver;
        bool mReturnToMenu;
        bool mEnteringName;
        bool mShowingLeaderboard;
        std::string mPlayerName;
        int mNameCharsEntered;
        int mScore;
        sf::Texture mFieldTexture;
        sf::Sprite mFieldSprite;
        sf::RectangleShape mHudArea;
        sf::Font mFont;
        sf::Text mGameOverText;
        sf::Text mScoreLabel;
        sf::Text mScoreValue;
        sf::Text mNameEntryText;
        sf::RectangleShape mGameOverOverlay;
        AudioManager& mAudio;
        GameOverMenu mGameOverMenu;
        HighScoreTable mHighScoreTable;
        Menu& mMenu;

        // Difficulty system members
        DifficultyLevel mDifficulty;
        std::vector<sf::Vector2i> mHellBarriers;
        float mUpdateInterval;
        int mApplesEaten;

        // Initialization and helper methods
        void loadResources();
        void setupGameOverText();
        void setupScoreText();
        void setupNameEntryText();
        void updateScoreDisplay();
        void handleNameInput(sf::Keyboard::Key key);
        void drawLeaderboard(sf::RenderWindow& window);

        // Hell difficulty barrier management
        void spawnHellBarrier();
        bool isValidBarrierPosition(const sf::Vector2i& pos) const;
    };
}



















