#include "Outrospection.h"

static void onReady(GLFMDisplay *display, int width, int height) {
    std::cout << "Time to leak some memory!" << std::endl;

    Outrospection* o = new Outrospection(display);
    // leak that memory!!
}

#ifdef USE_GLFM
void glfmMain(GLFMDisplay *display) {
    //int argc = 0;
    //char** argv = "Outrospection";


#else // #ifdef USE_GLFM

// ugly Windows code so that we don't open a cmd window along the program, but can still see output if we start from cmd
#ifdef PLATFORM_WINDOWS
bool haveConsole = false;

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {

#ifdef _DEBUG
    if (AllocConsole()) {
#else
    if (AttachConsole(ATTACH_PARENT_PROCESS)) { // set up console output (if there is a console to attach to)
#endif // #ifdef _DEBUG
        
        FILE* empty;
        freopen_s(&empty, "CONOUT$", "w", stdout);
        freopen_s(&empty, "CONOUT$", "w", stderr);
        haveConsole = true;
    }

    int argc = __argc;
    auto argv = __argv;
#else // #ifdef PLATFORM_WINDOWS

int main(int argc, char** argv) {
#endif // #ifdef PLATFORM_WINDOWS

#endif // #ifdef USE_GLFM

    // check if we can read the filesystem
    bool canReadFiles = Util::fileExists("ShaderData/sprite.vert");
    if(!canReadFiles)
    {
        std::string err = "Can't access \"res\" folder! Make sure you:\n- run the game from its directory\n- see the \"res\" folder next to the game\n- have unzipped it fully";

        std::cout << err << std::endl;

        std::vector<std::string> files = Util::listFiles("res");
        std::cout << "can see: ";
        for(std::string& s : files)
            std::cout << s << ", ";

        std::cout << std::endl;

#ifndef PLATFORM_WINDOWS
        std::cin.get();
#else
        if(haveConsole)
            std::cin.get();
        else // show error dialog
        {
            MessageBox(nullptr, err.c_str(), "Unable to start Outrospection Engine!", MB_OK);
        }
#endif // #ifndef PLATFORM_WINDOWS

#ifdef USE_GLFM
        return;
#else
        return -1;
#endif
    }

#ifdef USE_GLFM
    glfmSetDisplayConfig(display,
                         GLFMRenderingAPIOpenGLES3,
                         GLFMColorFormatRGBA8888,
                         GLFMDepthFormatNone,
                         GLFMStencilFormatNone,
                         GLFMMultisampleNone);

    glfmSetSurfaceCreatedFunc(display, onReady);

#else
    auto outrospection = Outrospection();

    // run the game!
    outrospection.run();

    return 0;
#endif
}
