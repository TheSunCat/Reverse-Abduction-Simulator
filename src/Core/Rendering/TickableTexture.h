#pragma once

#include <string>
#include <vector>

#include "SimpleTexture.h"

class TickableTexture : public SimpleTexture
{
public:
    TickableTexture() = default;
    TickableTexture(const std::vector<GLuint>& texIds, const unsigned int _frameLength, bool _shouldRestart);

    void tick() override;
    void tick(unsigned int& stepCount);
    void nextFrame();

    void reset() override;
private:
    std::vector<GLuint> textures;
    unsigned int frameLength = 5;

    unsigned int frameTally = 0;
    GLuint curFrame = 0;
};
