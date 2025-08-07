#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <deque>
#include "Constants.h"

namespace SnakeGame
{
    class Apple
    {
    public:
        Apple();
        bool update();
        void draw(sf::RenderWindow& window);
        void setSnakeSegments(const std::deque<sf::Vector2i>& segments);

        // Добавляем метод для получения позиций яблок
        const std::vector<sf::Vector2i>& getPositions() const { return mPositions; }

    private:
        static constexpr int MAX_APPLES = 8;
        sf::Texture mTexture;
        sf::Sprite mSprite;
        std::vector<sf::Vector2i> mPositions;
        std::deque<sf::Vector2i> mSnakeSegments;

        void generateNewPositions();
        bool isPositionValid(const sf::Vector2i& pos) const;
    };
}






