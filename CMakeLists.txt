cmake_minimum_required(VERSION 3.20)

if(EMSCRIPTEN OR CMAKE_SYSTEM_NAME MATCHES "Android")
    set(GLFM "1")
    message("Setting GLFM to true...")
endif()

message(CMAKE_SYSTEM_NAME="${CMAKE_SYSTEM_NAME}")
message(EMSCRIPTEN="${EMSCRIPTEN}")

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
    find_package(freetype CONFIG REQUIRED)
    find_package(glfw3 3.3 REQUIRED)
    find_package(OpenGL REQUIRED)
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
    #set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    #set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    #set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    #add_subdirectory(lib/glfw)
    
    add_subdirectory(lib/glad)
endif()

set(SOLOUD_BACKEND_SDL2 OFF CACHE BOOL "Set to ON for building SDL2 backend")
add_compile_definitions(WITH_MINIAUDIO)
add_subdirectory(lib/soloud/contrib)

add_subdirectory(lib/glm)


# executable
file(GLOB_RECURSE DIR_SRC src/*.cpp)

if(GLFM)
    SET(CMAKE_EXECUTABLE_SUFFIX ".html")
    add_executable(Outrospection ${DIR_SRC})

    set_target_properties(Outrospection PROPERTIES LINK_FLAGS "-s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2 -s FULL_ES2=1 -s ASSERTIONS -s GL_ASSERTIONS -s GL_DEBUG -s DEMANGLE_SUPPORT=1 -s OFFSCREEN_FRAMEBUFFER=1 -s ALLOW_MEMORY_GROWTH -s EXIT_RUNTIME=1 --preload-file res@res")

    set(PLATFORM_DEPS "glfm")
else()
    set(CMAKE_EXE_LINKER_FLAGS) # -static
    add_executable(Outrospection ${DIR_SRC})
    
    set(PLATFORM_DEPS "glad glfw OpenGL::GL")
endif()

set_target_properties(Outrospection PROPERTIES COMPILE_FLAGS -Wno-format-security)

# symlink res folder
add_custom_command(TARGET Outrospection PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E create_symlink
                   ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)

target_link_libraries(Outrospection
    PRIVATE
    freetype soloud
    ${PLATFORM_DEPS}
    ${CMAKE_DL_LIBS}
)
