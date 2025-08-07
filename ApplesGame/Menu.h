#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "HighScoreTable.h"
#include "AudioManager.h"
#include "Constants.h"

namespace SnakeGame
{
    // Enum representing different menu states
    enum class MenuState
    {
        Main,
        Options,
        Difficulty,
        Leaderboard
    };

    class Menu
    {
    public:
        // Constructor taking window dimensions and audio manager reference
        Menu(int width, int height, AudioManager& audio);

        // Drawing and input handling 
        void draw(sf::RenderWindow& window);
        bool handleInput(sf::Keyboard::Key key);

        // Getters for menu state and selection
        int getSelectedItem() const;
        MenuState getState() const;

        // Update and leaderboard 
        void update();
        void refreshLeaderboard();

        // Difficulty level accessor
        DifficultyLevel getDifficulty() const { return mCurrentDifficulty; }

    private:
        // Menu items containers
        std::vector<sf::Text> mItems;
        std::vector<sf::Text> mLeaderboardItems;
        std::vector<sf::Text> mOptionsItems;
        std::vector<sf::Text> mDifficultyItems;

        // UI elements and state variables
        sf::Text mDifficultyText;
        int mSelectedIndex;
        sf::Font mFont;
        int mWidth;
        int mHeight;
        MenuState mState;

        // Background elements
        sf::Texture mPentaTexture;
        sf::Sprite mPentaSprite;

        // Animation variables
        float mAlphaValue;
        bool mIncreasingAlpha;
        sf::Clock mAnimationClock;

        // Game data and settings
        HighScoreTable mHighScoreTable;
        AudioManager& mAudio;
        SoundState mSoundState;
        MusicState mMusicState;
        DifficultyLevel mCurrentDifficulty;

        // Private helper 
        void updateVisuals(); 
        void setMenu(MenuState state); 
        void loadResources(); 
        void setupLeaderboard(); 
        void setupOptionsMenu();
        void setupDifficultyMenu();

        // Text generation helpers
        std::string getSoundStateText() const;
        std::string getMusicStateText() const;
        std::string getDifficultyText() const;

        // Sound effect 
        void playSelectSound(); 
        void playConfirmSound();
        void playMoveSound(); 
    };
}












