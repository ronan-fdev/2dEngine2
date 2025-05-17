#pragma once

#include "../ECS/Registry.h"
#include "../LOGGER/log.h"

#include "../MINIAUDIOSOUND/MINIAudioEngine.h"
#include "../ECS/COMPONENTS/MINIAudioSoundComponent.h"

class MINIAudioSoundSystem
{
public:
	MINIAudioSoundSystem(Registry& registry);
	~MINIAudioSoundSystem();

	void MINIAudioInitialize();

	void MINIAudioCleanUp();

	void MINIAudioUpdate(float dt);

private:
	Registry& m_Registry;
};

