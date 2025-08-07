#include "Apple.h"
#include <cstdlib>
#include <ctime>

namespace SnakeGame
{
    // Initialize apple texture and generate starting positions
    Apple::Apple()
    {
        if (!mTexture.loadFromFile("C:\\Source\\HW\\Apple-game-master\\ApplesGame\\Resources\\devil.png"))
        {

        }
        mSprite.setTexture(mTexture);
        mSprite.setScale(
            static_cast<float>(CELL_SIZE) / mTexture.getSize().x,
            static_cast<float>(CELL_SIZE) / mTexture.getSize().y
        );

        std::srand(static_cast<unsigned int>(std::time(nullptr)));

        generateNewPositions();
    }

    // Store current snake segments for collision detection
    void Apple::setSnakeSegments(const std::deque<sf::Vector2i>& segments)
    {
        mSnakeSegments = segments;
    }

    // Check if snake head collides with any apple
    bool Apple::update()
    {
        // Check if snake ate an apple
        const sf::Vector2i& headPos = mSnakeSegments.front();

        for (size_t i = 0; i < mPositions.size(); ++i)
        {
            if (mPositions[i] == headPos)
            {
                
                do
                {
                    mPositions[i].x = std::rand() % (GRID_WIDTH - 2) + 1;
                    mPositions[i].y = std::rand() % (GRID_HEIGHT - 2) + 1;
                } while (!isPositionValid(mPositions[i]));

                return true;
            }
        }

        return false;
    }

    // Render all apples at their current positions
    void Apple::draw(sf::RenderWindow& window)
    {
        for (const auto& pos : mPositions)
        {
            mSprite.setPosition(pos.x * CELL_SIZE, pos.y * CELL_SIZE);
            window.draw(mSprite);
        }
    }

    // Generate new random positions for all apples
    void Apple::generateNewPositions()
    {
        mPositions.clear();
        int attempts;

        for (int i = 0; i < MAX_APPLES; ++i)
        {
            attempts = 0;
            sf::Vector2i newPos;
            do
            {
                newPos.x = std::rand() % (GRID_WIDTH - 2) + 1;
                newPos.y = std::rand() % (GRID_HEIGHT - 2) + 1;
                attempts++;
            } while (!isPositionValid(newPos) && attempts < 100);

            mPositions.push_back(newPos);
        }
    }

    // Check if position is valid (not on snake or borders)
    bool Apple::isPositionValid(const sf::Vector2i& pos) const
    {
        for (const auto& segment : mSnakeSegments)
        {
            if (segment == pos)
                return false;
        }

        if (pos.x <= 0 || pos.x >= GRID_WIDTH - 1) return false;
        if (pos.y <= 0 || pos.y >= GRID_HEIGHT - 1) return false;

        return true;
    }
}





