#pragma once

#include "GUILayer.h"
#include "UIComponent.h"

class GUITutorial : public GUILayer
{
public:
    GUITutorial();
    ~GUITutorial();

    void tick() override;
    void draw() const override;

private:
    UIComponent m_tutorial;
};
