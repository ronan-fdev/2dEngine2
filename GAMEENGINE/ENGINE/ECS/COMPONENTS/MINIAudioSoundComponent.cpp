#include "MINIAudioSoundComponent.h"
#include "../../MINIAUDIOSOUND/MINIAudioEngine.h"
#include <algorithm>
#include <random>

std::unordered_map<std::string, std::shared_ptr<MINIAudioSound>> MINIAudioSoundComponent::sounds;
std::unordered_map<std::string, std::shared_ptr<MINIAudioMusic>> MINIAudioSoundComponent::music;

MINIAudioSoundComponent::MINIAudioSoundComponent()
    : posX(0.0f)
    , posY(0.0f)
    , posZ(0.0f)
    , velX(0.0f)
    , velY(0.0f)
    , velZ(0.0f)
    , currentSequenceIndex(0)
    , sequenceTimer(0.0f)
    , playingSequence(false)
{
}

MINIAudioSoundComponent::~MINIAudioSoundComponent() {
    //StopAllSounds();
    //StopAllMusic();
}

void MINIAudioSoundComponent::AddSound(const std::string& name, const std::string& filePath, AudioCategory category) {
    // Load the sound through the audio engine
    std::shared_ptr<MINIAudioSound> sound = MINIAudioEngine::Instance().LoadSound(filePath);
    if (sound) {
        sound->SetCategory(category);
        sounds[name] = sound;
    }
}

void MINIAudioSoundComponent::AddMusic(const std::string& name, const std::string& filePath, AudioCategory category) {
    // Load the music through the audio engine
    std::shared_ptr<MINIAudioMusic> musicTrack = MINIAudioEngine::Instance().LoadMusic(filePath);
    if (musicTrack) {
        musicTrack->SetCategory(category);
        music[name] = musicTrack;
    }
}

bool MINIAudioSoundComponent::PlaySound(const std::string& name, bool loop) {
    auto it = sounds.find(name);
    if (it != sounds.end()) {
        // Apply randomization if set
        ApplySoundRandomization(name, it->second);

        // Set position for spatial audio
        it->second->SetPosition(posX, posY, posZ);
        it->second->SetVelocity(velX, velY, velZ);

        // Set looping
        it->second->SetLooping(loop);

        // Play the sound
        return it->second->Play();
    }
    return false;
}

bool MINIAudioSoundComponent::PlayMusic(const std::string& name, bool loop) {
    auto it = music.find(name);
    if (it != music.end()) {
        LOG_INFO("ENTERED PLAY MUSIC");
        it->second->SetLooping(loop);
        return it->second->Play();
    }
    return false;
}

bool MINIAudioSoundComponent::PlayMusicWithFadeIn(const std::string& name, float fadeInDuration, bool loop) {
    auto it = music.find(name);
    if (it != music.end()) {
        it->second->SetLooping(loop);
        it->second->FadeIn(fadeInDuration);
        return true;
    }
    return false;
}

void MINIAudioSoundComponent::StopMusicWithFadeOut(const std::string& name, float fadeOutDuration) {
    auto it = music.find(name);
    if (it != music.end()) {
        it->second->FadeOut(fadeOutDuration, true);
    }
}

void MINIAudioSoundComponent::CrossFadeMusic(const std::string& oldMusic, const std::string& newMusic, float fadeDuration) {
    // Fade out the old music
    StopMusicWithFadeOut(oldMusic, fadeDuration);

    // Fade in the new music
    PlayMusicWithFadeIn(newMusic, fadeDuration, true);
}

void MINIAudioSoundComponent::StopSound(const std::string& name) {
    auto it = sounds.find(name);
    if (it != sounds.end()) {
        it->second->Stop();
    }
}

void MINIAudioSoundComponent::StopMusic(const std::string& name) {
    auto it = music.find(name);
    if (it != music.end()) {
        it->second->Stop();
    }
}

void MINIAudioSoundComponent::StopAllSounds() {
    for (auto& pair : sounds) {
        pair.second->Stop();
    }
}

void MINIAudioSoundComponent::StopAllMusic() {
    for (auto& pair : music) {
        pair.second->Stop();
    }
}

void MINIAudioSoundComponent::PauseSound(const std::string& name) {
    auto it = sounds.find(name);
    if (it != sounds.end()) {
        it->second->Pause();
    }
}

void MINIAudioSoundComponent::ResumeSound(const std::string& name) {
    auto it = sounds.find(name);
    if (it != sounds.end()) {
        it->second->Resume();
    }
}

void MINIAudioSoundComponent::PauseMusic(const std::string& name) {
    auto it = music.find(name);
    if (it != music.end()) {
        it->second->Pause();
    }
}

void MINIAudioSoundComponent::ResumeMusic(const std::string& name) {
    auto it = music.find(name);
    if (it != music.end()) {
        it->second->Resume();
    }
}

void MINIAudioSoundComponent::SetSoundVolume(const std::string& name, float volume) {
    auto it = sounds.find(name);
    if (it != sounds.end()) {
        it->second->SetVolume(volume);
    }
}

