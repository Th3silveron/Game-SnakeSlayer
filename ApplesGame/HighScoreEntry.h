#pragma once
#include <string>

namespace SnakeGame
{
    struct HighScoreEntry
    {
        std::string name = "";
        int score = 0;

        bool operator<(const HighScoreEntry& other) const
        {
            return score > other.score;
        }
    };
}



