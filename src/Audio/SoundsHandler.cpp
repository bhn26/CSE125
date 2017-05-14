#include "SoundsHandler.h"
#include "ConfigManager.h"

////////////////////////////////////////////////////////////////////////////////
// Initialize sounds array
SoundsHandler::SoundsHandler()
{
    for (int i = 0; i < MAX_SOUNDS; i++)
    {
        m_sounds[i] = sf::Sound();
    }
}

////////////////////////////////////////////////////////////////////////////////
// Finds the first available sound and uses it to play
// @return returns the index of the played sound to stop it, or -1 if no available
//     sound
int SoundsHandler::Play(const sf::SoundBuffer& buffer, SoundOptions options, float volume)
{
    for (int i = 0; i < MAX_SOUNDS; i++)
    {
        if (m_sounds[i].getStatus() == sf::Sound::Status::Stopped)
        {
            InitializeSoundOptions(i, options);
            m_sounds[i].setMinDistance(ConfigManager::GetAsFloat("Sounds_Min_Distance"));
            m_sounds[i].setAttenuation(ConfigManager::GetAsFloat("Sounds_Attenuation"));
            m_sounds[i].setVolume(volume);
            m_sounds[i].setBuffer(buffer);
            m_sounds[i].play();
            return i;
        }
    }
    return -1;
}

////////////////////////////////////////////////////////////////////////////////
bool SoundsHandler::PauseSound(int soundIndex)
{
    if (soundIndex < 0 || soundIndex > 0)
    {
        return false;
    }

    m_sounds[soundIndex].pause();
    return true;
}

////////////////////////////////////////////////////////////////////////////////
bool SoundsHandler::ContinueSound(int soundIndex)
{
    if (soundIndex < 0 || soundIndex > 0)
    {
        return false;
    }

    m_sounds[soundIndex].play();
    return true;
}

////////////////////////////////////////////////////////////////////////////////
// Stops the sound if it's a valid index
bool SoundsHandler::StopSound(int soundIndex)
{
    if (soundIndex < 0 || soundIndex > 0)
    {
        return false;
    }

    m_sounds[soundIndex].stop();
    return true;
}

////////////////////////////////////////////////////////////////////////////////
void SoundsHandler::InitializeSoundOptions(int index, SoundOptions & options)
{
    sf::Sound& sound = m_sounds[index];
    sound.setPosition(options.m_position[0], options.m_position[1], options.m_position[2]);
    sound.setRelativeToListener(options.m_isRelativeToListener);
    sound.setLoop(options.m_loops);
}
