#pragma once

#include "GUILayer.h"
#include "UIComponent.h"

class GUIPostGame : public GUILayer
{
public:
    GUIPostGame();

    virtual void tick() override;
    virtual void draw() const override;

    void start(bool goodEnding);

    ~GUIPostGame();

private:
    UIComponent m_backgroundFade;
    UIComponent m_planetDown;
    UIComponent m_creditsSequence;

    UIComponent m_starrySky;

    UIComponent m_ufo;
    UIComponent m_boss;

    UIComponent m_ufoText;
    UIComponent m_bossText;
};
