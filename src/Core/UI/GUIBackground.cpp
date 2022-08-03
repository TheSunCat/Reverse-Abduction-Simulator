#include "GUIBackground.h"
#include "Outrospection.h"
#include "GUIPeople.h"
#include "GUIPostGame.h"

GUIBackground::GUIBackground() : GUILayer("Background", false),
                                background("Starry sky", animatedTexture({"ObjectData/UI/", "starrySky"}, 10, 2, GL_LINEAR), UITransform(0, 0, 1920, 1080)),
                                globe("globe", GL_NEAREST, UITransform(1100, 20, 880, 880))
{
    globe.addAnimation("explode", animatedTexture({"ObjectData/", "explosion"}, 8, 8, GL_NEAREST));
    globe.addAnimation("gone", Resource());
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

void GUIBackground::startEndSequence()
{
    auto& o = Outrospection::get();

    globe.animationSpeed = 1;
    globe.setGoal(520, 20);
    globe.warpToGoal();

    ((GUIPeople*)o.layerPtrs["people"])->center();

    Util::doLater([this, &o]() {
        globe.setAnimation("explode");
        o.audioManager.play("explodeFinal");

        ((GUIPeople*)o.layerPtrs["people"])->explodeAll();
    }, 1500);

    Util::doLater([this]() {
        globe.setAnimation("gone");
    }, 2550);
}

