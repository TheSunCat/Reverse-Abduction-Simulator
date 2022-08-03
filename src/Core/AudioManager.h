#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <list>
#include <thread>
#include <vector>

#ifndef PLATFORM_EMSCRIPTEN
#include <future>
#endif

#include <soloud.h>
#include <soloud_wav.h>

#include "Core.h"

class AudioManager
{
private:
    SoLoud::Soloud engine;

#ifndef PLATFORM_EMSCRIPTEN
    std::vector<std::future<void>> m_futureWaves;
#endif

    std::unordered_map<std::string, std::unique_ptr<SoLoud::Wav>> waves;
    std::unordered_map<std::string, SoLoud::handle> handles;
public:
    AudioManager() = default;
    ~AudioManager();

    void init(const std::vector<std::string>& sounds = std::vector<std::string>());
    void play(const std::string& soundName, float vol = 1.0f, bool loop = false);
    void stop(const std::string& soundName);

    void setSoundVolume(const std::string& sound, float vol);
    void setGlobalVolume(float vol);
};
