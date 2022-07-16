#pragma once

#include "GUILayer.h"
#include "UIComponent.h"

class GUIPostGame : public GUILayer
{
public:
    GUIPostGame();

    virtual void tick() override;
    virtual void draw() const override;

    void fadeIn();
    void fadeOut();

    ~GUIPostGame();

private:
    UIComponent m_backgroundFade;
};
