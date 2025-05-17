#include "MINIAudioSoundSystem.h"

MINIAudioSoundSystem::MINIAudioSoundSystem(Registry& registry)
	: m_Registry(registry)
{
}

MINIAudioSoundSystem::~MINIAudioSoundSystem()
{
	MINIAudioSoundComponent::StopAllMusic();
	MINIAudioSoundComponent::StopAllSounds();
	MINIAudioEngine::Instance().Shutdown();
}

void MINIAudioSoundSystem::MINIAudioInitialize()
{
	if (!MINIAudioEngine::Instance().Initialize())
	{
		LOG_ERROR("Failed to initialize the MINIAudio!");
	}
}

void MINIAudioSoundSystem::MINIAudioCleanUp()
{
	MINIAudioEngine::Instance().Shutdown();
}

void MINIAudioSoundSystem::MINIAudioUpdate(float dt)
{
	bool atLeastSingleElementPresentInView = false;
	
	auto view = m_Registry.GetRegistry().view<MINIAudioSoundComponent>();
	for (const auto& entity : view)
	{
		auto& miniAudioSoundComponent = view.get<MINIAudioSoundComponent>(entity);
		miniAudioSoundComponent.Update(dt);
		atLeastSingleElementPresentInView = true;
	}

	if (atLeastSingleElementPresentInView)
	{
		MINIAudioEngine::Instance().Update(dt);
	}
}
