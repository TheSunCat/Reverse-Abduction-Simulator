#pragma once

#include "Platform.h"

#ifdef PLATFORM_ANDROID
#include <android/asset_manager.h>
#endif

#include "Resource.h"
#include "Core.h"

class File {
public:
    File(Resource res);

    bool exists();
    std::string path();

    std::vector<unsigned char> readAllBytes();

    // only if file is a directory
    std::vector<std::string> listFiles();

    DISALLOW_COPY_AND_ASSIGN(File);

private:
    Resource resource;

#ifdef PLATFORM_ANDROID
    static AAssetManager* assetManager;
#endif
};
