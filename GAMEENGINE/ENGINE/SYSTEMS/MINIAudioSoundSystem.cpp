#include "MINIAudioSoundSystem.h"

MINIAudioSoundSystem::MINIAudioSoundSystem(Registry& registry)
	: m_Registry(registry)
{
	MINIAudioInitialize();
}

MINIAudioSoundSystem::~MINIAudioSoundSystem()
{
	MINIAudioEngine::Instance().Shutdown();
}

void MINIAudioSoundSystem::MINIAudioInitialize()
{
	if (!MINIAudioEngine::Instance().Initialize())
	{
		LOG_ERROR("Failed to initialize the MINIAudio!");
	}
}

void MINIAudioSoundSystem::MINIAudioUpdate()
{
	MINIAudioEngine::Instance().Update(0.016);
	auto view = m_Registry.GetRegistry().view<MINIAudioSoundComponent>();
	for (const auto& entity : view)
	{
		auto& miniAudioSoundComponent = view.get<MINIAudioSoundComponent>(entity);
		miniAudioSoundComponent.Update(0.016);
	}
}
