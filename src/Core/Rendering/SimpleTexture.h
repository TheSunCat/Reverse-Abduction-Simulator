#pragma once

#include <string>

#ifdef USE_GLFM
#include "glfm.h"
#else
#include <glad/glad.h>
#endif

class SimpleTexture
{
public:
    SimpleTexture() = default;
    SimpleTexture(const GLuint& _texId);

    void bind() const;

    virtual void tick();

    virtual void reset();
    bool shouldTick = false;
    bool loop = true;

    GLuint texId = 0;

    bool operator==(const SimpleTexture& st) const;

    virtual ~SimpleTexture() = default;
};
