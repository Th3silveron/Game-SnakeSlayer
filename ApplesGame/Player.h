#pragma once
#include <SFML/Graphics.hpp>
#include <deque>
#include <map>
#include "Constants.h"

namespace SnakeGame
{
    class Player
    {
    public:
        Player();
        void update(bool grow);
        void draw(sf::RenderWindow& window);
        void setDirection(Direction dir);
        bool isCollidingWithWall() const;
        bool isCollidingWithSelf() const; 
        const std::deque<sf::Vector2i>& getSegments() const;

    private:
        std::deque<sf::Vector2i> mSegments;
        Direction mCurrentDirection;

        std::map<std::string, sf::Texture> mTextures;
        std::map<std::string, sf::Sprite> mSprites;

        void loadTextures();
        void move(bool grow);
        sf::Vector2i getDirectionOffset(const sf::Vector2i& from, const sf::Vector2i& to) const;
        std::string getBodyTextureName(const sf::Vector2i& prev, const sf::Vector2i& curr, const sf::Vector2i& next) const;
        std::string getTailTextureName(const sf::Vector2i& beforeTail, const sf::Vector2i& tail) const;
    };
}








