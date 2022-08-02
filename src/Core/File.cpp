#include "File.h"

#include "Util.h"

#include <fstream>
#include <utility>

#ifdef PLATFORM_ANDROID
AAssetManager* File::assetManager = nullptr;
#endif

File::File(Resource res) : resource(std::move(res))
{
#ifdef PLATFORM_ANDROID
    if(!assetManager) {
        // get the asset manager for the first time

        ANativeActivity *activity = glfmAndroidGetActivity();
        if (activity) {
            assetManager = activity->assetManager;
        }

        assert(activity != nullptr);
    }
#endif
}

bool File::exists()
{
    std::string fullPath = Util::path(resource.getPath());

    LOG("Checking file %s", fullPath);

#ifndef PLATFORM_ANDROID
    return std::filesystem::exists(fullPath);
#else
    AAsset* asset = AAssetManager_open(assetManager, fullPath.c_str(), AASSET_MODE_UNKNOWN);
    return asset != nullptr;
#endif
}

std::string File::path()
{
    return Util::path(resource.getPath());
}

std::vector<unsigned char> File::readAllBytes()
{
    //assert(!resource.isDir);

    std::string fullPath = Util::path(resource.getPath());

    unsigned int length = 0;
    std::vector<unsigned char> ret;

    LOG("readAllBytes(%s)", fullPath);
#ifdef PLATFORM_ANDROID
    assert(assetManager != nullptr);

    AAsset* asset = AAssetManager_open(assetManager, fullPath.c_str(), AASSET_MODE_UNKNOWN);

    if (asset) {
        length = AAsset_getLength(asset);
        ret.resize(length);

        AAsset_read(asset, ret.data(), length);
        return ret;
    } else {
        LOG_ERROR("Failed to read file \"%s\"!", fullPath);

        length = 0;
        return ret;
    }
#else
    std::ifstream fileStream;

    // enable exceptions
    fileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        // open file
        fileStream.open(fullPath, std::ifstream::binary | std::ifstream::in | std::ios::ate);

        // go to end, figure out size
        fileStream.seekg(0, std::ios::end);
        length = fileStream.tellg();
        ret.resize(length);

        // go back to the beginning to read
        fileStream.seekg(0, std::ios::beg);
        fileStream.read((char*) ret.data(), length);

        LOG("Read %i-long file %s", length, fullPath);

        fileStream.close();

        return ret;
    }
    catch (std::ifstream::failure& e)
    {
        LOG_ERROR("Failed to read file \"%s\"! errno %s", fullPath, e.what());

        length = 0;
        return ret;
    }
#endif
}


std::vector<std::string> File::listFiles()
{
    //(resource.isDir);

    std::vector<std::string> ret;

    std::string fullDir = Util::path(resource.getPath());

#ifdef PLATFORM_ANDROID
    AAssetDir* dir = AAssetManager_openDir(assetManager, fullDir.c_str());

    const char* curDir;
    while((curDir = AAssetDir_getNextFileName(dir)) != nullptr) {
        ret.push_back(curDir);
    }
#else
    for (const auto& entry : std::filesystem::directory_iterator(fullDir)) {
        auto str = entry.path().string();
        std::replace(str.begin(), str.end(), '\\', '/');

        ret.emplace_back(str);
    }
#endif

    return ret;
}
