#pragma once
#include <memory>
#include <map>
#include <vector>
#include <SFML/Audio.hpp>

// SoundBufferManager should be used to obtain any SoundBuffer
// SoundBufferManager owns all sounds and allows others to use them as well.
// SoundBufferManager gets all sounds via config files in the form of Sound_<name>
class SoundBufferManager
{
    std::map<std::string, std::shared_ptr<sf::SoundBuffer>> m_soundBufferMap;
    std::vector<std::string> m_soundBufferNames;

    SoundBufferManager() {}
    bool LoadSoundBuffer(const std::string& bufferName);

public:
    const static std::string s_soundDirectory;

    void AddSoundBufferToLoad(std::string bufferName);
    void LoadSoundBuffers();
    static SoundBufferManager* Instance()
    {
        static SoundBufferManager* instance = new SoundBufferManager();
        return instance;
    }

    static const std::shared_ptr<sf::SoundBuffer> GetSoundBuffer(std::string bufferName);
};
