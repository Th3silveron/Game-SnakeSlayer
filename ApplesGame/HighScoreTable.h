#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include "HighScoreEntry.h"

namespace SnakeGame
{
    class HighScoreTable
    {
    public:
        HighScoreTable();
        void addEntry(const HighScoreEntry& entry);
        const std::vector<HighScoreEntry>& getEntries() const;
        void loadFromFile();
        void saveToFile();

    private:
        std::vector<HighScoreEntry> mEntries;
        static const int MAX_ENTRIES = 5;
    };
}


