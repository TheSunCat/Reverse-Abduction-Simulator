cmake_minimum_required(VERSION 3.20)

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

find_package(freetype CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

# include
if (CMAKE_SYSTEM_NAME MATCHES "Android")
    include_directories(lib/glfm/include)
else()
    include_directories(lib/glad/include)
endif()

include_directories(lib/soloud/include src)
include_directories(lib/json)


# source files
if (CMAKE_SYSTEM_NAME MATCHES "Android")
    add_subdirectory(lib/glfm)
else()
    add_subdirectory(lib/glad)
endif()

set(SOLOUD_BACKEND_SDL2 OFF CACHE BOOL "Set to ON for building SDL2 backend")
add_subdirectory(lib/soloud/contrib)


# executable
file(GLOB_RECURSE DIR_SRC src/*.cpp)

if (CMAKE_SYSTEM_NAME MATCHES "Android")
    add_library(Outrospection SHARED ${DIR_SRC})

    set(PLATFORM_DEPS "glfm")
else()
    set(CMAKE_EXE_LINKER_FLAGS -static)
    add_executable(Outrospection ${DIR_SRC})
    
    set(PLATFORM_DEPS "glad glfw")
endif()



target_link_libraries(Outrospection
    PUBLIC
    glad
    glfw

    PRIVATE
    freetype soloud
    ${CMAKE_DL_LIBS})
