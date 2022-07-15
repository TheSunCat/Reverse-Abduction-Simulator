#pragma once

#include "UIComponent.h"

enum class HumanLayer
{
    FACE    = 0,
    LEGS    = 1,
    TORSO   = 2,
    HAT     = 3,
    HANDS   = 4,
};

class UIHuman : public UIComponent
{
public:
    UIHuman(const UITransform& transform);

    void draw(Shader& shader = Outrospection::get().shaders["sprite"], const Shader& = Outrospection::get().shaders["glyph"]) const;

    void setLayer(HumanLayer name, SimpleTexture* layer);
    void addLayer(HumanLayer name, const std::string& layer);

private:
    std::array<SimpleTexture*, 5> m_layers =
    {
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
    };
};
