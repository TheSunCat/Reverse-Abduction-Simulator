#include "GUIBackground.h"
#include "Outrospection.h"

GUIBackground::GUIBackground() : GUILayer("Background", false),
                                background("Starry sky", animatedTexture({"ObjectData/UI/", "starrySky"}, 10, 2, GL_LINEAR), UITransform(0, 0, 1920, 1080)),
                                globe("globe", GL_NEAREST, UITransform(1100, 20, 880, 880))
{

}

GUIBackground::~GUIBackground()
{

}


void GUIBackground::tick()
{
    background.tick();
    globe.tick();
}

void GUIBackground::draw() const
{
    background.draw();
    globe.draw();
}

void GUIBackground::centerGlobe()
{
    globe.setGoal(520, 20);
    //globe.warpToGoal();
}

