#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include "MINIAudioEngine.h"
#include "MINIAudioSound.h"
#include "MINIAudioMusic.h"

MINIAudioEngine::MINIAudioEngine()
    : pPlaybackDeviceInfos(nullptr)
    , playbackDeviceCount(0)
    , masterVolume(1.0f)
    , initialized(false)
{
    // Initialize default category volumes
    categoryVolumes[AudioCategory::SFX] = 1.0f;
    categoryVolumes[AudioCategory::MUSIC] = 1.0f;
    categoryVolumes[AudioCategory::VOICE] = 1.0f;
    categoryVolumes[AudioCategory::AMBIENT] = 1.0f;

    // Initialize mute states
    categoryMuted[AudioCategory::SFX] = false;
    categoryMuted[AudioCategory::MUSIC] = false;
    categoryMuted[AudioCategory::VOICE] = false;
    categoryMuted[AudioCategory::AMBIENT] = false;
}

MINIAudioEngine::~MINIAudioEngine() {
    Shutdown();
}

bool MINIAudioEngine::Initialize() {
    if (initialized) {
        return true;
    }

    // Initialize miniaudio context for device enumeration
    ma_result result = ma_context_init(nullptr, 0, nullptr, &context);
    if (result != MA_SUCCESS) {
        LOG_ERROR("Failed to setup MINIAudioEngine!");
        return false;
    }

    // Enumerate playback devices
    result = ma_context_get_devices(&context, &pPlaybackDeviceInfos, &playbackDeviceCount, nullptr, nullptr);
    if (result != MA_SUCCESS) {
        LOG_ERROR("Failed to setup MINIAudioEngine!");
        ma_context_uninit(&context);
        return false;
    }

    // Set the default device
    if (playbackDeviceCount > 0) {
        currentDevice = pPlaybackDeviceInfos[0].name;
    }

    // Initialize the engine
    ma_engine_config engineConfig = ma_engine_config_init();
    engineConfig.listenerCount = 1;

    result = ma_engine_init(&engineConfig, &engine);
    if (result != MA_SUCCESS) {
        ma_context_uninit(&context);
        LOG_ERROR("Failed to setup MINIAudioEngine!");
        return false;
    }

    LOG_INFO("MINIAudioEngine Initialization Completed!");

    initialized = true;
    return true;
}

void MINIAudioEngine::Shutdown() {
    if (!initialized) {
        return;
    }

    // Stop all MINIAudioSounds
    StopAll();

    // Uninitialize the engine
    ma_engine_uninit(&engine);
    ma_context_uninit(&context);

    initialized = false;
}

std::shared_ptr<MINIAudioSound> MINIAudioEngine::LoadSound(const std::string& filePath) {
    if (!initialized) {
        return nullptr;
    }

    std::shared_ptr<MINIAudioSound> MINIAudioSoundInitializer = std::make_shared<MINIAudioSound>(filePath);
    if (MINIAudioSoundInitializer->IsLoaded()) {
        return MINIAudioSoundInitializer;
    }

    return nullptr;
}

std::shared_ptr<MINIAudioMusic> MINIAudioEngine::LoadMusic(const std::string& filePath) {
    if (!initialized) {
        LOG_ERROR("MINIAudioEngine is not Initialized!.");
        return nullptr;
    }

    std::shared_ptr<MINIAudioMusic> MINIAudioMusicInitiliazer = std::make_shared<MINIAudioMusic>(filePath);
    if (MINIAudioMusicInitiliazer->IsLoaded()) {
        return MINIAudioMusicInitiliazer;
    }

    return nullptr;
}

void MINIAudioEngine::SetMasterVolume(float volume) {
    masterVolume = std::max(0.0f, std::min(volume, 1.0f));
    ma_engine_set_volume(&engine, masterVolume);
}

float MINIAudioEngine::GetMasterVolume() const {
    return masterVolume;
}

void MINIAudioEngine::SetCategoryVolume(AudioCategory category, float volume) {
    categoryVolumes[category] = std::max(0.0f, std::min(volume, 1.0f));

    // Apply volume to all MINIAudioSounds in this category
    std::lock_guard<std::mutex> lock(soundMutex);
    for (auto MINIAudioSound : activeSounds) {
        if (MINIAudioSound->GetCategory() == category) {
            MINIAudioSound->UpdateVolume();
        }
    }

    std::lock_guard<std::mutex> lockMINIAudioMusic(musicMutex);
    for (auto MINIAudioMusic : activeMusic) {
        if (MINIAudioMusic->GetCategory() == category) {
            MINIAudioMusic->UpdateVolume();
        }
    }
}

float MINIAudioEngine::GetCategoryVolume(AudioCategory category) const {
    auto it = categoryVolumes.find(category);
    if (it != categoryVolumes.end()) {
        return it->second;
    }
    return 1.0f;
}

void MINIAudioEngine::MuteAll(bool mute) {
    for (auto& pair : categoryMuted) {
        pair.second = mute;
    }

    // Update all MINIAudioSounds and MINIAudioMusic
    std::lock_guard<std::mutex> lock(soundMutex);
    for (auto MINIAudioSound : activeSounds) {
        MINIAudioSound->UpdateVolume();
    }

    std::lock_guard<std::mutex> lockMINIAudioMusic(musicMutex);
    for (auto MINIAudioMusic : activeMusic) {
        MINIAudioMusic->UpdateVolume();
    }
}

void MINIAudioEngine::MuteCategory(AudioCategory category, bool mute) {
    categoryMuted[category] = mute;

    // Apply mute to all MINIAudioSounds in this category
    std::lock_guard<std::mutex> lock(soundMutex);
    for (auto MINIAudioSound : activeSounds) {
        if (MINIAudioSound->GetCategory() == category) {
            MINIAudioSound->UpdateVolume();
        }
    }

    std::lock_guard<std::mutex> lockMINIAudioMusic(musicMutex);
    for (auto MINIAudioMusic : activeMusic) {
        if (MINIAudioMusic->GetCategory() == category) {
            MINIAudioMusic->UpdateVolume();
        }
    }
}

