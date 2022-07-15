#include "GUICharacterMaker.h"

#include "UIButton.h"

GUICharacterMaker::GUICharacterMaker() : GUILayer("Character maker"), m_human(UITransform(200, 100, 400, 880))
{
    m_human.addLayer(HumanLayer::HAT, "placeholderHat");
    m_human.addLayer(HumanLayer::FACE, "placeholderFace");
    m_human.addLayer(HumanLayer::TORSO, "placeholderTorso");
    m_human.addLayer(HumanLayer::HANDS, "placeholderHands");
    m_human.addLayer(HumanLayer::LEGS, "placeholderLegs");

    buttons.push_back(new UIButton("hatLeft", simpleTexture({"ObjectData/UI/", "leftArrow"}, GL_LINEAR), UITransform(70, 100, 82, 108), Bounds(), [](UIButton&, int) -> void {
        LOG("Left arrow pressed");
    }));
    buttons.push_back(new UIButton("hatRight", simpleTexture({"ObjectData/UI/", "rightArrow"}, GL_LINEAR), UITransform(670, 100, 82, 108), Bounds(), [](UIButton&, int) -> void {
        LOG("Right arrow pressed");
    }));

    buttons.push_back(new UIButton("faceLeft", simpleTexture({"ObjectData/UI/", "leftArrow"}, GL_LINEAR), UITransform(70, 250, 82, 108), Bounds(), [](UIButton&, int) -> void {
        LOG("Left arrow pressed");
    }));
    buttons.push_back(new UIButton("faceRight", simpleTexture({"ObjectData/UI/", "rightArrow"}, GL_LINEAR), UITransform(670, 250, 82, 108), Bounds(), [](UIButton&, int) -> void {
        LOG("Right arrow pressed");
    }));

    buttons.push_back(new UIButton("torsoLeft", simpleTexture({"ObjectData/UI/", "leftArrow"}, GL_LINEAR), UITransform(70, 400, 82, 108), Bounds(), [](UIButton&, int) -> void {
        LOG("Left arrow pressed");
    }));
    buttons.push_back(new UIButton("torsoRight", simpleTexture({"ObjectData/UI/", "rightArrow"}, GL_LINEAR), UITransform(670, 400, 82, 108), Bounds(), [](UIButton&, int) -> void {
        LOG("Right arrow pressed");
    }));

    buttons.push_back(new UIButton("handsLeft", simpleTexture({"ObjectData/UI/", "leftArrow"}, GL_LINEAR), UITransform(70, 550, 82, 108), Bounds(), [](UIButton&, int) -> void {
        LOG("Left arrow pressed");
    }));
    buttons.push_back(new UIButton("handsRight", simpleTexture({"ObjectData/UI/", "rightArrow"}, GL_LINEAR), UITransform(670, 550, 82, 108), Bounds(), [](UIButton&, int) -> void {
        LOG("Right arrow pressed");
    }));

    buttons.push_back(new UIButton("legsLeft", simpleTexture({"ObjectData/UI/", "leftArrow"}, GL_LINEAR), UITransform(70, 700, 82, 108), Bounds(), [](UIButton&, int) -> void {
        LOG("Left arrow pressed");
    }));
    buttons.push_back(new UIButton("legsRight", simpleTexture({"ObjectData/UI/", "rightArrow"}, GL_LINEAR), UITransform(670, 700, 82, 108), Bounds(), [](UIButton&, int) -> void {
        LOG("Right arrow pressed");
    }));
}

void GUICharacterMaker::tick()
{
    m_human.tick();

    for (UIButton* button : buttons)
    {
        button->tick();
    }
}

void GUICharacterMaker::draw() const
{
    m_human.draw();

    for (UIButton* button : buttons)
    {
        button->draw();
    }
}
