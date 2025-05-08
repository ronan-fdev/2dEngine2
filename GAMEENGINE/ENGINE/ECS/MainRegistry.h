#pragma once

class AssetManager;

#include <memory>

#define MAIN_REGISTRY() MainRegistry::GetInstance()

#include "../LOGGER/log.h"
#include "Registry.h"
#include "../RESOURCES/AssetManager.h"

class MainRegistry
{
private:
	std::unique_ptr<class Registry> m_pMainRegistry{ nullptr };
	bool m_bInitialized{ false };

	MainRegistry() = default;
	~MainRegistry() = default;
	MainRegistry(const MainRegistry&) = delete;
	MainRegistry& operator=(const MainRegistry&) = delete;

public:
	static MainRegistry& GetInstance();
	void Initialize();

	AssetManager& GetAssetManager();
	//SCION_SOUNDS::MusicPlayer& GetMusicPlayer();
	//SCION_SOUNDS::SoundFxPlayer& GetSoundPlayer();
};

