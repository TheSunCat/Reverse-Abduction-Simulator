#include "Outrospection.h"

#ifdef USE_GLFM
static void onReady(GLFMDisplay *display, int width, int height) {
    Util::Timer timer("Engine initialization timer");

    LOG("We're ready! Time to leak some memory >:)");

    Outrospection* o = new Outrospection(display);
    //o->run();
    // leak that memory!!
}

void glfmMain(GLFMDisplay *display) {
    LOG("glfmMain executing!");

#else // #ifndef USE_GLFM

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
#else // #ifndef PLATFORM_WINDOWS

int main(int argc, char** argv) {
#endif // #ifdef PLATFORM_WINDOWS

#endif // #ifdef USE_GLFM

    // check if we can read the filesystem
    File testFile = File({"ShaderData/", "sprite", "vert"});

    bool canReadFiles = testFile.exists();
    if(!canReadFiles)
    {
        std::string err = "Can't access \"res\" folder! Make sure you:\n- run the game from its directory\n- see the \"res\" folder next to the game\n- have unzipped it fully";

#ifdef PLATFORM_ANDROID
        __android_log_print(ANDROID_LOG_INFO, "lclclc", err.c_str());
#else
        std::cout << err << std::endl;
#endif
        File resDir = File({"res"});

        std::vector<std::string> files = resDir.listFiles();
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
    Outrospection::get().run();

    return 0;
#endif
}
