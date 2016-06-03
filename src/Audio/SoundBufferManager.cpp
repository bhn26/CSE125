#include "SoundBufferManager.h"

#include "ConfigManager.h"

#include <iostream>

////////////////////////////////////////////////////////////////////////////////
const std::string SoundBufferManager::soundDirectory = "assets/audio/";

////////////////////////////////////////////////////////////////////////////////
void SoundBufferManager::LoadSoundBuffers()
{
    printf("\n=== Loading Sounds ===\n");
    for (std::string& soundName : _soundBufferNames)
    {
        LoadSoundBuffer(soundName);
    }
    _soundBufferNames.clear();
}

////////////////////////////////////////////////////////////////////////////////
bool SoundBufferManager::LoadSoundBuffer(const std::string& soundName)
{
    printf("Attempting to load %s...\t", soundName.c_str());
    // Don't duplicate load
    if (_soundBufferMap.find(soundName) != _soundBufferMap.end())
    {
        printf("Duplicate found!\n");
        return false;
    }

    const static std::string soundPrefix = std::string("Sound_");
    std::string soundPath = ConfigManager::instance()->GetConfigValue(soundPrefix + soundName);
    if (!soundPath.length())   // Make sure we get the soundbuffer paths
    {
        printf("Error: No sound in config files!\n");
        return false;
    }

    std::shared_ptr<sf::SoundBuffer> buffer = std::shared_ptr<sf::SoundBuffer>(new sf::SoundBuffer());
    if (!buffer->loadFromFile(soundDirectory + soundPath))
    {
        printf("Error: could not load!\n");
        return false;
    }

    _soundBufferMap[soundName] = std::move(buffer);
    printf("Done!\n");
    return true;
}

////////////////////////////////////////////////////////////////////////////////
void SoundBufferManager::AddSoundBufferToLoad(std::string soundName)
{
    _soundBufferNames.push_back(soundName);
}

////////////////////////////////////////////////////////////////////////////////
const std::shared_ptr<sf::SoundBuffer> SoundBufferManager::GetSoundBuffer(std::string soundName)
{
    const SoundBufferManager* manager = Instance();
    std::map<std::string, std::shared_ptr<sf::SoundBuffer>>::const_iterator it = manager->_soundBufferMap.find(soundName);
    // If in map
    if (it != manager->_soundBufferMap.end())    // Get sound mapped to this string
    {
        return it->second;
    }
    else    // Print error message
    {
        std::cout << "Sound not found in map\n";
        return std::shared_ptr<sf::SoundBuffer>(nullptr);
    }
}