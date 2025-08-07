#include "Player.h"
#include <iostream>

namespace SnakeGame
{
    Player::Player()
        : mCurrentDirection(Direction::Right)
    {
        loadTextures();
        mSegments = { {5, 10}, {4, 10}, {3, 10} };
    }

    // Load all snake segment textures from files
    void Player::loadTextures()
    {
        const std::vector<std::string> textureNames = {
            "head_up", "head_down", "head_left", "head_right",
            "tail_up", "tail_down", "tail_left", "tail_right",
            "body_horizontal", "body_vertical",
            "body_topleft", "body_topright",
            "body_bottomleft", "body_bottomright"
        };

        for (const auto& name : textureNames)
        {
            sf::Texture tex;
            if (!tex.loadFromFile("Resources/" + name + ".png"))
            {
                std::cerr << "download error: " << name << '\n';
            }
            mTextures[name] = tex;
            sf::Sprite sprite;
            sprite.setTexture(mTextures[name]);
            mSprites[name] = sprite;
        }
    }

    // Main update called each frame
    void Player::update(bool grow)
    {
        move(grow);
    }

    // Move snake segments based on current direction
    void Player::move(bool grow)
    {
        sf::Vector2i headPos = mSegments.front();

        switch (mCurrentDirection)
        {
        case Direction::Up:    headPos.y--; break;
        case Direction::Down:  headPos.y++; break;
        case Direction::Left:  headPos.x--; break;
        case Direction::Right: headPos.x++; break;
        }

        mSegments.push_front(headPos);
        if (!grow)
        {
            mSegments.pop_back();
        }
    }

    // Change movement direction with input validation
    void Player::setDirection(Direction dir)
    {
        if ((mCurrentDirection == Direction::Up && dir != Direction::Down) ||
            (mCurrentDirection == Direction::Down && dir != Direction::Up) ||
            (mCurrentDirection == Direction::Left && dir != Direction::Right) ||
            (mCurrentDirection == Direction::Right && dir != Direction::Left))
        {
            mCurrentDirection = dir;
        }
    }

    // Render complete snake with appropriate segment textures
    void Player::draw(sf::RenderWindow& window)
    {
        if (mSegments.empty()) return;

        const auto& head = mSegments[0];
        std::string headName;
        switch (mCurrentDirection)
        {
        case Direction::Up:    headName = "head_up"; break;
        case Direction::Down:  headName = "head_down"; break;
        case Direction::Left:  headName = "head_left"; break;
        case Direction::Right: headName = "head_right"; break;
        }

        mSprites[headName].setPosition(head.x * CELL_SIZE, head.y * CELL_SIZE);
        window.draw(mSprites[headName]);

        for (size_t i = 1; i < mSegments.size() - 1; ++i)
        {
            const auto& prev = mSegments[i - 1];
            const auto& curr = mSegments[i];
            const auto& next = mSegments[i + 1];
            std::string bodyTex = getBodyTextureName(prev, curr, next);

            mSprites[bodyTex].setPosition(curr.x * CELL_SIZE, curr.y * CELL_SIZE);
            window.draw(mSprites[bodyTex]);
        }

        const auto& tail = mSegments.back();
        const auto& beforeTail = mSegments[mSegments.size() - 2];
        std::string tailTex = getTailTextureName(beforeTail, tail);

        mSprites[tailTex].setPosition(tail.x * CELL_SIZE, tail.y * CELL_SIZE);
        window.draw(mSprites[tailTex]);
    }

    sf::Vector2i Player::getDirectionOffset(const sf::Vector2i& from, const sf::Vector2i& to) const
    {
        return { to.x - from.x, to.y - from.y };
    }

    // Determine correct body segment texture based on adjacent segments
    std::string Player::getBodyTextureName(const sf::Vector2i& prev, const sf::Vector2i& curr, const sf::Vector2i& next) const
    {
        auto dir1 = getDirectionOffset(curr, prev);
        auto dir2 = getDirectionOffset(curr, next);

        if (dir1.x == 0 && dir2.x == 0) return "body_vertical";
        if (dir1.y == 0 && dir2.y == 0) return "body_horizontal";

        if ((dir1 == sf::Vector2i{ -1, 0 } && dir2 == sf::Vector2i{ 0, -1 }) || (dir2 == sf::Vector2i{ -1, 0 } && dir1 == sf::Vector2i{ 0, -1 }))
            return "body_topleft";

        if ((dir1 == sf::Vector2i{ 1, 0 } && dir2 == sf::Vector2i{ 0, -1 }) || (dir2 == sf::Vector2i{ 1, 0 } && dir1 == sf::Vector2i{ 0, -1 }))
            return "body_topright";

        if ((dir1 == sf::Vector2i{ -1, 0 } && dir2 == sf::Vector2i{ 0, 1 }) || (dir2 == sf::Vector2i{ -1, 0 } && dir1 == sf::Vector2i{ 0, 1 }))
            return "body_bottomleft";

        if ((dir1 == sf::Vector2i{ 1, 0 } && dir2 == sf::Vector2i{ 0, 1 }) || (dir2 == sf::Vector2i{ 1, 0 } && dir1 == sf::Vector2i{ 0, 1 }))
            return "body_bottomright";

        return "body_horizontal";
    }

    // Determine correct tail texture based on adjacent segment
    std::string Player::getTailTextureName(const sf::Vector2i& beforeTail, const sf::Vector2i& tail) const
    {
        auto offset = getDirectionOffset(tail, beforeTail);
        if (offset == sf::Vector2i{ 0, -1 }) return "tail_down";
        if (offset == sf::Vector2i{ 0, 1 })  return "tail_up";
        if (offset == sf::Vector2i{ -1, 0 }) return "tail_right";
        if (offset == sf::Vector2i{ 1, 0 })  return "tail_left";

        return "tail_right";
    }

    // Collision detection with game boundariess
    bool Player::isCollidingWithWall() const
    {
        const auto& head = mSegments.front();
        return (head.x <= 0 || head.x >= GRID_WIDTH - 1 || head.y <= 0 || head.y >= GRID_HEIGHT - 1);
    }

    // Collision detection with snake's own body
    bool Player::isCollidingWithSelf() const
    {
        const auto& head = mSegments.front();
        for (size_t i = 1; i < mSegments.size(); ++i)
        {
            if (head == mSegments[i])
                return true;
        }
        return false;
    }

    const std::deque<sf::Vector2i>& Player::getSegments() const
    {
        return mSegments;
    }
}






