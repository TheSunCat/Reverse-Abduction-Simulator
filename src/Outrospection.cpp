#include "Outrospection.h"

#include <iostream>
#include <fstream>
#include <chrono>
#include <csignal>

#include <ext/matrix_clip_space.hpp>

#ifdef USE_GLFM
#include "glfm.h"
#else
#include "GLFW/glfw3.h"
#endif

#include "Util.h"
#include "Core/Layer.h"

#include "Core/UI/GUILayer.h"
#include "Events/Event.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Events/WindowEvent.h"

#include "Core/UI/GUIBackground.h"
#include "Core/UI/GUICharacterMaker.h"
#include "Core/UI/GUIStats.h"
#include "Core/UI/GUIPeople.h"
#include "Core/UI/GUIPostGame.h"
#include "Core/UI/GUITutorial.h"

Outrospection* Outrospection::instance = nullptr;

#ifdef USE_GLFM
Outrospection::Outrospection(GLFMDisplay* display) : opengl(display, onSurfaceCreated, onSurfaceDestroyed, onFrame, onTouch)
#else
Outrospection::Outrospection()
#endif
{
    instance = this;

    LOG("Initializing engine...");

    // seed rand()
    srand(time(nullptr));

    // TODO emscripten loggerThread.start();
    // TODO consoleThread.start();
    audioManager.init({
         "pageTurn0", "pageTurn1", "pageTurn2", "pageTurn3", "pageTurn4", "explode", "explodeFinal", "end",
         "newsongfornewgame", "noo0", "noo1", "noo2", "noo3", "timesUp", "reverseAbduction", "planetDown"
    });

#ifdef USE_GLFM
    gameDisplay = display;
#else
    gameWindow = opengl.gameWindow;
#endif

    framebuffers.insert(std::make_pair("default", Framebuffer()));

    fontCharacters = freetype.loadedCharacters;

#ifndef USE_GLFM
    registerCallbacks();
    createCursors();
    createIcon();
#endif
    createShaders();

    setCursor("default");

    layerPtrs["tutorial"] = new GUITutorial();
    layerPtrs["background"] = new GUIBackground();
    layerPtrs["characterMaker"] = new GUICharacterMaker();
    layerPtrs["stats"] = new GUIStats();
    layerPtrs["people"] = new GUIPeople();
    layerPtrs["postGame"] = new GUIPostGame();

    pushOverlay(layerPtrs["tutorial"]);

    audioManager.play("newsongfornewgame", 1, true);

    // for good measure, redo UI here
    int width = 0, height = 0;
    
#ifdef USE_GLFM
    glfmGetDisplaySize(gameDisplay, &width, &height);
#else
    glfwGetFramebufferSize(gameWindow, &width, &height);
#endif

    updateResolution(width, height);

    lastFrame = Util::currentTimeMillis();
}

Outrospection::~Outrospection()
{
    LOG_INFO("Terminating engine...");

#ifndef USE_GLFM
    glfwTerminate();
#endif

    //consoleThread.stop();
    // TODO emscripten loggerThread.stop();

    std::cout << "Concluded the termination of the engine." << std::endl;
}

void Outrospection::stop()
{
#ifdef USE_GLFM
    abort(); // TODO ugly but works, since GLFM is event-driven and provides no way to stop :/
#else
    running = false;
#endif
}

void Outrospection::run()
{
    using namespace std::chrono_literals;

    running = true;

    lastFrame = Util::currentTimeMillis(); // I miss java
    deltaTime = 1.0f / 60.0f;

    // GLFM calls this by itself
#ifndef USE_GLFM
    while (running)
    {
        runGameLoop();

        if (glfwWindowShouldClose(gameWindow))
            running = false;

        currentTimeMillis = Util::currentTimeMillis();
        time_t frameTime = currentTimeMillis - lastFrame;

        // sleep for any extra time we have
        auto extraTime = 16 - frameTime;

        if(extraTime > 0) {
            auto m = std::chrono::milliseconds(extraTime - 1);
            std::this_thread::sleep_for(m);
        }
    }
#endif
}

void Outrospection::onEvent(Event& e)
{
    dispatchEvent<WindowCloseEvent>(e, BIND_EVENT_FUNC(Outrospection::onWindowClose));
    //dispatcher.dispatch<WindowResizeEvent>(BIND_EVENT_FUNC(Outrospection::onWindowResize));
    dispatchEvent<MouseMovedEvent>(e, BIND_EVENT_FUNC(Outrospection::onMouseMoved));
    dispatchEvent<MouseScrolledEvent>(e, BIND_EVENT_FUNC(Outrospection::onMouseScrolled));

    dispatchEvent<KeyPressedEvent>(e, BIND_EVENT_FUNC(Outrospection::onKeyPressed));
    dispatchEvent<KeyReleasedEvent>(e, BIND_EVENT_FUNC(Outrospection::onKeyReleased));

    for (int i = layerStack.size() - 1; i >= 0; i--)
    {
        if (e.handled)
            break;
        layerStack[i]->onEvent(e);
    }
}

