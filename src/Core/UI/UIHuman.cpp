#include "UIHuman.h"

UIHuman::UIHuman(const UITransform& transform) : UIComponent("Human base", TextureManager::None, transform)
{

}

void UIHuman::addToLayer(HumanLayer name, SimpleTexture* texture, bool bad)
{
    m_layers[int(name)].push_back(texture);
    m_layerBad[int(name)].push_back(bad);
}

void UIHuman::addToLayer(HumanLayer name, const std::string& textureName, bool bad)
{
    addToLayer(name, &simpleTexture({"CostumeData/", textureName}, GL_LINEAR), bad);
}

void UIHuman::draw(Shader& shader, const Shader&) const
{
    if (!visible)
        return;

    shader.use();

    // TODO maybe we should cache this
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(transform.getPos(), 0));
    model = glm::scale(model, glm::vec3(transform.getSize(), 0));

    shader.setMat4("model", model);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(quadVAO);


    if(curAnimation == "default") {
        for(int i = 0; i < m_layers.size(); i++)
        {
            SimpleTexture* layer = m_layers[i].operator[](m_curLayer[i]);

            if(!layer)
                continue;

            LOG_DEBUG("Drawing layer %s", layer.c_str());
            layer->bind();

            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    } else {
        animations.at(curAnimation)->bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    glBindVertexArray(0);
}

void UIHuman::tick()
{
    if(glm::length(m_goal) != 0 && transform.getPos() != m_goal) {
        //LOG("LERPING HUMAN");

        transform.setPos(Util::lerp(transform.getPos(), m_goal, 0.01));
    }
}


void UIHuman::changeLayer(HumanLayer layer, int delta)
{
    m_curLayer[int(layer)] += delta;

    // wrap around
    if(m_curLayer[int(layer)] < 0)
        m_curLayer[int(layer)] = m_layers[int(layer)].size() - 1;

    if(m_curLayer[int(layer)] >= m_layers[int(layer)].size())
        m_curLayer[int(layer)] = 0;
}

void UIHuman::rollTheDice()
{
    for(int i = 0; i < m_layers.size(); i++) {
        float random = rand() / float(RAND_MAX) * m_layers[i].size();

        m_curLayer[i] = random;
    }
}


void UIHuman::setGoal(int x, int y)
{
    m_goal = glm::vec2(x, y);
}

bool UIHuman::hasGoal()
{
    return (1 - abs(glm::dot(m_goal, transform.getPos()))) > 0.1;
}

void UIHuman::warpToGoal()
{
    transform.setPos(m_goal);
}

bool UIHuman::isBad()
{
    for(int i = 0; i < m_layers.size(); i++) {
        if(m_layerBad[i][m_curLayer[i]])
            return true;
    }

    return false;
}
