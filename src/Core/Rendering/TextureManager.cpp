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
    if(r.empty())
        return TextureManager::None;

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
    if(!wantedTextures.empty())
    {
        // asynchronously load all needed textures
        loadWantedTextures();
    }

    for (auto& val : textures)
    {
        val.second->tick();
    }
}

TextureManager::Image TextureManager::readImageBytes(const Resource& res)
{
    File file = File(res);
    const std::vector<unsigned char> data = file.readAllBytes();

    Image ret = Image{res};
    ret.bytes = stbi_load_from_memory(data.data(), int(data.size()), &ret.width, &ret.height, &ret.nrComponents, 0);

    assert(ret.bytes);

    return ret;
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
    glGenTextures(1, &tex);

    Image image = readImageBytes(res);

    if (image.bytes)
    {
        GLint format;
        switch(image.nrComponents) {
            case 1:
                format = GL_RED;
                break;
            case 3:
                format = GL_RGB;
                break;
            case 4:
                format = GL_RGBA;
                break;
        }

        createTexture(tex, image.bytes, format, image.width, image.height, filter);

        stbi_image_free(image.bytes);

        return tex;
    }
    else
    {
        LOG_ERROR("Texture failed to load at path: %s", res.getPath());
        LOG_ERROR("stbi_failure_reason: %s", stbi_failure_reason());
        stbi_image_free(image.bytes);

        return INT_MAX;
    }
}

#include <future>

void TextureManager::requestTexture(const WantedTexture& tex)
{
#ifdef PLATFORM_EMSCRIPTEN
    wantedTextures.push_back([tex] {
#else
    wantedTextures.emplace_back(std::async(std::launch::async, [tex] {
#endif
        WantedTexture ret = tex;

        ret.data = new Image[ret.frameCount];

        // TODO async this, too
        for (int i = 0; i < ret.frameCount; i++) {
            Resource curRes;
            if (ret.frameCount == 1)
                curRes = ret.resource;
            else
                curRes = ret.resource.getNth(i);

            ret.data[i] = readImageBytes(curRes);
        }

        return ret;
#ifdef PLATFORM_EMSCRIPTEN
    }());
#else
    }));
#endif
}

void TextureManager::loadWantedTextures()
{
#ifdef PLATFORM_EMSCRIPTEN
    for(auto& wantedTex : wantedTextures)
    {
#else
    for(auto& wantedTexFuture : wantedTextures)
    {
        //wantedTexFuture.wait();
        const WantedTexture& wantedTex = wantedTexFuture.get();
#endif

        std::vector<GLuint> texIDs;

        for(int i = 0; i < wantedTex.frameCount; i++) {
            GLuint texID;
            glGenTextures(1, &texID);

            if (wantedTex.data[i].bytes) {
                GLint format;
                switch (wantedTex.data[i].nrComponents) {
                    case 1:
                        format = GL_RED;
                        break;
                    case 3:
                        format = GL_RGB;
                        break;
                    case 4:
                        format = GL_RGBA;
                        break;
                }

                // upload to GPU
                createTexture(texID, wantedTex.data[i].bytes, format, wantedTex.data[i].width, wantedTex.data[i].height, wantedTex.filter);

                texIDs.push_back(texID);
            } else {
                LOG_ERROR("Texture failed to load at path: %s", wantedTex.resource.getPath());
                LOG_ERROR("stbi_failure_reason: %s", stbi_failure_reason());

                texIDs.push_back(TextureManager::MissingTexture.texId);
            }

            stbi_image_free(wantedTex.data[i].bytes);
        }

        if(texIDs.size() == 1)
        {
            textures.insert(std::make_pair(wantedTex.resource, std::make_unique<SimpleTexture>(texIDs[0])));
        } else {
            textures.insert(std::make_pair(wantedTex.resource, std::make_unique<TickableTexture>(texIDs, wantedTex.tickLength, wantedTex.loop)));
        }
    }

    wantedTextures.clear();
}
