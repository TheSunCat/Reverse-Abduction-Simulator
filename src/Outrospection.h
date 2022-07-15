#pragma once

#include "Core.h"

#include <glm/vec2.hpp>

#include "Types.h"
#include "Constants.h"

#include "KeyBinding.h"
#include "Util.h"
#include "Core/jthread.h"
#include "Core/LayerStack.h"
#include "Core/Registry.h"
#include "Core/AudioManager.h"
#include "Core/Rendering/FreeType.h"
#include "Core/Rendering/Framebuffer.h"
#include "Core/Rendering/OpenGL.h"
#include "Core/Rendering/Shader.h"
#include "Core/Rendering/TextureManager.h"
#include "Core/UI/GUILayer.h"


class MouseMovedEvent;
class WindowCloseEvent;
class MouseScrolledEvent;
class KeyPressedEvent;
class KeyReleasedEvent;

class Event;
class Layer;
class GUILayer;

class Outrospection
{

    OpenGL opengl; // defined at the beginning so nothing gets initialized before this
    FreeType freetype;

    glm::ivec2 curWindowResolution = glm::ivec2(1920, 1080);
public:
    static Outrospection& get()
    {
        return *instance;
    }

    Outrospection();
    ~Outrospection();

    void stop();

    void run();
    void onEvent(Event& e);

    void pushLayer(Layer* layer);
    void pushOverlay(Layer* overlay);

    void popLayer(Layer* layer);
    void popOverlay(Layer* overlay);

    void captureMouse(bool doCapture);

    void scheduleWorldTick(); // tick world NOW

    void toggleFullscreen();

    void setResolution(glm::vec2 res);
    void updateResolution(int x, int y);
    glm::vec2 getWindowResolution() const;

    void setWindowText(const std::string& text) const;

    glm::ivec2* curFbResolution = &curWindowResolution;

    glm::vec2 lastMousePos = glm::vec2(curWindowResolution / 2);

    TextureManager textureManager;
    AudioManager audioManager;

	std::vector<Util::FutureRun> futureFunctions;
    std::unordered_map<char, FontCharacter> fontCharacters;
    
    std::unordered_map<std::string, Shader> shaders;

    GUILayer* background;
    GUILayer* characterMaker;

    bool won = false;

    time_t currentTimeMillis = 0;
	
    DISALLOW_COPY_AND_ASSIGN(Outrospection)
private:
    void runGameLoop();
    void runTick();
    time_t lastTick = 0;

    // set to false when the game loop shouldn't run
    bool running = false;

    // timing
    float deltaTime = 0;    // time between current frame and last frame
    time_t lastFrame = 0;   // time of last frame

    GLFWwindow* gameWindow;
    bool isFullscreen = false;

    std::unordered_map<std::string, Framebuffer> framebuffers;
    GLuint crtVAO;

    // camera stuff
    //Camera camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
    bool firstMouse = true;


    bool onWindowClose(WindowCloseEvent& e);
    bool onMouseMoved(MouseMovedEvent& e);
    bool onMouseScrolled(MouseScrolledEvent& e);
    bool onKeyPressed(KeyPressedEvent& e);
    bool onKeyReleased(KeyReleasedEvent& e);

    static void error_callback(int errorcode, const char* description);
	
    void registerCallbacks() const;
    void createShaders();
    void createCursors();
    void createIcon() const;
	
    GLFWcursor *gameCursor{};
	
    void updateInput();

    bool isGamePaused = false;

    LayerStack layerStack;

    static Outrospection* instance;

    jthread loggerThread{
        [&] {
            using namespace std::chrono_literals;

            while (!loggerQueue.empty())
            {
                const auto& log = loggerQueue.pop();
                if (log != nullptr)
                {
                    log();
                    std::putchar('\n');
                }
            }

            std::this_thread::sleep_for(1ms);
        }
    };

public:
    static int loadSave();
    static void writeSave(int number);
};
