cmake_minimum_required(VERSION 3.18)

set(CMAKE_CXX_FLAGS_DEBUG  "-g")

if(EMSCRIPTEN OR CMAKE_SYSTEM_NAME MATCHES "Android")
    set(GLFM "1")
    message("Setting GLFM to true...")
endif()

message(CMAKE_SYSTEM_NAME="${CMAKE_SYSTEM_NAME}")
#set(CMAKE_VERBOSE_MAKEFILE on)

# add vcpkg
include(FetchContent)
FetchContent_Declare(
    vcpkg
    GIT_REPOSITORY "https://github.com/microsoft/vcpkg.git"
    GIT_TAG "master"
)
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    FetchContent_Populate(vcpkg)
    set(CMAKE_TOOLCHAIN_FILE "${vcpkg_SOURCE_DIR}/scripts/buildsystems/vcpkg.cmake")
endif()

project(Outrospection)

set(CMAKE_CXX_STANDARD 20)

if(${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    add_compile_options(-sUSE_FREETYPE=1)   
else()
    message("Finding packages for desktop...")

    add_subdirectory(lib/freetype)

    #find_package(freetype CONFIG REQUIRED)
    #find_package(OpenGL REQUIRED)
endif()

# include
if (GLFM)
    include_directories(lib/glfm/include)
else()
    include_directories(lib/glad/include)
endif()

include_directories(lib/soloud/include)
include_directories(lib/json)
include_directories(lib/glm/glm)

include_directories(src)


# source files
if(GLFM)
    add_compile_definitions(GLFM_INCLUDE_ES3 USE_GLFM)
    add_subdirectory(lib/glfm)
else()
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    add_subdirectory(lib/glfw)
    
    add_subdirectory(lib/glad)
endif()

set(SOLOUD_BACKEND_SDL2 OFF CACHE BOOL "Set to ON for building SDL2 backend")
add_compile_definitions(WITH_MINIAUDIO)
add_subdirectory(lib/soloud/contrib)

add_subdirectory(lib/glm)

# executable
file(GLOB_RECURSE DIR_SRC src/*.cpp)

if(EMSCRIPTEN)
    SET(CMAKE_EXECUTABLE_SUFFIX ".html")
    
    # hack stolen from GLFM build script
    # modify shell_minimal.html to make canvas fill the screen
    file(READ ${EMSCRIPTEN_ROOT_PATH}/src/shell_minimal.html EMSCRIPTEN_SHELL_HTML)
    string(FIND "${EMSCRIPTEN_SHELL_HTML}" "<style>" HAS_STYLE)
    if (${HAS_STYLE} EQUAL -1)
        message(WARNING "<style> not found in shell_minimal.html, copying as-is")
    else()
        string(CONCAT STYLE_REPLACEMENT "<meta name=\"viewport\" content=\"width=device-width,user-scalable=no,viewport-fit=cover\">\n"
            "    <meta name=\"apple-mobile-web-app-capable\" content=\"yes\">\n"
            "    <style>\n"
            "      /* GLFM: Start changes */\n"
            "      :root {\n"
            "          --glfm-chrome-top-old: constant(safe-area-inset-top);\n"
            "          --glfm-chrome-right-old: constant(safe-area-inset-right);\n"
            "          --glfm-chrome-bottom-old: constant(safe-area-inset-bottom);\n"
            "          --glfm-chrome-left-old: constant(safe-area-inset-left);\n"
            "          --glfm-chrome-top: env(safe-area-inset-top);\n"
            "          --glfm-chrome-right: env(safe-area-inset-right);\n"
            "          --glfm-chrome-bottom: env(safe-area-inset-bottom);\n"
            "          --glfm-chrome-left: env(safe-area-inset-left);\n"
            "      }\n"
            "      body, html { border: 0px none; padding: 0px; margin: 0px; width: 100%; height: 100%; overflow: hidden; position: fixed; }\n"
            "      canvas.emscripten { background: black; width: 100%; height: 100%; }\n"
            "      .emscripten_border { width: 100%; height: 100%; border: 0px none !important;}\n"
            "      hr { display: none; }\n"
            "      /* GLFM: End changes */"
        )
        string(REPLACE "<style>" "${STYLE_REPLACEMENT}" EMSCRIPTEN_SHELL_HTML "${EMSCRIPTEN_SHELL_HTML}")
    endif()
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/shell.html.in "${EMSCRIPTEN_SHELL_HTML}")

    add_executable(Outrospection ${DIR_SRC})

    set_target_properties(Outrospection PROPERTIES LINK_FLAGS "-s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2 -s FULL_ES2=1 -s ASSERTIONS -s GL_ASSERTIONS -s GL_DEBUG -s DEMANGLE_SUPPORT=1 -s OFFSCREEN_FRAMEBUFFER=1 -s ALLOW_MEMORY_GROWTH -s EXIT_RUNTIME=1 --preload-file res@res --shell-file ${CMAKE_CURRENT_BINARY_DIR}/shell.html.in")

elseif(CMAKE_SYSTEM_NAME MATCHES "Android")
    add_library(Outrospection SHARED ${DIR_SRC})
endif() # TODO iOS

if(GLFM)
    set(PLATFORM_DEPS "glfm")

    if(CMAKE_SYSTEM_NAME MATCHES "Android")
        set(PLATFORM_DEPS_2_ELECTRIC_BOOGALOO "GLESv3")
        set(PLATFORM_DEPS_3_ELECTRIC_BOOGALOO "log")
    endif()

else()
    set(CMAKE_EXE_LINKER_FLAGS) # -static
    add_executable(Outrospection ${DIR_SRC})

    set(PLATFORM_DEPS "glfw")
    set(PLATFORM_DEPS_2_ELECTRIC_BOOGALOO "glad")
endif()

set_target_properties(Outrospection PROPERTIES COMPILE_FLAGS "-fPIC -Wno-format-security")

# symlink res folder
add_custom_command(TARGET Outrospection PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E create_symlink
                   ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)

find_library(GLESv3-lib GLESv3)

# for some reason this fails if I don't put GLFW and GLAD in separate lines. WHAT.
target_link_libraries(Outrospection

    PRIVATE
    freetype soloud
    
    PUBLIC
    ${PLATFORM_DEPS}
    ${PLATFORM_DEPS_2_ELECTRIC_BOOGALOO}
    ${PLATFORM_DEPS_3_ELECTRIC_BOOGALOO}
    
    PRIVATE
    ${CMAKE_DL_LIBS}
)