void MINIAudioSoundComponent::SetMusicVolume(const std::string& name, float volume) {
    auto it = music.find(name);
    if (it != music.end()) {
        it->second->SetVolume(volume);
    }
}

void MINIAudioSoundComponent::SetPosition(float x, float y, float z) {
    posX = x;
    posY = y;
    posZ = z;

    // Update position for all playing sounds
    for (auto& pair : sounds) {
        if (pair.second->IsPlaying()) {
            pair.second->SetPosition(x, y, z);
        }
    }
}

void MINIAudioSoundComponent::SetAttenuationRange(float min, float max)
{
    AttenuationRangeMin = min;
    AttenuationRangeMax = max;

    // Update position for all playing sounds
    for (auto& pair : sounds) {
        if (pair.second->IsPlaying()) {
            pair.second->SetAttenuationRange(min, max);
        }
    }
}

void MINIAudioSoundComponent::SetVelocity(float x, float y, float z) {
    velX = x;
    velY = y;
    velZ = z;

    // Update velocity for all playing sounds
    for (auto& pair : sounds) {
        if (pair.second->IsPlaying()) {
            pair.second->SetVelocity(x, y, z);
        }
    }
}

void MINIAudioSoundComponent::AddSoundTrigger(const std::string& soundName, SoundTriggerType triggerType,
    float parameter, const std::string& eventName) {
    // Make sure the sound exists
    if (sounds.find(soundName) == sounds.end()) {
        return;
    }

    SoundTrigger trigger;
    trigger.type = triggerType;
    trigger.parameter = parameter;
    trigger.eventName = eventName;
    trigger.accumulator = 0.0f;
    trigger.active = true;

    soundTriggers[soundName] = trigger;
}

void MINIAudioSoundComponent::RemoveSoundTrigger(const std::string& soundName, SoundTriggerType triggerType) {
    auto it = soundTriggers.find(soundName);
    if (it != soundTriggers.end() && it->second.type == triggerType) {
        soundTriggers.erase(it);
    }
}

void MINIAudioSoundComponent::TriggerEvent(const std::string& eventName) {
    // Check for any sounds that should be triggered by this event
    for (auto& pair : soundTriggers) {
        if (pair.second.type == SoundTriggerType::ON_EVENT &&
            pair.second.eventName == eventName &&
            pair.second.active) {

            PlaySound(pair.first, false);
        }
    }
}

void MINIAudioSoundComponent::Update(float deltaTime, float distanceToListener) {

    // Update music tracks (for fading)
    for (auto& pair : music) {
        pair.second->Update(deltaTime);
    }

    // Process sound triggers
    for (auto& pair : soundTriggers) {
        const std::string& soundName = pair.first;
        SoundTrigger& trigger = pair.second;

        switch (trigger.type) {
        case SoundTriggerType::ON_TIMER:
            trigger.accumulator += deltaTime;
            if (trigger.accumulator >= trigger.parameter) {
                PlaySound(soundName, false);
                trigger.accumulator = 0.0f;
            }
            break;

        case SoundTriggerType::ON_DISTANCE:
            // Only process if distance is provided
            if (distanceToListener >= 0.0f && trigger.active) {
                if (distanceToListener <= trigger.parameter) {
                    PlaySound(soundName, false);
                    trigger.active = false; // Prevent repeated triggers
                }
            }
            // Reset when moving away
            else if (distanceToListener > trigger.parameter * 1.5f) {
                trigger.active = true;
            }
            break;

        default:
            // Other trigger types are handled elsewhere
            break;
        }
    }

    // Update sound sequence if active
    if (playingSequence && !sequenceItems.empty()) {
        sequenceTimer += deltaTime;

        if (sequenceTimer >= sequenceItems[currentSequenceIndex].delay) {
            // Play the sound
            PlaySound(sequenceItems[currentSequenceIndex].soundName, false);

            // Move to next sound in sequence
            currentSequenceIndex++;
            sequenceTimer = 0.0f;

            // End of sequence?
            if (currentSequenceIndex >= sequenceItems.size()) {
                playingSequence = false;
            }
        }
    }
}

bool MINIAudioSoundComponent::IsSoundPlaying(const std::string& name) const {
    auto it = sounds.find(name);
    if (it != sounds.end()) {
        return it->second->IsPlaying();
    }
    return false;
}

bool MINIAudioSoundComponent::IsMusicPlaying(const std::string& name) const {
    auto it = music.find(name);
    if (it != music.end()) {
        return it->second->IsPlaying();
    }
    return false;
}

void MINIAudioSoundComponent::SetRandomPitchRange(const std::string& soundName, float minPitch, float maxPitch) {
    RandomRange range;
    range.enabled = true;
    range.min = std::max(0.5f, minPitch);
    range.max = std::min(2.0f, maxPitch);

    pitchRanges[soundName] = range;
}

