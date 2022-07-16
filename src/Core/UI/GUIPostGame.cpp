#include "GUIPostGame.h"

GUIPostGame::GUIPostGame() : GUILayer("Postgame GUI", false), m_backgroundFade("fade", simpleTexture({"ObjectData/UI/", "fadeColor"}, GL_NEAREST), UITransform(0, 0, 1920, 1080))
{
    m_backgroundFade.animationSpeed = 0.1;

    m_backgroundFade.opacityGoal = 0;
    m_backgroundFade.warpToGoal();
}

void GUIPostGame::tick()
{
    m_backgroundFade.tick();
}

void GUIPostGame::draw() const
{
    m_backgroundFade.draw();
}

void GUIPostGame::fadeIn()
{
    m_backgroundFade.opacityGoal = 1.0;
}

void GUIPostGame::fadeOut()
{
    m_backgroundFade.opacityGoal = 0.0;
}

GUIPostGame::~GUIPostGame()
{

}
