#pragma once

#include "GUILayer.h"
#include <glm/glm.hpp>

class UIHuman;

class GUIPeople : public GUILayer
{
public:
    GUIPeople();
    virtual ~GUIPeople();

    void tick() override;
    void draw() const override;

    void addHuman(UIHuman human);
    int humanCount();

    DISALLOW_COPY_AND_ASSIGN(GUIPeople);
private:
    std::vector<UIHuman> m_people;
};
