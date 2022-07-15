#include "GUICharacterMaker.h"

GUICharacterMaker::GUICharacterMaker() : GUILayer("Character maker"), m_human(UITransform(100, 100, 400, 880))
{
    m_human.addLayer(HumanLayer::HAT, "placeholderHat");
    m_human.addLayer(HumanLayer::FACE, "placeholderFace");
    m_human.addLayer(HumanLayer::TORSO, "placeholderTorso");
    m_human.addLayer(HumanLayer::HANDS, "placeholderHands");
    m_human.addLayer(HumanLayer::LEGS, "placeholderLegs");
}

void GUICharacterMaker::tick()
{
    m_human.tick();
}

void GUICharacterMaker::draw() const
{
    m_human.draw();
}
