#pragma once

namespace SnakeGame
{
    constexpr int GRID_WIDTH = 17;
    constexpr int GRID_HEIGHT = 15;
    constexpr int HUD_WIDTH = 4;

    constexpr int FIELD_TEXTURE_WIDTH = 800;
    constexpr int FIELD_TEXTURE_HEIGHT = 600;

    constexpr int CELL_SIZE_X = FIELD_TEXTURE_WIDTH / GRID_WIDTH;
    constexpr int CELL_SIZE_Y = FIELD_TEXTURE_HEIGHT / GRID_HEIGHT;

    constexpr int CELL_SIZE = (CELL_SIZE_X < CELL_SIZE_Y) ? CELL_SIZE_X : CELL_SIZE_Y;

    constexpr float UPDATE_INTERVAL = 0.2f;

    enum class Direction
    {
        Up, Down, Left, Right
    };

    enum class SoundState
    {
        On,
        Off
    };

    enum class MusicState
    {
        On,
        Off
    };

    enum class DifficultyLevel
    {
        Easy,
        Normal,
        Hell
    };
}






