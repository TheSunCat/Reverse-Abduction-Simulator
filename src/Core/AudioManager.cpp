#include "AudioManager.h"

#include "Util.h"
#include "Core/File.h"

static void loadSound(SoLoud::Wav* wave, const std::string& soundName)
{
    File file = File({"SoundData/", soundName, "ogg"});
    std::vector<unsigned char> data = file.readAllBytes();

    wave->loadMem(data.data(), data.size(), false, false); // load the file
}

void AudioManager::init(const std::vector<std::string>& sounds)
{
    engine.init(0U, // aFlags
                0U, // aBackend
                0U, // aSampleRate
                0U, // aBufferSize
                1U);// aChannels

    engine.setGlobalVolume(0.5);

    // load the unordered_map with the keys, then load asynchronously
    for(const std::string& sound : sounds) {
        auto [it, success] = waves.try_emplace(sound);

        if(!it->second) {
            it->second = std::make_unique<SoLoud::Wav>();

            m_futureWaves.push_back(std::async(std::launch::async, loadSound, it->second.get(), sound));
        }
    }
}

AudioManager::~AudioManager()
{
    engine.deinit();
}

float filter_param0[6] = { 0, 0, 0, 0, 0, 0 };
float filter_param1[6] = { 1000, 8000, 0, 0, 0 ,0 };
float filter_param2[6] = { 2, 3,  0, 0, 0, 0 };

void AudioManager::play(const std::string& soundName, float vol, bool loop)
{
    SoLoud::Wav* wave = nullptr;

    auto& [key, wavePtr] = *waves.try_emplace(soundName).first;
    if (wavePtr)
    {
        wave = &*wavePtr;
    }
    else
    {
        LOG_ERROR("Sound \"%s\" was played without being pre-loaded! Please add its name to the init call.", soundName);

        loadSound(wavePtr.get(), soundName);

        wave = waves[soundName].get();
    }
    
    wave->setLooping(loop);

    handles.insert_or_assign(soundName, engine.play(*wave, vol));
}

void AudioManager::stop(const std::string& soundName)
{
    auto f = handles.find(soundName); // <- I'm getting the error here
    if (f == handles.end()) {
        LOG_ERROR("Tried to stop nonexistent sound \"%s\"!", soundName);
        return;
    }

    engine.stop(f->second);
}

void AudioManager::setSoundVolume(const std::string& sound, float vol)
{
    auto f = handles.find(sound); 
    if (f == handles.end()) {
        LOG_ERROR("Tried to change volume of nonexistent sound \"%s\"!", sound);
        return;
    }

    engine.setVolume(f->second, vol);
}

void AudioManager::setGlobalVolume(float vol)
{
    engine.setGlobalVolume(vol);
}
