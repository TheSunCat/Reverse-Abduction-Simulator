#include "GUICharacterMaker.h"

#include "UIButton.h"
#include "GUIPeople.h"

GUICharacterMaker::GUICharacterMaker() : GUILayer("Character maker"), m_human(UITransform(50, 0, 768, 1080)), m_ufoBeam("UFO beam", simpleTexture({"ObjectData/", "ufoBeam"}, GL_LINEAR), UITransform(1150, 140, 380, 380))
{
    // TODO euch this code sucks

    m_ufoBeam.visible = false;

    m_human.addAnimation("exploding", animatedTexture({"ObjectData/", "explosion"}, 2, 8, GL_NEAREST));
    m_human.addAnimation("dead", TextureManager::None);

    m_human.addToLayer(HumanLayer::HAT, &TextureManager::None);
    m_human.addToLayer(HumanLayer::HAT, "hat/0");
    m_human.addToLayer(HumanLayer::HAT, "hat/1");
    m_human.addToLayer(HumanLayer::HAT, "hat/2", true);
    m_human.addToLayer(HumanLayer::HAT, "hat/3");
    m_human.addToLayer(HumanLayer::FACE, "face/0");
    m_human.addToLayer(HumanLayer::FACE, "face/1");
    m_human.addToLayer(HumanLayer::FACE, "face/2");
    m_human.addToLayer(HumanLayer::FACE, "face/3", true);
    m_human.addToLayer(HumanLayer::TORSO, "torso/0");
    m_human.addToLayer(HumanLayer::TORSO, "torso/1");
    m_human.addToLayer(HumanLayer::TORSO, "torso/2", true);
    m_human.addToLayer(HumanLayer::TORSO, "torso/3");
    m_human.addToLayer(HumanLayer::HANDS, &TextureManager::None);
    m_human.addToLayer(HumanLayer::HANDS, "hands/0");
    m_human.addToLayer(HumanLayer::HANDS, "hands/1", true);
    m_human.addToLayer(HumanLayer::LEGS, "legs/0");
    m_human.addToLayer(HumanLayer::LEGS, "legs/1", true);
    m_human.addToLayer(HumanLayer::LEGS, "legs/2");
    m_human.addToLayer(HumanLayer::LEGS, "legs/3");
    m_human.addToLayer(HumanLayer::LEGS, "legs/4");

    m_human.rollTheDice();

    buttons.push_back(new UIButton("hatL", simpleTexture({"ObjectData/UI/", "leftArrow"}, GL_LINEAR), UITransform(70, 70, 82, 108), Bounds(), [&](UIButton&, int) -> void {
        m_human.changeLayer(HumanLayer::HAT, -1);
        Outrospection::get().audioManager.play("pageTurn" + std::to_string(int(rand() / float(RAND_MAX) * 4)));
    }));
    buttons.push_back(new UIButton("hatR", simpleTexture({"ObjectData/UI/", "rightArrow"}, GL_LINEAR), UITransform(800, 70, 82, 108), Bounds(), [&](UIButton&, int) -> void {
        m_human.changeLayer(HumanLayer::HAT, 1);
        Outrospection::get().audioManager.play("pageTurn" + std::to_string(int(rand() / float(RAND_MAX) * 4)));
    }));

    buttons.push_back(new UIButton("faceL", simpleTexture({"ObjectData/UI/", "leftArrow"}, GL_LINEAR), UITransform(70, 250, 82, 108), Bounds(), [&](UIButton&, int) -> void {
        m_human.changeLayer(HumanLayer::FACE, -1);
        Outrospection::get().audioManager.play("pageTurn" + std::to_string(int(rand() / float(RAND_MAX) * 4)));
    }));
    buttons.push_back(new UIButton("faceR", simpleTexture({"ObjectData/UI/", "rightArrow"}, GL_LINEAR), UITransform(800, 250, 82, 108), Bounds(), [&](UIButton&, int) -> void {
        m_human.changeLayer(HumanLayer::FACE, 1);
        Outrospection::get().audioManager.play("pageTurn" + std::to_string(int(rand() / float(RAND_MAX) * 4)));
    }));

    buttons.push_back(new UIButton("torsoL", simpleTexture({"ObjectData/UI/", "leftArrow"}, GL_LINEAR), UITransform(70, 430, 82, 108), Bounds(), [&](UIButton&, int) -> void {
        m_human.changeLayer(HumanLayer::TORSO, -1);
        Outrospection::get().audioManager.play("pageTurn" + std::to_string(int(rand() / float(RAND_MAX) * 4)));
    }));
    buttons.push_back(new UIButton("torsoR", simpleTexture({"ObjectData/UI/", "rightArrow"}, GL_LINEAR), UITransform(800, 430, 82, 108), Bounds(), [&](UIButton&, int) -> void {
        m_human.changeLayer(HumanLayer::TORSO, 1);
        Outrospection::get().audioManager.play("pageTurn" + std::to_string(int(rand() / float(RAND_MAX) * 4)));
    }));

    buttons.push_back(new UIButton("handsL", simpleTexture({"ObjectData/UI/", "leftArrow"}, GL_LINEAR), UITransform(70, 610, 82, 108), Bounds(), [&](UIButton&, int) -> void {
        m_human.changeLayer(HumanLayer::HANDS, -1);
        Outrospection::get().audioManager.play("pageTurn" + std::to_string(int(rand() / float(RAND_MAX) * 4)));
    }));
    buttons.push_back(new UIButton("handsR", simpleTexture({"ObjectData/UI/", "rightArrow"}, GL_LINEAR), UITransform(800, 610, 82, 108), Bounds(), [&](UIButton&, int) -> void {
        m_human.changeLayer(HumanLayer::HANDS, 1);
        Outrospection::get().audioManager.play("pageTurn" + std::to_string(int(rand() / float(RAND_MAX) * 4)));
    }));

    buttons.push_back(new UIButton("legsL", simpleTexture({"ObjectData/UI/", "leftArrow"}, GL_LINEAR), UITransform(70, 790, 82, 108), Bounds(), [&](UIButton&, int) -> void {
        m_human.changeLayer(HumanLayer::LEGS, -1);
        Outrospection::get().audioManager.play("pageTurn" + std::to_string(int(rand() / float(RAND_MAX) * 4)));
    }));
    buttons.push_back(new UIButton("legsR", simpleTexture({"ObjectData/UI/", "rightArrow"}, GL_LINEAR), UITransform(800, 790, 82, 108), Bounds(), [&](UIButton&, int) -> void {
        m_human.changeLayer(HumanLayer::LEGS, 1);
        Outrospection::get().audioManager.play("pageTurn" + std::to_string(int(rand() / float(RAND_MAX) * 4)));
    }));

    for(UIButton* button : buttons)
    {
        if(button->text[button->text.length() - 1] == 'L')
            button->addAnimation("hovered", simpleTexture({"ObjectData/UI/", "leftArrowHover"}, GL_LINEAR));
        if(button->text[button->text.length() - 1] == 'R')
            button->addAnimation("hovered", simpleTexture({"ObjectData/UI/", "rightArrowHover"}, GL_LINEAR));

        button->onHover = [](UIButton& button, int) { button.setAnimation("hovered"); };
        button->onUnhover = [](UIButton& button, int) { button.setAnimation("default"); };
    }

    buttons.push_back(new UIButton("UFO", simpleTexture({"ObjectData/", "ufo"}, GL_LINEAR), UITransform(1050, 40, 260, 300), Bounds(UITransform(1100, 20, 300), BoundsShape::Circle), [&](UIButton&, int)
    {
        ((GUIPeople*)(Outrospection::get().layerPtrs["people"]))->addHuman(m_human);
        Outrospection::get().audioManager.play("reverseAbduction", 0.2);

        m_ufoBeam.visible = true;

        m_beamTimer.setDuration(2000);
        m_beamTimer.start();

        // reset human with new random stats (roll the dice)
        m_human.rollTheDice();
    }));
}

void GUICharacterMaker::tick()
{
    m_human.tick();
    m_ufoBeam.tick();

    for (UIButton* button : buttons)
    {
        button->tick();
    }

    m_beamTimer.tick();
    if(m_beamTimer.ended())
        m_ufoBeam.visible = false;

}

void GUICharacterMaker::draw() const
{
    m_human.draw();
    m_ufoBeam.draw();

    for (UIButton* button : buttons)
    {
        button->draw();
    }
}

void GUICharacterMaker::moveOutOfTheWay()
{
    m_human.setGoal(-5000, 0);
    for (UIButton* button : buttons)
    {
        if(button->text != "UFO")
            button->setGoal(-5000, 0);
        else
        {
            button->setGoal(650, 40);
        }

        button->onClick = nullptr;
    }
}

GUICharacterMaker::~GUICharacterMaker()
{
    for (UIButton* button : buttons)
    {
        delete button;
    }
}