bool MINIAudioEngine::IsCategoryMuted(AudioCategory category) const {
    auto it = categoryMuted.find(category);
    if (it != categoryMuted.end()) {
        return it->second;
    }
    return false;
}

void MINIAudioEngine::PauseAll() {
    std::lock_guard<std::mutex> lock(soundMutex);
    for (auto MINIAudioSound : activeSounds) {
        MINIAudioSound->Pause();
    }

    std::lock_guard<std::mutex> lockMINIAudioMusic(musicMutex);
    for (auto MINIAudioMusic : activeMusic) {
        MINIAudioMusic->Pause();
    }
}

void MINIAudioEngine::ResumeAll() {
    std::lock_guard<std::mutex> lock(soundMutex);
    for (auto MINIAudioSound : activeSounds) {
        MINIAudioSound->Resume();
    }

    std::lock_guard<std::mutex> lockMINIAudioMusic(musicMutex);
    for (auto MINIAudioMusic : activeMusic) {
        MINIAudioMusic->Resume();
    }
}

void MINIAudioEngine::StopAll() {
    std::lock_guard<std::mutex> lock(soundMutex);
    for (auto MINIAudioSound : activeSounds) {
        MINIAudioSound->Stop();
    }

    std::lock_guard<std::mutex> lockMINIAudioMusic(musicMutex);
    for (auto MINIAudioMusic : activeMusic) {
        MINIAudioMusic->Stop();
    }
}

std::vector<std::string> MINIAudioEngine::GetAudioDevices() const {
    std::vector<std::string> devices;

    for (ma_uint32 i = 0; i < playbackDeviceCount; i++) {
        devices.push_back(pPlaybackDeviceInfos[i].name);
    }

    return devices;
}

bool MINIAudioEngine::SetAudioDevice(const std::string& deviceName) {
    if (!initialized) {
        return false;
    }

    // Find the device
    for (ma_uint32 i = 0; i < playbackDeviceCount; i++) {
        if (deviceName == pPlaybackDeviceInfos[i].name) {
            // Stop all current MINIAudioSounds
            StopAll();

            // Uninitialize the engine
            ma_engine_uninit(&engine);

            // Create a new engine with the selected device
            ma_engine_config engineConfig = ma_engine_config_init();
            engineConfig.pPlaybackDeviceID = &pPlaybackDeviceInfos[i].id;

            ma_result result = ma_engine_init(&engineConfig, &engine);
            if (result != MA_SUCCESS) {
                // Try to reinitialize with the default device
                engineConfig.pPlaybackDeviceID = nullptr;
                result = ma_engine_init(&engineConfig, &engine);
                if (result != MA_SUCCESS) {
                    return false;
                }
            }

            // Set the master volume
            ma_engine_set_volume(&engine, masterVolume);

            currentDevice = deviceName;
            return true;
        }
    }

    return false;
}

std::string MINIAudioEngine::GetCurrentDevice() const {
    return currentDevice;
}

void MINIAudioEngine::RegisterSound(MINIAudioSound* MINIAudioSound) {
    if (!MINIAudioSound) return;

    std::lock_guard<std::mutex> lock(soundMutex);
    activeSounds.push_back(MINIAudioSound);
}

void MINIAudioEngine::UnregisterSound(MINIAudioSound* MINIAudioSound) {
    if (!MINIAudioSound) return;

    std::lock_guard<std::mutex> lock(soundMutex);
    auto it = std::find(activeSounds.begin(), activeSounds.end(), MINIAudioSound);
    if (it != activeSounds.end()) {
        activeSounds.erase(it);
    }
}

void MINIAudioEngine::RegisterMusic(MINIAudioMusic* MINIAudioMusic) {
    if (!MINIAudioMusic) return;

    std::lock_guard<std::mutex> lock(musicMutex);
    activeMusic.push_back(MINIAudioMusic);
}

void MINIAudioEngine::UnregisterMusic(MINIAudioMusic* MINIAudioMusic) {
    if (!MINIAudioMusic) return;

    std::lock_guard<std::mutex> lock(musicMutex);
    auto it = std::find(activeMusic.begin(), activeMusic.end(), MINIAudioMusic);
    if (it != activeMusic.end()) {
        activeMusic.erase(it);
    }
}

void MINIAudioEngine::SetListenerPosition(float x, float y, float z) {
    ma_engine_listener_set_position(&engine, 0, x, y, z);
}

void MINIAudioEngine::SetListenerDirection(float x, float y, float z) {
    ma_engine_listener_set_direction(&engine, 0, x, y, z);
}

void MINIAudioEngine::SetListenerVelocity(float x, float y, float z) {
    ma_engine_listener_set_velocity(&engine, 0, x, y, z);
}

void MINIAudioEngine::SetListenerWorldUp(float x, float y, float z) {
    ma_engine_listener_set_world_up(&engine, 0, x, y, z);
}

void MINIAudioEngine::Update(float deltaTime) {
    if (!initialized)
    {
        return;
    }
    // Clean up any finished MINIAudioSounds
    std::lock_guard<std::mutex> lock(soundMutex);
    auto MINIAudioSoundIt = activeSounds.begin();
    while (MINIAudioSoundIt != activeSounds.end()) {
        if (!(*MINIAudioSoundIt)->IsPlaying()) {
            MINIAudioSoundIt = activeSounds.erase(MINIAudioSoundIt);
        }
        else {
            ++MINIAudioSoundIt;
        }
    }
}

