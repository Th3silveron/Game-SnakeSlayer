#include "AudioManager.h"
#include <iostream>

namespace SnakeGame
{
    // Initialize audio system and load all sound assets
    AudioManager::AudioManager() : mSoundEnabled(true), mMusicEnabled(true)
    {
        const std::vector<std::string> soundNames = {
            "Apple_eaten",
            "Game_over",
            "Menu_select",
            "Menu_confirm",
            "Menu_move"
        };

        // Load all sound effects
        for (const auto& name : soundNames)
        {
            sf::SoundBuffer buffer;
            if (!buffer.loadFromFile("Resources/" + name + ".wav"))
            {
                std::cerr << "Failed to load sound: " << name << "\n";
            }
            else
            {
                mBuffers[name] = buffer;
                mSounds[name].setBuffer(mBuffers[name]);
            }
        }

        // Load and configure menu music
        if (!mMenuMusic.openFromFile("Resources/Menu_sound.wav"))
        {
            std::cerr << "Failed to load menu music\n";
        }
        else
        {
            mMenuMusic.setLoop(true);
            mMenuMusic.setVolume(20.f);
        }

        // Load and configure game music
        if (!mGameMusic.openFromFile("Resources/Game_ready.wav"))
        {
            std::cerr << "Failed to load game music\n";
        }
        else
        {
            mGameMusic.setLoop(true);
            mGameMusic.setVolume(20.f);
        }

        // Set default volumes for sound effects
        setVolume("Apple_eaten", 160.f);
        setVolume("Game_over", 100.f);
        setVolume("Menu_select", 100.f);
        setVolume("Menu_confirm", 100.f);
        setVolume("Menu_move", 100.f);
    }

    // Play specified sound effect if enabled
    void AudioManager::playSound(const std::string& name)
    {
        if (!mSoundEnabled) return;

        auto it = mSounds.find(name);
        if (it != mSounds.end())
        {
            it->second.play();
        }
    }

    // Set volume for specific sound effect
    void AudioManager::setVolume(const std::string& name, float volume)
    {
        auto it = mSounds.find(name);
        if (it != mSounds.end())
        {
            it->second.setVolume(volume);
        }
    }

    // Set volume for all music tracks
    void AudioManager::setMusicVolume(float volume)
    {
        mGameMusic.setVolume(volume);
        mMenuMusic.setVolume(volume);
    }

    // Stop currently playing game music
    void AudioManager::stopMusic()
    {
        mGameMusic.stop();
    }

    // Play menu music if enabled
    void AudioManager::playMenuMusic()
    {
        if (!mMusicEnabled) return;
        mMenuMusic.play();
    }

    // Play game music if enabled
    void AudioManager::playGameMusic()
    {
        if (!mMusicEnabled) return;
        mGameMusic.play();
    }

    // Stop all music playback
    void AudioManager::stopAllMusic()
    {
        mGameMusic.stop();
        mMenuMusic.stop();
    }

    // Toggle sound effects on/off
    void AudioManager::setSoundEnabled(bool enabled)
    {
        mSoundEnabled = enabled;
    }

    // Toggle music on/off with proper state handling
    void AudioManager::setMusicEnabled(bool enabled)
    {
        mMusicEnabled = enabled;
        if (!enabled)
        {
            stopAllMusic();
        }
        else
        {
            if (mMenuMusic.getStatus() == sf::Music::Stopped &&
                mGameMusic.getStatus() == sf::Music::Stopped)
            {
                playMenuMusic();
            }
        }
    }

    bool AudioManager::isSoundEnabled() const
    {
        return mSoundEnabled;
    }

    bool AudioManager::isMusicEnabled() const
    {
        return mMusicEnabled;
    }
}








