#include "OpenALSoundComponent.h"

OpenALSoundComponent::OpenALSoundComponent()
	: p_Pitch(1.f), p_Gain(1.f), p_Position{ 0,0,0 }, p_Velocity{ 0,0,0 }, p_LoopSound(false), p_Buffer(0)
{
	alGenSources(1, &p_Source);
	alSourcef(p_Source, AL_PITCH, p_Pitch);
	alSourcef(p_Source, AL_GAIN, p_Gain);
	alSource3f(p_Source, AL_POSITION, p_Position[0], p_Position[1], p_Position[2]);
	alSource3f(p_Source, AL_VELOCITY, p_Velocity[0], p_Velocity[1], p_Velocity[2]);
	alSourcei(p_Source, AL_LOOPING, p_LoopSound);
}

OpenALSoundComponent::~OpenALSoundComponent()
{
	//Stop();                         =>UNCOMMENT THESE TO SEE THE ERROR!
	//alDeleteSources(1, &p_Source);  =>UNCOMMENT THESE TO SEE THE ERROR!
}

void OpenALSoundComponent::Play(std::string name)
{
	auto& assetManager = MAIN_REGISTRY().GetAssetManager();
	ALuint buffer_to_play = assetManager.GetSoundEffect(name);
	if (buffer_to_play == 0)
	{
		LOG_WARN("Play sound effect is called but the buffer received is zero!");
	}
	if (buffer_to_play != p_Buffer)
	{
		Stop();
		p_Buffer = buffer_to_play;
		alSourcei(p_Source, AL_BUFFER, static_cast<ALint>(p_Buffer));
	}

	alSourcePlay(p_Source);
}

void OpenALSoundComponent::Stop()
{
	alSourceStop(p_Source);
}

void OpenALSoundComponent::Pause()
{
	alSourcePause(p_Source);
}

void OpenALSoundComponent::Resume()
{
	alSourcePlay(p_Source);
}

void OpenALSoundComponent::SetPitch(float pitch)
{
	p_Pitch = pitch;
	alSourcef(p_Source, AL_PITCH, p_Pitch);
}

void OpenALSoundComponent::SetGain(float gain)
{
	p_Gain = gain;
	alSourcef(p_Source, AL_GAIN, p_Gain);
}

void OpenALSoundComponent::SetPosition(float x, float y, float z)
{
	p_Position[0] = x;
	p_Position[1] = y;
	p_Position[2] = z;
	alSource3f(p_Source, AL_POSITION, x, y, z);
}

void OpenALSoundComponent::SetVelocity(float x, float y, float z)
{
	p_Velocity[0] = x;
	p_Velocity[1] = y;
	p_Velocity[2] = z;
	alSource3f(p_Source, AL_VELOCITY, x, y, z);
}

void OpenALSoundComponent::SetLooping(bool loop)
{
	p_LoopSound = loop;
	alSourcei(p_Source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
}

bool OpenALSoundComponent::IsPlaying() const
{
	ALint state;
	alGetSourcei(p_Source, AL_SOURCE_STATE, &state);
	return (state == AL_PLAYING);
}

bool OpenALSoundComponent::IsPaused() const
{
	ALint state;
	alGetSourcei(p_Source, AL_SOURCE_STATE, &state);
	return (state == AL_PAUSED);
}

bool OpenALSoundComponent::IsStopped() const
{
	ALint state;
	alGetSourcei(p_Source, AL_SOURCE_STATE, &state);
	return (state == AL_STOPPED);
}

void OpenALSoundComponent::CreateLuaOpenALSoundComponentBind(sol::state& lua)
{
	lua.new_usertype<OpenALSoundComponent>(
		"OpenALSoundComponent",
		"type_id", &entt::type_hash<OpenALSoundComponent>::value,
		sol::call_constructor,
		sol::factories(
			[&]() {
				return OpenALSoundComponent();
			}
		),

		// Playback control
		"Play", [](OpenALSoundComponent& self, const std::string& name) {
			self.Play(name);
		},
		"Stop", &OpenALSoundComponent::Stop,
		"Pause", &OpenALSoundComponent::Pause,
		"Resume", &OpenALSoundComponent::Resume,

		// Property setters
		"SetPitch", &OpenALSoundComponent::SetPitch,
		"SetGain", &OpenALSoundComponent::SetGain,
		"SetPosition", &OpenALSoundComponent::SetPosition,
		"SetVelocity", &OpenALSoundComponent::SetVelocity,
		"SetLooping", &OpenALSoundComponent::SetLooping,

		// State queries
		"IsPlaying", &OpenALSoundComponent::IsPlaying,
		"IsPaused", &OpenALSoundComponent::IsPaused,
		"IsStopped", &OpenALSoundComponent::IsStopped
	);
}
