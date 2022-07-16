#include "GUIBackground.h"
#include "Outrospection.h"
#include "GUIPeople.h"
#include "GUIPostGame.h"

GUIBackground::GUIBackground() : GUILayer("Background", false),
                                background("Starry sky", animatedTexture({"ObjectData/UI/", "starrySky"}, 10, 2, GL_LINEAR), UITransform(0, 0, 1920, 1080)),
                                globe("globe", GL_NEAREST, UITransform(1100, 20, 880, 880))
{
    globe.addAnimation("explode", animatedTexture({"ObjectData/", "explosion"}, 4, 8, GL_NEAREST));
    globe.addAnimation("gone", TextureManager::None);
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
    o.popOverlay(o.layerPtrs["characterMaker"]);
    o.popOverlay(o.layerPtrs["stats"]);


    globe.animationSpeed = 0.1;
    globe.setGoal(520, 20);

    ((GUIPeople*)o.layerPtrs["people"])->center();

    Util::doLater([this]() {
        auto& o = Outrospection::get();

        globe.setAnimation("explode");
        Outrospection::get().audioManager.play("explode", 0.5);

        ((GUIPeople*)o.layerPtrs["people"])->explodeAll();
    }, 1000);

    Util::doLater([this]() {
        auto& o = Outrospection::get();

        globe.setAnimation("gone");

        o.pushOverlay(o.layerPtrs["postGame"]);
        ((GUIPostGame*)o.layerPtrs["postGame"])->fadeIn();
    }, 1550);
}

