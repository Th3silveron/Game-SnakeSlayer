#pragma once
#include <SFML/Audio.hpp>
#include <string>
#include <map>
#include "Constants.h"

namespace SnakeGame
{
    class AudioManager
    {
    public:
        AudioManager();

        // Sound effects
        void playSound(const std::string& name);
        void setVolume(const std::string& name, float volume);
        bool isSoundEnabled() const;
        void setSoundEnabled(bool enabled);

        // Metal!!!
        void setMusicVolume(float volume);
        void playMenuMusic();
        void playGameMusic();
        void stopMusic();
        void stopAllMusic();
        bool isMusicEnabled() const;
        void setMusicEnabled(bool enabled);

    private:
        std::map<std::string, sf::SoundBuffer> mBuffers;
        std::map<std::string, sf::Sound> mSounds;
        sf::Music mMenuMusic;
        sf::Music mGameMusic;
        bool mSoundEnabled;
        bool mMusicEnabled;
    };
}








