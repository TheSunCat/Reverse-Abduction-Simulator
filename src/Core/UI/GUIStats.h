#pragma once

#include "GUILayer.h"
#include "Timer.h"
#include "UIComponent.h"

class GUIStats : public GUILayer
{
public:
    GUIStats();
    virtual ~GUIStats();

    void setTimer(time_t millis);

    void tick() override;
    void draw() const override;

    DISALLOW_COPY_AND_ASSIGN(GUIStats);
private:
    Timer m_timer;

    UIComponent m_timerDisplay;
    UIComponent m_bossIsBack;

    UIComponent m_timerBlurTop;
};
