#include "GUICharacterMaker.h"

#include "UIButton.h"

GUICharacterMaker::GUICharacterMaker() : GUILayer("Character maker"), m_human(UITransform(200, 100, 400, 880))
{
    m_human.addLayer(HumanLayer::HAT, "placeholderHat");
    m_human.addLayer(HumanLayer::FACE, "placeholderFace");
    m_human.addLayer(HumanLayer::TORSO, "placeholderTorso");
    m_human.addLayer(HumanLayer::HANDS, "placeholderHands");
    m_human.addLayer(HumanLayer::LEGS, "placeholderLegs");

    buttons.push_back(new UIButton("hatL", simpleTexture({"ObjectData/UI/", "leftArrow"}, GL_LINEAR), UITransform(70, 100, 82, 108), Bounds(), [](UIButton&, int) -> void {
        LOG("Left arrow pressed");
    }));
    buttons.push_back(new UIButton("hatR", simpleTexture({"ObjectData/UI/", "rightArrow"}, GL_LINEAR), UITransform(670, 100, 82, 108), Bounds(), [](UIButton&, int) -> void {
        LOG("Right arrow pressed");
    }));

    buttons.push_back(new UIButton("faceL", simpleTexture({"ObjectData/UI/", "leftArrow"}, GL_LINEAR), UITransform(70, 250, 82, 108), Bounds(), [](UIButton&, int) -> void {
        LOG("Left arrow pressed");
    }));
    buttons.push_back(new UIButton("faceR", simpleTexture({"ObjectData/UI/", "rightArrow"}, GL_LINEAR), UITransform(670, 250, 82, 108), Bounds(), [](UIButton&, int) -> void {
        LOG("Right arrow pressed");
    }));

    buttons.push_back(new UIButton("torsoL", simpleTexture({"ObjectData/UI/", "leftArrow"}, GL_LINEAR), UITransform(70, 400, 82, 108), Bounds(), [](UIButton&, int) -> void {
        LOG("Left arrow pressed");
    }));
    buttons.push_back(new UIButton("torsoR", simpleTexture({"ObjectData/UI/", "rightArrow"}, GL_LINEAR), UITransform(670, 400, 82, 108), Bounds(), [](UIButton&, int) -> void {
        LOG("Right arrow pressed");
    }));

    buttons.push_back(new UIButton("handsL", simpleTexture({"ObjectData/UI/", "leftArrow"}, GL_LINEAR), UITransform(70, 550, 82, 108), Bounds(), [](UIButton&, int) -> void {
        LOG("Left arrow pressed");
    }));
    buttons.push_back(new UIButton("handsR", simpleTexture({"ObjectData/UI/", "rightArrow"}, GL_LINEAR), UITransform(670, 550, 82, 108), Bounds(), [](UIButton&, int) -> void {
        LOG("Right arrow pressed");
    }));

    buttons.push_back(new UIButton("legsL", simpleTexture({"ObjectData/UI/", "leftArrow"}, GL_LINEAR), UITransform(70, 700, 82, 108), Bounds(), [](UIButton&, int) -> void {
        LOG("Left arrow pressed");
    }));
    buttons.push_back(new UIButton("legsR", simpleTexture({"ObjectData/UI/", "rightArrow"}, GL_LINEAR), UITransform(670, 700, 82, 108), Bounds(), [](UIButton&, int) -> void {
        LOG("Right arrow pressed");
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

GUICharacterMaker::~GUICharacterMaker()
{
    for (UIButton* button : buttons)
    {
        delete button;
    }
}
