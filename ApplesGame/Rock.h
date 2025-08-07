#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"

namespace SnakeGame
{
    class Rock
    {
    public:
        Rock();
        void draw(sf::RenderWindow& window);
        void drawAtPosition(sf::RenderWindow& window, int x, int y);

    private:
        sf::Texture mTexture;
        sf::Sprite mSprite;
    };
}




