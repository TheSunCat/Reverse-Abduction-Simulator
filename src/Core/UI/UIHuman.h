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

    void draw(Shader& shader = Outrospection::get().shaders["sprite"], const Shader& = Outrospection::get().shaders["glyph"]) const override;
    void tick() override;

    void addToLayer(HumanLayer name, SimpleTexture* texture, bool bad = false);
    void addToLayer(HumanLayer name, const std::string& textureName, bool bad = false);

    void changeLayer(HumanLayer layer, int delta);

    void rollTheDice();

    void setGoal(int x, int y);
    bool hasGoal();
    void warpToGoal();

    bool isBad();

private:
    std::array<std::vector<SimpleTexture*>, 5> m_layers;
    std::array<int, 5> m_curLayer = { 0, 0, 0, 0, 0 };
    std::array<std::vector<bool>, 5> m_layerBad;

    glm::vec2 m_goal = glm::vec2(0);
};
