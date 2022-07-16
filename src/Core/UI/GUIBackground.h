#pragma once
#include "GUILayer.h"
#include "UIButton.h"

class GUIBackground :
    public GUILayer
{
public:
    GUIBackground();
    virtual ~GUIBackground();

    void tick() override;

    void draw() const override;

    void centerGlobe();

    DISALLOW_COPY_AND_ASSIGN(GUIBackground);
private:
    UIComponent background;
    UIComponent globe;
};
