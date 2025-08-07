#include "HighScoreTable.h"

namespace SnakeGame
{
    // Constructor loads existing highscores
    HighScoreTable::HighScoreTable()
    {
        loadFromFile();
    }

    // Adds new entry and maintains sorted list
    void HighScoreTable::addEntry(const HighScoreEntry& entry)
    {
        mEntries.push_back(entry);
        std::sort(mEntries.begin(), mEntries.end());

        // Keep only top scores
        if (mEntries.size() > MAX_ENTRIES)
        {
            mEntries.pop_back();
        }

        saveToFile(); // Persist changes
    }

    // Returns all entries
    const std::vector<HighScoreEntry>& HighScoreTable::getEntries() const
    {
        return mEntries;
    }

    // Loads highscores from file
    void HighScoreTable::loadFromFile()
    {
        mEntries.clear();
        std::ifstream file("highscores.txt");

        if (file.is_open())
        {
            HighScoreEntry entry;
            while (file >> entry.name >> entry.score)
            {
                mEntries.push_back(entry);
            }
            file.close();
        }

        // Ensure proper sorting and size
        std::sort(mEntries.begin(), mEntries.end());
        if (mEntries.size() > MAX_ENTRIES)
        {
            mEntries.resize(MAX_ENTRIES);
        }
    }

    // Saves highscores to file
    void HighScoreTable::saveToFile()
    {
        std::ofstream file("highscores.txt");

        if (file.is_open())
        {
            for (const auto& entry : mEntries)
            {
                file << entry.name << " " << entry.score << "\n";
            }
            file.close();
        }
    }
}



