#include "GUIBackground.h"
#include "Outrospection.h"

GUIBackground::GUIBackground() : GUILayer("Background", false),
                                background("Starry sky", animatedTexture({"ObjectData/UI/", "starrySky"}, 10, 2, GL_LINEAR), UITransform(0, 0, 1920, 1080)),
                                globe("globe", GL_NEAREST, UITransform(960, 100, 880, 880))
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
