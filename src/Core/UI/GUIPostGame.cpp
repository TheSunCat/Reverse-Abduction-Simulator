#include "GUIPostGame.h"
#include "GUIBackground.h"
#include "GUIStats.h"
#include "GUIPeople.h"
#include "GUIStats.h"

GUIPostGame::GUIPostGame() : GUILayer("Postgame GUI", false),
                    m_backgroundFade("fade", simpleTexture({"ObjectData/UI/", "fadeColor"}, GL_NEAREST), UITransform(0, 0, 1920, 1080)),
                    m_planetDown("Planet counter", TextureManager::None, UITransform(0, 0, 1920, 1080)),
                    m_creditsSequence("Credits sequence", simpleTexture({"ObjectData/UI/", "credits"}, GL_NEAREST), UITransform(0, 0, 1920, 1080)),
                    m_starrySky("Starry sky (end)", animatedTexture({"ObjectData/UI/", "starrySky"}, 4, 2, GL_LINEAR), UITransform(0, 0, 1920, 1080)),
                    m_ufo("End UFO", simpleTexture({"ObjectData/end/", "ufo"}, GL_LINEAR), UITransform(0, 0, 1920, 1080)),
                    m_boss("End boss UFO", simpleTexture({"ObjectData/end/", "boss"}, GL_LINEAR), UITransform(2000, 0, 1920, 1080)),
                    m_ufoText("End UFO text", simpleTexture({"ObjectData/end/", "ellipsis"}, GL_LINEAR), UITransform(0, 0, 1920, 1080)),
                    m_bossText("End Boss text", simpleTexture({"ObjectData/end/", "whatDidIMiss"}, GL_LINEAR), UITransform(0, 0, 1920, 1080)),
                    m_human("human", simpleTexture({"ObjectData/", "purplePerson"}, GL_LINEAR), UITransform(1400, 800, 99, 120)),
                    m_scoreValueText("End score value", TextureManager::None, UITransform(1500, 830, 100, 100))

{
    m_backgroundFade.animationSpeed = 0.1;
    m_backgroundFade.opacityGoal = 0;
    m_backgroundFade.warpToGoal();

    m_planetDown.animationSpeed = 0.1;
    m_planetDown.opacityGoal = 0;
    m_planetDown.warpToGoal();

    m_creditsSequence.animationSpeed = 0.1;
    m_creditsSequence.opacityGoal = 0;
    m_creditsSequence.warpToGoal();

    m_starrySky.flip = true;
    m_ufo.visible = false;
    m_boss.visible = false;
    m_starrySky.visible = false;

    m_ufo.animationSpeed = 0.1;
    m_boss.animationSpeed = 0.1;

    m_ufo.bobUpAndDown = true;
    m_boss.bobUpAndDown = true;

    m_ufoText.visible = false;
    m_bossText.visible = false;

    m_scoreValueText.visible = false;
    m_scoreValueText.textSize = 2;
    m_scoreValueText.textShadow = true;
    m_scoreValueText.textColor = Color(0.6475, 0.5569, 0.9686);

    m_human.visible = false;


    m_planetDown.addAnimation("planetDown", animatedTexture({"ObjectData/", "goodbyeWorld/"}, 3, 32, GL_NEAREST, false));

    m_ufo.addAnimation("shrug", animatedTexture({"ObjectData/end/", "ufoShrug"}, 5, 2, GL_LINEAR));
    m_ufo.addAnimation("thumbsup", simpleTexture({"ObjectData/end/", "ufoThumbsup"}, GL_LINEAR));
    m_ufo.addAnimation("sad", simpleTexture({"ObjectData/end/", "ufoSad"}, GL_LINEAR));
    m_ufo.addAnimation("boom", simpleTexture({"ObjectData/end/", "ufoBoom"}, GL_LINEAR));

    m_bossText.addAnimation("so that's what humans", simpleTexture({"ObjectData/end/", "soThatsWhatHumans"}, GL_LINEAR));
    m_bossText.addAnimation("look like. huh.", simpleTexture({"ObjectData/end/", "lookLike"}, GL_LINEAR));

    m_bossText.addAnimation("where did the humans go?", simpleTexture({"ObjectData/end/", "whereDidTheyGo"}, GL_LINEAR));
    m_bossText.addAnimation("ok boom time.", simpleTexture({"ObjectData/end/", "okBoomTime"}, GL_LINEAR));
}

void GUIPostGame::tick()
{
    m_starrySky.tick();
    m_creditsSequence.tick();
    m_backgroundFade.tick();
    m_planetDown.tick();

    m_ufo.tick();
    m_boss.tick();
    m_ufoText.tick();
    m_bossText.tick();
    m_scoreValueText.tick();
    m_human.tick();

    for(UIButton* button : buttons)
    {
        button->tick();
    }
}

void GUIPostGame::draw() const
{
    m_starrySky.draw();
    m_boss.draw();
    m_ufo.draw();
    m_ufoText.draw();
    m_bossText.draw();

    m_creditsSequence.draw();
    m_scoreValueText.draw();
    m_human.draw();

    for(UIButton* button : buttons)
    {
        button->draw();
    }

    m_backgroundFade.draw();
    m_planetDown.draw();

}

