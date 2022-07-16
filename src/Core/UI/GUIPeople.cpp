#include "GUIPeople.h"

#include "UIHuman.h"
#include "UIButton.h"
#include "GUIStats.h"

GUIPeople::GUIPeople() : GUILayer("People renderer", false)
{

}

GUIPeople::~GUIPeople()
{

}


void GUIPeople::tick()
{
    for(auto& button : buttons)
    {
        button->tick();
    }

    for(int i = 0; i < m_people.size(); i++)
    {
        UIHuman& human = m_people[i];

        if(!human.hasGoal()) {

            // random chance to assign a new goal
            float random = rand() / float(RAND_MAX);

            if(random >= 0.995) {

                float r = 300 * sqrt(rand() / float(RAND_MAX));
                float theta = (rand() / float(RAND_MAX)) * 2 * M_PI;

                glm::vec2 goal(1100 + 440 - 96, 20 + 440 - 135);
                goal.x += r * cos(theta);
                goal.y += r * sin(theta);

                human.setGoal(goal.x, goal.y);

            }
        }

        human.tick();
    }
}

void GUIPeople::draw() const
{
    for(auto& button : buttons)
    {
        button->draw();
    }

    for(auto& human : m_people)
    {
        human.draw();
    }
}


void GUIPeople::addHuman(UIHuman human)
{
    human.setScale(192, 270);
    human.setPosition(1050, 80);

    human.setGoal(1550, 520);
    m_people.emplace_back(human);

    if(human.isBad()) {
        /*for(int i = 0; i < m_people.size() - 1; i++) // exclude the new human that is bad
        {
            float random = rand() / float(RAND_MAX);
            if(random > 0.75)
            {
                UIHuman& curHuman = m_people[i];

                Util::doLater([&curHuman, &i, this]()
                {
                    curHuman.setScale(200, 200);
                    curHuman.setAnimation("exploding");
                    Outrospection::get().audioManager.play("explode", 0.5);

                    Util::doLater([&i, this]() { this->m_people.erase(this->m_people.begin() + i); }, 300);
                }, random * 1000 + 1000);
            }
        }*/

        Util::doLater([&human]()
        {
            human.setScale(200, 200);
            human.setAnimation("exploding");
            //Outrospection::get().audioManager.play("explode", 0.5);
        }, 2000);

        // delete bad human
        //Util::doLater([&, this]() { std::remove(this->m_people.begin(), this->m_people.end(), human); }, 2300);
    } else {
        ((GUIStats*) Outrospection::get().layerPtrs["stats"])->setPeopleCount(m_people.size());
    }
}
