#include "miniaudio.h"
#include "MINIAudioSound.h"
#include "MINIAudioEngine.h"

MINIAudioSound::MINIAudioSound(const std::string& filePath)
    : filePath(filePath)
    , loaded(false)
    , volume(1.0f)
    , category(AudioCategory::SFX)
    , playing(false)
    , paused(false)
{
    engine = MINIAudioEngine::Instance().GetEngine();

    // Initialize the sound
    ma_result result = ma_sound_init_from_file(engine, filePath.c_str(), 0, NULL, NULL, &sound);
    loaded = (result == MA_SUCCESS);

    if (loaded) {
        // Register with the audio engine
        MINIAudioEngine::Instance().RegisterSound(this);
    }
}

MINIAudioSound::~MINIAudioSound() {
    if (loaded) {
        Stop(); // Ensure the sound is stopped
        ma_sound_uninit(&sound);
        MINIAudioEngine::Instance().UnregisterSound(this);
    }
}

bool MINIAudioSound::Play() {
    if (!loaded) return false;

    ma_result result = ma_sound_start(&sound);
    if (result == MA_SUCCESS) {
        playing = true;
        paused = false;
        return true;
    }
    return false;
}

void MINIAudioSound::Stop() {
    if (!loaded) return;

    ma_sound_stop(&sound);
    ma_sound_seek_to_pcm_frame(&sound, 0); // Reset position
    playing = false;
    paused = false;
}

void MINIAudioSound::Pause() {
    if (!loaded || !playing || paused) return;

    ma_sound_stop(&sound);
    paused = true;
}

void MINIAudioSound::Resume() {
    if (!loaded || !paused) return;

    ma_sound_start(&sound);
    paused = false;
}

void MINIAudioSound::SetVolume(float vol) {
    volume = std::max(0.0f, std::min(vol, 1.0f));
    UpdateVolume();
}

float MINIAudioSound::GetVolume() const {
    return volume;
}

void MINIAudioSound::SetPitch(float pitch) {
    if (!loaded) return;

    // Clamp pitch to reasonable values
    pitch = std::max(0.5f, std::min(pitch, 2.0f));
    ma_sound_set_pitch(&sound, pitch);
}

float MINIAudioSound::GetPitch() const {
    if (!loaded) return 1.0f;

    return ma_sound_get_pitch(&sound);
}

void MINIAudioSound::SetPan(float pan) {
    if (!loaded) return;

    // Clamp pan between -1.0 (left) and 1.0 (right)
    pan = std::max(-1.0f, std::min(pan, 1.0f));
    ma_sound_set_pan(&sound, pan);
}

float MINIAudioSound::GetPan() const {
    if (!loaded) return 0.0f;

    return ma_sound_get_pan(&sound);
}

void MINIAudioSound::SetLooping(bool loop) {
    if (!loaded) return;

    ma_sound_set_looping(&sound, loop);
}

bool MINIAudioSound::IsLooping() const {
    if (!loaded) return false;

    return ma_sound_is_looping(&sound);
}

void MINIAudioSound::SetPosition(float x, float y, float z) {
    if (!loaded) return;

    ma_sound_set_position(&sound, x, y, z);
}

void MINIAudioSound::SetVelocity(float x, float y, float z) {
    if (!loaded) return;

    ma_sound_set_velocity(&sound, x, y, z);
}

void MINIAudioSound::SetAttenuationRange(float minDistance, float maxDistance) {
    if (!loaded) return;

    ma_sound_set_spatialization_enabled(&sound, MA_TRUE);
    ma_sound_set_attenuation_model(&sound, ma_attenuation_model_linear);
    ma_sound_set_min_distance(&sound, minDistance);
    ma_sound_set_max_distance(&sound, maxDistance);

    if (ma_sound_is_spatialization_enabled(&sound)) {
        std::cout << "Spatialization enabled\n";
    }
}

bool MINIAudioSound::IsPlaying() {
    if (!loaded) return false;

    if (paused) return false;

    // Check with the miniaudio API
    ma_bool32 isPlaying = ma_sound_is_playing(&sound);

    // Update our internal state
    if (isPlaying == MA_FALSE && playing) {
        // MINIAudioSound has finished playing
        playing = false;
        if (finishedCallback) {
            finishedCallback();
        }
    }

    return isPlaying == MA_TRUE;
}

bool MINIAudioSound::IsPaused() const {
    return paused;
}

bool MINIAudioSound::IsLoaded() const {
    return loaded;
}

float MINIAudioSound::GetDuration() const {
    if (!loaded) return 0.0f;

    float durationInSeconds = 0.0f;
    ma_sound_get_length_in_seconds(const_cast<ma_sound*>(&sound), &durationInSeconds);
    return durationInSeconds;
}

float MINIAudioSound::GetPlaybackPosition() const {
    if (!loaded) return 0.0f;

    float positionInSeconds = 0.0f;
    ma_sound_get_cursor_in_seconds(const_cast<ma_sound*>(&sound), &positionInSeconds);
    return positionInSeconds;
}

void MINIAudioSound::SetPlaybackPosition(float positionInSeconds) {
    if (!loaded) return;

    float duration = GetDuration();
    if (positionInSeconds < 0) positionInSeconds = 0;
    if (positionInSeconds > duration) positionInSeconds = duration;

    ma_uint64 frameCount = 0;
    ma_uint32 sampleRate = ma_engine_get_sample_rate(engine);
    frameCount = (ma_uint64)(positionInSeconds * sampleRate);

    ma_sound_seek_to_pcm_frame(&sound, frameCount);
}

void MINIAudioSound::SetCategory(AudioCategory cat) {
    category = cat;
    UpdateVolume();
}

AudioCategory MINIAudioSound::GetCategory() const {
    return category;
}

void MINIAudioSound::UpdateVolume() {
    if (!loaded) return;

    float effectiveVolume = volume;

    // Apply category volume
    effectiveVolume *= MINIAudioEngine::Instance().GetCategoryVolume(category);

    // Apply mute if applicable
    if (MINIAudioEngine::Instance().IsCategoryMuted(category)) {
        effectiveVolume = 0.0f;
    }

    ma_sound_set_volume(&sound, effectiveVolume);
}

void MINIAudioSound::SetFinishedCallback(std::function<void()> callback) {
    finishedCallback = callback;
}