void MINIAudioSoundComponent::SetRandomVolumeRange(const std::string& soundName, float minVolume, float maxVolume) {
    RandomRange range;
    range.enabled = true;
    range.min = std::max(0.0f, minVolume);
    range.max = std::min(1.0f, maxVolume);

    volumeRanges[soundName] = range;
}

void MINIAudioSoundComponent::PlaySoundSequence(const std::vector<std::string>& soundNames, const std::vector<float>& delays) {
    // Validate inputs
    if (soundNames.empty() || soundNames.size() != delays.size()) {
        return;
    }

    // Stop any existing sequence
    StopSoundSequence();

    // Setup the new sequence
    sequenceItems.clear();
    for (size_t i = 0; i < soundNames.size(); i++) {
        SoundSequenceItem item;
        item.soundName = soundNames[i];
        item.delay = delays[i];
        sequenceItems.push_back(item);
    }

    // Start the sequence
    currentSequenceIndex = 0;
    sequenceTimer = 0.0f;
    playingSequence = true;
}

void MINIAudioSoundComponent::StopSoundSequence() {
    if (playingSequence && currentSequenceIndex < sequenceItems.size()) {
        StopSound(sequenceItems[currentSequenceIndex].soundName);
    }

    playingSequence = false;
    sequenceItems.clear();
}

void MINIAudioSoundComponent::ApplySoundRandomization(const std::string& name, std::shared_ptr<MINIAudioSound> sound) {
    // Apply random pitch if configured
    auto pitchIt = pitchRanges.find(name);
    if (pitchIt != pitchRanges.end() && pitchIt->second.enabled) {
        sound->SetPitch(GetRandomFloat(pitchIt->second.min, pitchIt->second.max));
    }

    // Apply random volume if configured
    auto volumeIt = volumeRanges.find(name);
    if (volumeIt != volumeRanges.end() && volumeIt->second.enabled) {
        sound->SetVolume(GetRandomFloat(volumeIt->second.min, volumeIt->second.max));
    }
}

float MINIAudioSoundComponent::GetRandomFloat(float min, float max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}

void MINIAudioSoundComponent::CreateLuaMINIAudioBind(sol::state& lua)
{
    // AudioCategory enum
    lua.new_enum("AudioCategory",
        "SFX", AudioCategory::SFX,
        "MUSIC", AudioCategory::MUSIC,
        "VOICE", AudioCategory::VOICE,
        "AMBIENT", AudioCategory::AMBIENT
    );

    // MINIAudioEngine singleton
    lua.new_usertype<MINIAudioEngine>("MINIAudioEngine",
        sol::no_constructor,
        "SetMasterVolume", &MINIAudioEngine::SetMasterVolume,
        "GetMasterVolume", &MINIAudioEngine::GetMasterVolume,
        "SetCategoryVolume", &MINIAudioEngine::SetCategoryVolume,
        "GetCategoryVolume", &MINIAudioEngine::GetCategoryVolume,
        "MuteAll", &MINIAudioEngine::MuteAll,
        "MuteCategory", &MINIAudioEngine::MuteCategory,
        "GetAudioDevices", &MINIAudioEngine::GetAudioDevices,
        "SetAudioDevice", &MINIAudioEngine::SetAudioDevice,
        "GetCurrentDevice", &MINIAudioEngine::GetCurrentDevice
    );
    // Provide a global 'MINIAudioEngine' object in Lua
    lua["MINIAudioEngine"] = &MINIAudioEngine::Instance();

    lua.set_function("AddSound", &MINIAudioSoundComponent::AddSound);
    lua.set_function("AddMusic", &MINIAudioSoundComponent::AddMusic);

    // MINIAudioSoundComponent class
    lua.new_usertype<MINIAudioSoundComponent>(
        "MINIAudioSoundComponent",
        "type_id", &entt::type_hash<MINIAudioSoundComponent>::value,
        sol::call_constructor,
        sol::factories([]() { 
            return MINIAudioSoundComponent(); 
            }),
        "PlaySound", &MINIAudioSoundComponent::PlaySound,
        "StopSound", &MINIAudioSoundComponent::StopSound,
        "PlayMusic", &MINIAudioSoundComponent::PlayMusic,
        "PlayMusicWithFadeIn", &MINIAudioSoundComponent::PlayMusicWithFadeIn,
        "StopMusicWithFadeOut", &MINIAudioSoundComponent::StopMusicWithFadeOut,
        "StopAllSounds", &MINIAudioSoundComponent::StopAllSounds,
        "StopAllMusic", &MINIAudioSoundComponent::StopAllMusic,
        "SetPosition", &MINIAudioSoundComponent::SetPosition,
        "SetVelocity", &MINIAudioSoundComponent::SetVelocity,
        "SetAttenuationRange", &MINIAudioSoundComponent::SetAttenuationRange,
        "SetSoundVolume", &MINIAudioSoundComponent::SetSoundVolume,
        "SetMusicVolume", &MINIAudioSoundComponent::SetMusicVolume
    );
}