void Outrospection::pushLayer(Layer* layer)
{
    layerStack.pushLayer(layer);
    layer->onAttach();
}

void Outrospection::pushOverlay(Layer* overlay)
{
    layerStack.pushOverlay(overlay);
    overlay->onAttach();
}

void Outrospection::popLayer(Layer* layer)
{
    layerStack.popLayer(layer);
    layer->onDetach();
}

void Outrospection::popOverlay(Layer* overlay)
{
    layerStack.popOverlay(overlay);
    overlay->onDetach();
}

void Outrospection::captureMouse(const bool doCapture)
{
#ifdef USE_GLFM
    LOG_ERROR("Can't capture mouse in GLFM!");
#else

    if (false) { // TODO make this not hardcoded
        glfwSetInputMode(gameWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    else
    {
        //glfwSetInputMode(gameWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        //glfwSetCursorPos(gameWindow, SCR_WIDTH / 2.0f, SCR_HEIGHT / 2.0f);
    }
#endif
}

void Outrospection::scheduleWorldTick()
{
    lastTick = Util::currentTimeMillis() - 5000;
}

#ifndef USE_GLFM
void Outrospection::toggleFullscreen()
{
    auto monitor = glfwGetPrimaryMonitor();

    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    if (isFullscreen)
    { // exit fullscreen
        int w = mode->width;
        int h = mode->height;

        int windowWidth = w / 2;
        int windowHeight = h / 2;

        glfwSetWindowMonitor(gameWindow, nullptr,
            (w - windowWidth) / 2, (h - windowHeight) / 2,
            windowWidth, windowHeight, mode->refreshRate);
        updateResolution(windowWidth, windowHeight);
    } else
    { // enter fullscreen
        
        glfwSetWindowMonitor(gameWindow, monitor, 0, 0,
            mode->width, mode->height, mode->refreshRate);
        updateResolution(mode->width, mode->height);
    }

    isFullscreen = !isFullscreen;
}
#endif

void Outrospection::runGameLoop()
{
    currentTimeMillis = Util::currentTimeMillis();
    deltaTime = float(currentTimeMillis - lastFrame) / 1000.0f;
    lastFrame = currentTimeMillis;


    // Update game world
    {
        // fetch input into simplified controller class
        updateInput();

        if (!isGamePaused)
        {
            // Run one "tick" of the game physics
            runTick();
            textureManager.tickAllTextures();

            // execute scheduled tasks
            for(int i = 0; i < futureFunctions.size(); i++)
            {
                const auto& futureFunc = futureFunctions[i];
                
                if(currentTimeMillis - futureFunc.startTime > futureFunc.waitTime)
                {
                    futureFunc.func();

                    // pop the function
                    futureFunctions.erase(futureFunctions.begin() + i);
                    i--;
                }
            }
        }
        
        // UIs are also updated when game is paused
        for (auto& layer : layerStack)
        {
            layer->tick();
        }
    }

    // Draw the frame!
    {
        glDisable(GL_DEPTH_TEST); // disable depth test so stuff near camera isn't clipped

        framebuffers["default"].bind();
        glClear(GL_COLOR_BUFFER_BIT);
        
        //glBindVertexArray(crtVAO);
        //framebuffers["crt"].bindTexture();
        //glDrawArrays(GL_TRIANGLES, 0, 6);

        shaders["screen"].use();
        // draw UI
        for (const auto& layer : layerStack)
        {
            if (layer->handleManually) // TODO this is jank
                continue;
            
            layer->draw();
        }
    }

    // check for errors
    Util::glError();

    // swap buffers and poll IO events
    // -------------------------------
#ifdef USE_GLFM
    glfmSwapBuffers(gameDisplay);
#else
    glfwSwapBuffers(gameWindow);
    glfwPollEvents();
#endif
}

void Outrospection::runTick()
{
    if (currentTimeMillis - lastTick < 200) // five ticks per second
        return;
    
    lastTick = currentTimeMillis;

    //((GUIScene*)scene)->worldTick();
}

#ifdef USE_GLFM
void Outrospection::onFrame(GLFMDisplay* display)
{
    Outrospection::get().runGameLoop();
}

void Outrospection::onSurfaceCreated(GLFMDisplay* display, int width, int height)
{
    Outrospection::get().updateResolution(width, height);
}

void Outrospection::onSurfaceDestroyed(GLFMDisplay* display)
{
    WindowCloseEvent event;
    Outrospection::get().onEvent(event);
}

bool Outrospection::onTouch(GLFMDisplay* display, int touch, GLFMTouchPhase phase, double x, double y)
{
    glm::ivec2 windowRes = Outrospection::get().getWindowResolution();
    float targetAspectRatio = 1920 / 1080.f;

    float width = windowRes.x;
    float height = (width / targetAspectRatio + 0.5f);

    if (height > windowRes.y) // pillarbox
    {
        height = windowRes.y;
        width = (height * targetAspectRatio + 0.5f);
    }

    // center
    float xPos = float(x) - (windowRes.x - width) / 2;
    float yPos = float(y) - (windowRes.y - height) / 2;

    float scaleFactor = width / 1920.f;
    float scaledX = xPos * (1/scaleFactor);
    float scaledY = yPos * (1/scaleFactor);
    
    {
        MouseMovedEvent event(scaledX, scaledY);
        Outrospection::get().onEvent(event);
    }

    switch (phase)
    {
    case GLFMTouchPhaseMoved:
    case GLFMTouchPhaseHover:
    {
        return false;
    }
    case GLFMTouchPhaseBegan:
    {
        MouseButtonPressedEvent event(0);
        Outrospection::get().onEvent(event);
        return true;
    }
    case GLFMTouchPhaseCancelled:
    case GLFMTouchPhaseEnded:
    {
        MouseButtonReleasedEvent event(0);
        Outrospection::get().onEvent(event);

        // move the "mouse" out of the way so buttons unhover
        MouseMovedEvent event2(-9999, -9999);
        Outrospection::get().onEvent(event2);
        return true;
    }
    }

    return false;
}
#else
void Outrospection::registerCallbacks() const
{
    // Register OpenGL events
    glfwSetFramebufferSizeCallback(gameWindow, [](GLFWwindow*, const int width, const int height)
    {
        Outrospection::get().updateResolution(width, height);
    });

    glfwSetCursorPosCallback(gameWindow, [](GLFWwindow* window, const double xPosD, const double yPosD)
    {
        // support for HiDPI
        float xDPI = 0, yDPI = 0;
        glfwGetWindowContentScale(window, &xDPI, &yDPI);


        glm::ivec2 windowRes = Outrospection::get().getWindowResolution();
        float targetAspectRatio = 1920 / 1080.f;

        float width = windowRes.x;
        float height = (width / targetAspectRatio + 0.5f);

        if (height > windowRes.y) // pillarbox
        {
            height = windowRes.y;
            width = (height * targetAspectRatio + 0.5f);
        }

#ifdef PLATFORM_MACOS
        // weird center on apple because what
        float xPos = float(xPosD) - (windowRes.x - width) / (2 * xDPI);
        float yPos = float(yPosD) - (windowRes.y - height) / (2 * yDPI);
#else
        // center
        float xPos = float(xPosD) - (windowRes.x - width) / 2;
        float yPos = float(yPosD) - (windowRes.y - height) / 2;
#endif

        float scaleFactor = width / 1920.f;
        float scaledX = xPos * (1/scaleFactor);
        float scaledY = yPos * (1/scaleFactor);

#ifdef PLATFORM_MACOS
        scaledX *= xDPI;
        scaledY *= yDPI;
#endif

        MouseMovedEvent event(scaledX, scaledY);
        Outrospection::get().onEvent(event);
    });

    glfwSetMouseButtonCallback(gameWindow, [](GLFWwindow* window, const int button, const int action, const int mods)
    {
        switch (action)
        {
        case GLFW_PRESS:
            {
                MouseButtonPressedEvent event(button);
                Outrospection::get().onEvent(event);
                break;
            }
        case GLFW_RELEASE:
            {
                MouseButtonReleasedEvent event(button);
                Outrospection::get().onEvent(event);
                break;
            }
        }
    });

    glfwSetScrollCallback(gameWindow, [](GLFWwindow* window, const double xDelta, const double yDelta)
    {
        MouseScrolledEvent event(xDelta, yDelta);
        Outrospection::get().onEvent(event);
    });

    glfwSetKeyCallback(gameWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if(action == GLFW_PRESS || action == GLFW_REPEAT)
        {
            KeyPressedEvent event(key, 0);
            Outrospection::get().onEvent(event);
        } else if (action == GLFW_RELEASE)
        {
            KeyReleasedEvent event(key);
            Outrospection::get().onEvent(event);
        }
    });
    glfwSetErrorCallback(error_callback);
}
#endif

void Outrospection::createShaders()
{
    LOG_INFO("Setting up shaders...");

    shaders.insert(std::make_pair("screen", Shader("screen", "screen")));
    shaders.insert(std::make_pair("sprite", Shader("sprite", "sprite")));
    shaders.insert(std::make_pair("glyph",  Shader("sprite", "glyph" )));
}

#ifndef USE_GLFM
void Outrospection::createCursors()
{
    GLFWimage cursorImage;
    int width = 10, height = 10, nrComponents = 4;

    unsigned char* data = TextureManager::readImageBytes({"ObjectData/Textures/", "mouse", "png"}, width, height, nrComponents);
    cursorImage.pixels = data; cursorImage.width = width; cursorImage.height = height;
    cursors["default"] = glfwCreateCursor(&cursorImage, 0, 0);
    TextureManager::free(data);

    data = TextureManager::readImageBytes({"ObjectData/Textures/", "mouse_hover", "png"}, width, height, nrComponents);
    cursorImage.pixels = data; cursorImage.width = width; cursorImage.height = height;
    cursors["hovering"] = glfwCreateCursor(&cursorImage, 0, 0);
    TextureManager::free(data);

    data = TextureManager::readImageBytes({"ObjectData/Textures/", "mouse_click", "png"}, width, height, nrComponents);
    cursorImage.pixels = data; cursorImage.width = width; cursorImage.height = height;
    cursors["clicking"] = glfwCreateCursor(&cursorImage, 0, 0);
    TextureManager::free(data);
}

void Outrospection::createIcon() const
{
    GLFWimage image;
    int width = 10, height = 10, nrComponents = 4;

    unsigned char* data = TextureManager::readImageBytes({"ObjectData/", "icon", "png"}, width, height, nrComponents);
    image.pixels = data; image.width = width; image.height = height;

    glfwSetWindowIcon(gameWindow, 1, &image);

    TextureManager::free(data);
}
#endif

void Outrospection::setResolution(glm::vec2 res)
{
    updateResolution(res.x, res.y);
}

void Outrospection::updateResolution(int x, int y)
{
    curWindowResolution = glm::ivec2(x, y);

    LOG_INFO("updateResolution(%i, %i)", x, y);
}

glm::vec2 Outrospection::getWindowResolution() const
{
    return glm::vec2(curWindowResolution);
}

void Outrospection::setWindowText(const std::string& text) const
{
#ifdef USE_GLFM
    LOG_ERROR("Can't set window title in GLFM!");
#else
    glfwSetWindowTitle(gameWindow, ("Outrospection Engine | " + text).c_str());
#endif
}

void Outrospection::setCursor(const std::string& cursorName)
{
#ifdef USE_GLFM
    LOG_ERROR("Can't set cursor in GLFM!");
#else
    glfwSetCursor(gameWindow, cursors[cursorName]);
#endif
}

bool Outrospection::onWindowClose(WindowCloseEvent& e)
{
    running = false;

    return true;
}

bool Outrospection::onMouseMoved(MouseMovedEvent& e)
{
    const auto xPos = float(e.getX());
    const auto yPos = float(e.getY());

    if (firstMouse)
    {
        lastMousePos.x = xPos;
        lastMousePos.y = yPos;
        firstMouse = false;
        return true; // nothing to calculate because we technically didn't move the mouse
    }

    const float xOffset = xPos - lastMousePos.x;
    const float yOffset = lastMousePos.y - yPos;

    lastMousePos.x = xPos;
    lastMousePos.y = yPos;

    return false;
}

bool Outrospection::onMouseScrolled(MouseScrolledEvent& e)
{
    return false;
}

bool Outrospection::onKeyPressed(KeyPressedEvent& e)
{
    LOG_INFO("pressed key %i", e.getKeyCode());

#ifndef USE_GLFM
    switch(e.getKeyCode())
    {
#ifdef _DEBUG
    case GLFW_KEY_ESCAPE:
        Outrospection::get().running = false;
        return true;
#endif
    case GLFW_KEY_F11:
        Outrospection::get().toggleFullscreen();
        return true;
    }
#endif

    return false;
}

bool Outrospection::onKeyReleased(KeyReleasedEvent& e)
{
    return false;
}

void Outrospection::error_callback(const int errorcode, const char* description)
{
    LOG_ERROR("GLFW error (%i): %s", errorcode, description);
}

void Outrospection::updateInput()
{
    
}
/* TODO
int Outrospection::loadSave()
{
#ifdef _DEBUG // don't load save data on Debug mode
    return 0;
#endif

    if(Util::fileExists("save"))
    {
        std::string saveData = Util::readAllBytes("save");

        if(!saveData.starts_with("OSAV"))
        {
            LOG_ERROR("Save file signature invalid!");
            return 0;
        }

        saveData = saveData.substr(4);
        return Util::stoi(saveData);
    }

    return 0;
}

void Outrospection::writeSave(int number)
{
#ifdef _DEBUG
    return;
#endif

    if(Util::fileExists("save"))
    {
        std::remove("save");
    }

    std::ofstream saveFile("save");
    saveFile << "OSAV" << number;
    saveFile.close();
}
*/
