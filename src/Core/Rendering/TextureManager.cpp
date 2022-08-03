#include "TextureManager.h"

#include <External/stb_image.h>
#include <string>

#include "Util.h"
#include "Core/File.h"
#include "Core/Rendering/TickableTexture.h"

SimpleTexture TextureManager::MissingTexture(-1);
SimpleTexture TextureManager::None(-2);

TextureManager::TextureManager()
{
    // create error texture
    const unsigned char missingTexData[] = {
        255, 0, 220, // purple, top left
        0, 0, 0, // black, top right
        0, 0, 0, // black, bottom left
        255, 0, 220 // purple, bottom right
    };

    GLuint texId = -1;
    glGenTextures(1, &texId);
    createTexture(texId, missingTexData, GL_RGB, 2, 2, GL_NEAREST);

    MissingTexture.texId = texId;

    // create blank texture
    const unsigned char noneTexData[] = {
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0, // all transparent RGBA
    };

    glGenTextures(1, &texId);
    createTexture(texId, noneTexData, GL_RGBA, 2, 2, GL_NEAREST);

    None.texId = texId;
}

SimpleTexture& TextureManager::loadTexture(const Resource& res, const GLint& filter)
{
    Resource r = res;
    r.setExtension("png");

    const GLuint texId = textureFromFile(r, filter);

    if (texId != INT_MAX)
    {
        SimpleTexture texObj(texId);

        textures.insert(std::pair(r, std::make_unique<SimpleTexture>(texObj)));

        return *textures[r];
    }
    else
    {
        LOG_ERROR("Failed to generate texture ID for %s", r.getPath());

        return MissingTexture;
    }
}

SimpleTexture& TextureManager::loadAnimatedTexture(const Resource& res, unsigned int textureTickLength,
                                                   const unsigned int textureFrameCount, const GLint& filter, bool loop)
{
    Resource r = res;
    r.setExtension("png");

    std::vector<GLuint> textureIds;

    for (unsigned int i = 0; i < textureFrameCount; i++)
    {
        Resource curRes = r.getNth(i);

        GLuint currentTextureId = textureFromFile(curRes, filter);

        if (currentTextureId != INT_MAX)
        {
            textureIds.push_back(currentTextureId);
        }
        else
        {
            LOG_ERROR("Failed to generate texture ID for animated texture frame %i at %s", textureFrameCount,
                      curRes.getPath());

            textureIds.push_back(MissingTexture.texId);
        }
    }

    auto [it, success] = textures.insert(
        std::pair(r, std::make_unique<TickableTexture>(textureIds, textureTickLength, loop)));

    return *(it->second);
}

void TextureManager::bindTexture(Resource& r)
{
    const SimpleTexture& tex = get(r);

    tex.bind();
}

SimpleTexture& TextureManager::get(const Resource& r)
{
    const auto f = textures.find(r);

    if (f == textures.end())
    {
        // resource not found in already existing storage, needs to be loaded
        LOG_ERROR("Texture %s was not loaded before fetching! Loading now as fallback...", r.getPath());
        return loadTexture(r, GL_LINEAR);
    }
    else
    {
        return *(*f).second;
    }
}

// Called every tick, calls tick on every tickable texture.
void TextureManager::tickAllTextures()
{
    for (auto& val : textures)
    {
        val.second->tick();
    }
}

unsigned char* TextureManager::readImageBytes(const Resource& res, int& width, int& height, int& nrComponents)
{
    File file = File(res);
    const std::vector<unsigned char> data = file.readAllBytes();

    return stbi_load_from_memory(data.data(), int(data.size()), &width, &height, &nrComponents, 0);
}

void TextureManager::free(unsigned char* data)
{
    stbi_image_free(data);
}

void TextureManager::createTexture(const GLuint& texId, const unsigned char* data, const GLint& format,
                                   const GLsizei& width, const GLsizei& height, const GLint& filter)
{
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    
    // TODO make this an option
    //glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
}

GLuint TextureManager::textureFromFile(const Resource& res, const GLint& filter)
{
    GLuint tex;
    int width, height;
    glGenTextures(1, &tex);

    int nrComponents = 0;
    unsigned char* data = readImageBytes(res, width, height, nrComponents);

    if (data)
    {
        GLint format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        createTexture(tex, data, format, width, height, filter);

        stbi_image_free(data);

        return tex;
    }
    else
    {
        LOG_ERROR("Texture failed to load at path: %s", res.getPath());
        LOG_ERROR("stbi_failure_reason: %s", stbi_failure_reason());
        stbi_image_free(data);

        return INT_MAX;
    }
}
