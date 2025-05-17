#pragma once

#include <sol/sol.hpp>
#include <entt/entt.hpp>
#include <AL/al.h>
#include <string>

#include "../MainRegistry.h"
#include "../../LOGGER/log.h"

class OpenALSoundComponent
{
public:
	OpenALSoundComponent();
	~OpenALSoundComponent();

    void Play(std::string name);
    void Stop();
    void Pause();
    void Resume();

    void SetPitch(float pitch);
    void SetGain(float gain);
    void SetPosition(float x, float y, float z);
    void SetVelocity(float x, float y, float z);
    void SetLooping(bool loop);

    bool IsPlaying() const;
    bool IsPaused() const;
    bool IsStopped() const;

    static void CreateLuaOpenALSoundComponentBind(sol::state& lua);

private:
    ALuint p_Source;
    float p_Pitch;
    float p_Gain;
    float p_Position[3];
    float p_Velocity[3];
    bool p_LoopSound;
    ALuint p_Buffer;
};

