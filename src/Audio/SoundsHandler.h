#pragma once

#include <SFML/Audio.hpp>

class SoundsHandler
{
public:
    const static int MAX_SOUNDS = 5;
    struct SoundOptions
    {
        float _position[3];
        bool _loops;
        bool _isRelativeToListener;
        SoundOptions(float x = 0.0f, float y = 0.0f, float z = 0.0f) : _loops(false), _isRelativeToListener(true)
        {
            _position[0] = x;
            _position[1] = y;
            _position[2] = z;
        }
    };

    SoundsHandler();

    int Play(const sf::SoundBuffer& buffer, SoundOptions options = SoundOptions());
    bool PauseSound(int soundIndex);
    bool ContinueSound(int soundIndex);
    bool StopSound(int soundIndex);

private:
    sf::Sound m_sounds[MAX_SOUNDS];

    void InitializeSoundOptions(int index, SoundOptions& options);
};
