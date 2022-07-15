#include "UIHuman.h"

UIHuman::UIHuman(const UITransform& transform) : UIComponent("Human base", TextureManager::None, transform)
{

}

void UIHuman::setLayer(HumanLayer name, SimpleTexture* layer)
{
    m_layers[int(name)] = layer;
}

void UIHuman::addLayer(HumanLayer name, const std::string& layer)
{
    assert(m_layers[int(name)] == nullptr);

    setLayer(name, &simpleTexture({"CostumeData/", layer}, GL_LINEAR));
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


    for(SimpleTexture* layer : m_layers)
    {
        if(!layer)
            continue;

        LOG_DEBUG("Drawing layer %s", layer.c_str());
        layer->bind();

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    glBindVertexArray(0);
}
