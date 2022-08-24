#pragma once
#include "Core.h"

#include <unordered_map>
#include <future>

#ifdef USE_GLFM
#include "glfm.h"
#else
#include <glad/glad.h>
#endif

#include "Core/Resource.h"
#include "Types.h"

#include "SimpleTexture.h"

class TextureManager
{
private:
    std::unordered_map<Resource, std::unique_ptr<SimpleTexture>, Hashes> textures;

public:

    struct Image
    {
        Resource res{};

        unsigned char* bytes = nullptr;
        int width = 10, height = 10;
        int nrComponents = 4;
    };
    struct WantedTexture
    {
        Resource resource;
        GLint filter = GL_LINEAR;

        int tickLength = 1;
        int frameCount = 1;
        bool loop = true;

        Image* data = nullptr;
    };

    TextureManager();

    void requestTexture(const WantedTexture& tex);

    SimpleTexture& loadTexture(const Resource& r, const GLint& filter = GL_LINEAR);

    SimpleTexture& loadAnimatedTexture(const Resource& r, unsigned int textureTickLength,
                                       unsigned int textureFrameCount, const GLint& filter = GL_LINEAR, bool loop = true);

    void bindTexture(Resource& r);

    SimpleTexture& get(const Resource& r);

    void tickAllTextures();

    static SimpleTexture MissingTexture;
    static SimpleTexture None;

    static Image readImageBytes(const Resource& res);
    static void free(unsigned char* data);

    void loadWantedTextures();

    DISALLOW_COPY_AND_ASSIGN(TextureManager);
private:
    static GLuint textureFromFile(const Resource& res, const GLint& filter);

    static void createTexture(const GLuint& texId, const unsigned char* data, const GLint& format,
                              const GLsizei& width, const GLsizei& height, const GLint& filter);

#ifdef PLATFORM_EMSCRIPTEN
    std::vector<WantedTexture> wantedTextures;
#else
    std::vector<std::future<WantedTexture>> wantedTextures;
#endif
};
