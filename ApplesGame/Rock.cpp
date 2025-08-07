#include "Rock.h"
#include <iostream>

namespace SnakeGame
{
    // Initialize rock texture and set up sprite scaling
    Rock::Rock()
    {
        if (!mTexture.loadFromFile("Resources/Rock.jpg"))
        {
            std::cerr << "Failed to load rock texture\n";
        }
        mSprite.setTexture(mTexture);
        mSprite.setScale(
            static_cast<float>(CELL_SIZE) / mTexture.getSize().x,
            static_cast<float>(CELL_SIZE) / mTexture.getSize().y
        );
    }

    // Draw rocks around the game borders
    void Rock::draw(sf::RenderWindow& window)
    {
        // Draw top and bottom borders
        for (int x = 0; x < GRID_WIDTH; ++x)
        {
            mSprite.setPosition(x * CELL_SIZE, 0);
            window.draw(mSprite);
            mSprite.setPosition(x * CELL_SIZE, (GRID_HEIGHT - 1) * CELL_SIZE);
            window.draw(mSprite);
        }

        // Draw left and right borders
        for (int y = 0; y < GRID_HEIGHT; ++y)
        {
            mSprite.setPosition(0, y * CELL_SIZE);
            window.draw(mSprite);
            mSprite.setPosition((GRID_WIDTH - 1) * CELL_SIZE, y * CELL_SIZE);
            window.draw(mSprite);
        }
    }

    // Draw single rock at specified grid position
    void Rock::drawAtPosition(sf::RenderWindow& window, int x, int y)
    {
        mSprite.setPosition(x * CELL_SIZE, y * CELL_SIZE);
        window.draw(mSprite);
    }
}





