#pragma once

#include "GUILayer.h"
#include "UIHuman.h"

class GUICharacterMaker : public GUILayer
{
public:
    GUICharacterMaker();
    ~GUICharacterMaker();

    void tick() override;
    void draw() const override;

    DISALLOW_COPY_AND_ASSIGN(GUICharacterMaker);
private:

    UIHuman m_human;

    UIComponent m_ufoBeam;
};
