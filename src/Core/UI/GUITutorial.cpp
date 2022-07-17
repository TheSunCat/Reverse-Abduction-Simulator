#include "GUITutorial.h"

#include "UIButton.h"
#include "GUIStats.h"

GUITutorial::GUITutorial() : GUILayer("Tutorial", false), m_tutorial("Tutorial", simpleTexture({"ObjectData/UI/", "tutorial"}, GL_LINEAR), UITransform(0, 0, 1920, 1080))
{
    buttons.push_back(new UIButton("close", simpleTexture({"ObjectData/UI/", "closeButton"}, GL_LINEAR), UITransform(1820, 20, 80, 80), Bounds(), [] (UIButton&, int) {
        auto& o = Outrospection::get();
        o.popOverlay(o.layerPtrs["tutorial"]);

        o.pushOverlay(o.layerPtrs["background"]);
        o.pushOverlay(o.layerPtrs["people"]);
        o.pushOverlay(o.layerPtrs["characterMaker"]);
        o.pushOverlay(o.layerPtrs["stats"]);

        ((GUIStats*)o.layerPtrs["stats"])->setTimer(150900);
    }));
}

void GUITutorial::tick()
{
    m_tutorial.tick();

    for(UIButton* button : buttons)
    {
        button->tick();
    }
}

void GUITutorial::draw() const
{
    m_tutorial.draw();

    for(UIButton* button : buttons)
    {
        button->draw();
    }
}

GUITutorial::~GUITutorial()
{

}
