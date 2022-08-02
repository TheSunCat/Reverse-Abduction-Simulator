#pragma once
#include "Core.h"

#include <unordered_map>

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
    TextureManager();

    SimpleTexture& loadTexture(const Resource& r, const GLint& filter = GL_LINEAR);

    SimpleTexture& loadAnimatedTexture(const Resource& r, unsigned int textureTickLength,
                                       unsigned int textureFrameCount, const GLint& filter = GL_LINEAR, bool loop = true);

    void bindTexture(Resource& r);

    SimpleTexture& get(const Resource& r);

    void tickAllTextures();

    static SimpleTexture MissingTexture;
    static SimpleTexture None;

    static unsigned char* readImageBytes(const Resource& res, int& width, int& height, int& nrComponents);
    static void free(unsigned char* data);

    DISALLOW_COPY_AND_ASSIGN(TextureManager);
private:
    static GLuint textureFromFile(const Resource& res, const GLint& filter);
    static void createTexture(const GLuint& texId, const unsigned char* data, const GLint& format,
                              const GLsizei& width, const GLsizei& height, const GLint& filter);
};