void GUIPostGame::start(bool goodEnding)
{
    std::cout << "Started postgame!" << std::endl;

    // TODO this code is bad, eh

    auto& o = Outrospection::get();

    // save the score before humans explode
    setScore(((GUIPeople*)o.layerPtrs["people"])->humanCount());

    o.pushOverlay(o.layerPtrs["postGame"]);

    o.audioManager.stop("newsongfornewgame");
    o.audioManager.play("end", 1.0, true);

    // start end cutscene
    m_backgroundFade.opacityGoal = 1.0;

    // do stuff while faded
    Util::doLater([this, &o](){
        m_ufo.visible = true;
        m_boss.visible = true;
        m_starrySky.visible = true;
        o.popOverlay(o.layerPtrs["characterMaker"]);
        o.popOverlay(o.layerPtrs["stats"]);

        m_backgroundFade.opacityGoal = 0.0;
    }, 2000);

    Util::doLater([this](){
        m_boss.setGoal(1, 0);
        m_bossText.visible = true;
    }, 3000);

    Util::doLater([this](){
        m_ufoText.visible = true;
        m_bossText.visible = false;
    }, 5000);

    Util::doLater([this](){
        m_ufo.setAnimation("shrug");
    }, 6000);

    Util::doLater([this, goodEnding](){
        m_ufoText.visible = false;
        m_bossText.visible = true;

        if(goodEnding) {
            m_bossText.setAnimation("so that's what humans");
        } else {
            m_bossText.setAnimation("where did the humans go?");
        }
    }, 7000);

    Util::doLater([this, goodEnding](){
        if(goodEnding)
            m_ufo.setAnimation("thumbsup");
        else
            m_ufo.setAnimation("sad");
    }, 8000);

    Util::doLater([this, goodEnding](){
        m_ufoText.visible = false;
        m_bossText.visible = true;

        if(goodEnding) {
            m_bossText.setAnimation("look like. huh.");
        } else {
            m_bossText.setAnimation("ok boom time.");
        }
    }, 9000);

    Util::doLater([this, goodEnding](){
        if(!goodEnding)
            m_ufo.setAnimation("boom");
    }, 10000);

    Util::doLater([this](){
        m_backgroundFade.opacityGoal = 1.0;
    }, 11000);

    // wait for cutscene to be done
    Util::doLater([&o, this]() {
        m_boss.visible = false;
        m_ufo.visible = false;
        m_starrySky.visible = false;

        m_backgroundFade.animationSpeed = 0.7;
        m_backgroundFade.opacityGoal = 0.0;

        ((GUIBackground*)o.layerPtrs["background"])->startEndSequence();
    }, 12000);


    // wait for background to be done
    Util::doLater([this]() {

        m_backgroundFade.animationSpeed = 0.1;

        m_backgroundFade.opacityGoal = 1.0;
        m_planetDown.opacityGoal = 1.0;
    }, 14150);

    Util::doLater([this, &o](){
        m_planetDown.setAnimation("planetDown");
        o.audioManager.play("planetDown");
    }, 15150);
    Util::doLater([this](){
        m_planetDown.opacityGoal = 0.0;
    }, 17150);

    Util::doLater([this](){
        m_backgroundFade.opacityGoal = 1.0;
    }, 17500);

    Util::doLater([this, &o](){
        m_creditsSequence.opacityGoal = 1.0;
        m_creditsSequence.warpToGoal();

        m_backgroundFade.moveLinearly = true;
        m_backgroundFade.setGoal(0, 2000);

        buttons.push_back(new UIButton("octopuzzlerURL", TextureManager::None, UITransform(960, 350, 900, 100), Bounds(), [](UIButton&, int){
            Util::openLink("https://2foamboards.itch.io/octopuzzler");
        }));

        buttons.push_back(new UIButton("gitURL", TextureManager::None, UITransform(560, 500, 1300, 100), Bounds(), [](UIButton&, int){
            Util::openLink("https://github.com/RealTheSunCat/Reverse-Abduction-Simulator");
        }));

        buttons.push_back(new UIButton("exit", simpleTexture({"ObjectData/UI/", "exitButton"}, GL_LINEAR), UITransform(50, 913, 126, 127), Bounds(), [](UIButton&, int){
            Outrospection::get().stop();
        }));
        buttons[buttons.size() - 1]->addAnimation("hovered", simpleTexture({"ObjectData/UI/", "exitButtonHover"}, GL_LINEAR));

        buttons[buttons.size() - 1]->onHover = [] (UIButton& b, int) {
            b.setAnimation("hovered");
        };

        buttons[buttons.size() - 1]->onUnhover = [] (UIButton& b, int) {
            b.setAnimation("default");
        };

        m_scoreValueText.visible = true;
        m_human.visible = true;
    }, 18500);

}

void GUIPostGame::setScore(int score)
{
    m_scoreValueText.text = "x " + std::to_string(score);
}


GUIPostGame::~GUIPostGame()
{

}
