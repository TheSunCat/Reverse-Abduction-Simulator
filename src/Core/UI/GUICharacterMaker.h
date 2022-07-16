#pragma once

#include "GUILayer.h"
#include "UIHuman.h"

#include <Timer.h>

class GUICharacterMaker : public GUILayer
{
public:
    GUICharacterMaker();
    ~GUICharacterMaker();

    void tick() override;
    void draw() const override;

    void moveOutOfTheWay();

    DISALLOW_COPY_AND_ASSIGN(GUICharacterMaker);
private:

    UIHuman m_human;

    UIComponent m_ufoBeam;

    Timer m_beamTimer;
};
