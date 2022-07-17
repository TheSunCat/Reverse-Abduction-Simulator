#pragma once
#include <string>

#include <glm/vec2.hpp>

#include "Outrospection.h"
#include "Core/Rendering/SimpleTexture.h"
#include "Core/Rendering/TextureManager.h"

class Shader;

enum class UIAlign
{
    CENTER,
    TOP_LEFT,
    TOP_RIGHT,
    BOT_LEFT,
    BOT_RIGHT,
};

class UITransform
{
    glm::vec2 defaultRes = glm::vec2(1920, 1080);

    glm::vec2 pos;
    glm::vec2 size;
public:
    // units are in 1080p pixels
    UITransform(int posX, int posY, int sizeX, int sizeY,
                const glm::vec2& fbRes = {1920, 1080}, UIAlign _alignment = UIAlign::TOP_LEFT);

    // convenience function for Bounds ctor. Make sure to specify BoundsShape::Circle!
    UITransform(int posX, int posY, int radius,
                const glm::vec2& fbRes = {1920, 1080}, UIAlign _alignment = UIAlign::TOP_LEFT);

    glm::vec2 getPos() const;
    glm::vec2 getSize() const;
    glm::vec2 getSizeRatio() const;

    void setPos(glm::vec2 _pos);
    void setPos(int x, int y);
    void setSize(int x, int y);
    UIAlign alignment;
};

class UIComponent
{
public:
    UIComponent(const std::string& _texName, const GLint& texFilter, const UITransform& transform);
    UIComponent(std::string _name, SimpleTexture& _tex, const UITransform& transform);

    virtual void draw(Shader& shader = Outrospection::get().shaders["sprite"], const Shader& glyphShader = Outrospection::get().shaders["glyph"]) const;

    virtual void tick();

    void addAnimation(const std::string& anim, SimpleTexture& _tex);
    void setAnimation(const std::string& anim);

    void setPosition(int x, int y);
    void setScale(int px);
    void setScale(int x, int y);

    void setGoal(int x, int y);
    bool hasGoal();
    void warpToGoal();

    std::string text;

    float textSize = 0;
    Color textColor;
    bool textShadow = false;

    bool visible = true;
    bool flip = false;

    // gimme a break this is speed-coding
    bool bobUpAndDown = false;

    bool moveLinearly = false;
    float animationSpeed = 0;
    float opacityGoal = 1.0;

    virtual ~UIComponent() = default;
protected:
    UITransform transform;

    static GLuint quadVAO;

    virtual void drawText(const std::string& text, const Shader& glyphShader) const;

    std::string curAnimation = "default";
    std::unordered_map<std::string, SimpleTexture*> animations;

    glm::vec2 m_goal = glm::vec2(0);

    float opacity = 1.0;
};
