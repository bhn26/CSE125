#include "GameData.h"
#include <cstdio>

////////////////////////////////////////////////////////////////////////////////
void GameInfo::Serialize(std::uint8_t* data)
{
    memcpy(data, this, sizeof(GameInfo));
}

void GameInfo::Deserialize(std::uint8_t* data)
{
    memcpy(this, data, sizeof(GameInfo));
}

////////////////////////////////////////////////////////////////////////////////
void PosInfo::Serialize(std::uint8_t* data)
{
    memcpy(data, this, sizeof(PosInfo));
}

void PosInfo::Deserialize(std::uint8_t* data)
{
    memcpy(this, data, sizeof(PosInfo));
}

////////////////////////////////////////////////////////////////////////////////
void RemInfo::Serialize(std::uint8_t* data)
{
    memcpy(data, this, sizeof(RemInfo));
}

void RemInfo::Deserialize(std::uint8_t* data)
{
    memcpy(this, data, sizeof(RemInfo));
}

////////////////////////////////////////////////////////////////////////////////
void ScoreInfo::Serialize(std::uint8_t* data)
{
    memcpy(data, this, sizeof(ScoreInfo));
}

void ScoreInfo::Deserialize(std::uint8_t* data)
{
    memcpy(this, data, sizeof(ScoreInfo));
}

////////////////////////////////////////////////////////////////////////////////
void EmoteInfo::Serialize(std::uint8_t* data)
{
    memcpy(data, this, sizeof(EmoteInfo));
}

void EmoteInfo::Deserialize(std::uint8_t* data)
{
    memcpy(this, data, sizeof(EmoteInfo));
}

////////////////////////////////////////////////////////////////////////////////
void MiscInfo::Serialize(std::uint8_t* data)
{
    memcpy(data, this, sizeof(MiscInfo));
}

void MiscInfo::Deserialize(std::uint8_t* data)
{
    memcpy(this, data, sizeof(MiscInfo));
}

////////////////////////////////////////////////////////////////////////////////
void NameInfo::Serialize(std::uint8_t* data)
{
    memcpy(data, this, sizeof(NameInfo));
}

void NameInfo::Deserialize(std::uint8_t* data)
{
    memcpy(this, data, sizeof(NameInfo));
}