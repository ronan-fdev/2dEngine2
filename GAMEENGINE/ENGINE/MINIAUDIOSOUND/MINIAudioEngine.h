#pragma once

#include "miniaudio.h"   

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <mutex>

#include "../LOGGER/log.h"

class MINIAudioSound;
class MINIAudioMusic;

enum class AudioCategory {
    SFX,
    MUSIC,
    VOICE,
    AMBIENT
};

class MINIAudioEngine
{
public:
    static MINIAudioEngine& Instance() {
        static MINIAudioEngine instance;
        return instance;
    }

    bool Initialize();
    void Shutdown();

    // Sound management
    std::shared_ptr<MINIAudioSound> LoadSound(const std::string& filePath);
    std::shared_ptr<MINIAudioMusic> LoadMusic(const std::string& filePath);

    // Global volume control
    void SetMasterVolume(float volume);
    float GetMasterVolume() const;

    // Category volume control
    void SetCategoryVolume(AudioCategory category, float volume);
    float GetCategoryVolume(AudioCategory category) const;

    // Mute/unmute
    void MuteAll(bool mute);
    void MuteCategory(AudioCategory category, bool mute);
    bool IsCategoryMuted(AudioCategory category) const;

    // Pause/resume all audio
    void PauseAll();
    void ResumeAll();

    // Stop all sounds
    void StopAll();

    // Device enumeration
    std::vector<std::string> GetAudioDevices() const;
    bool SetAudioDevice(const std::string& deviceName);
    std::string GetCurrentDevice() const;

    // Internal use (called by Sound/Music classes)
    ma_engine* GetEngine() { return &engine; }
    void RegisterSound(MINIAudioSound* sound);
    void UnregisterSound(MINIAudioSound* sound);
    void RegisterMusic(MINIAudioMusic* music);
    void UnregisterMusic(MINIAudioMusic* music);

    // 3D Audio settings
    void SetListenerPosition(float x, float y, float z = 0.0f);
    void SetListenerDirection(float x, float y, float z = 0.0f);
    void SetListenerVelocity(float x, float y, float z = 0.0f);
    void SetListenerWorldUp(float x, float y, float z = 1.0f);

    // Update method to be called once per frame
    void Update(float deltaTime);

private:
    MINIAudioEngine();
    ~MINIAudioEngine();

    // Prevent copying
    MINIAudioEngine(const MINIAudioEngine&) = delete;
    MINIAudioEngine& operator=(const MINIAudioEngine&) = delete;

    ma_engine engine;
    ma_device_info* pPlaybackDeviceInfos;
    ma_uint32 playbackDeviceCount;
    ma_context context;

    float masterVolume;
    std::unordered_map<AudioCategory, float> categoryVolumes;
    std::unordered_map<AudioCategory, bool> categoryMuted;

    std::vector<MINIAudioSound*> activeSounds;
    std::vector<MINIAudioMusic*> activeMusic;

    std::string currentDevice;
    bool initialized;

    std::mutex soundMutex;
    std::mutex musicMutex;
};

