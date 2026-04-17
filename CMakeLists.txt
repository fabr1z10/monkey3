cmake_minimum_required(VERSION 3.15)

project(monkey)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(FetchContent)

# ----------------------------------------------------
# Catch2 (for tests)
# ----------------------------------------------------
FetchContent_Declare(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.12.0
)

FetchContent_MakeAvailable(catch2)


# --- Dependencies ---
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
#find_package(PNG REQUIRED)
find_package(yaml-cpp REQUIRED)

# ----------------------------------------------------
# 1️⃣ Build the C++ core library (engine only)
# ----------------------------------------------------
file(GLOB ENGINE_SOURCES CONFIGURE_DEPENDS core/src/*.cpp)

add_library(monkey_core STATIC ${ENGINE_SOURCES})

target_include_directories(monkey_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/core/include
        ${CMAKE_CURRENT_SOURCE_DIR}/core/external/glm ${CMAKE_CURRENT_SOURCE_DIR}/core/external/stb)

target_link_libraries(monkey_core PUBLIC ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} glfw ${PNG_LIBRARIES} ${YAML_CPP_LIBRARIES})

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

# ----------------------------------------------------
# 2️⃣ Demos
# ----------------------------------------------------
file(GLOB DEMO_SOURCES CONFIGURE_DEPENDS demos/*.cpp)
message(STATUS "DEMO_SOURCES = ${DEMO_SOURCES}")

foreach(demo ${DEMO_SOURCES})
    get_filename_component(demo_name ${demo} NAME_WE)

    add_executable(${demo_name} ${demo})

    target_link_libraries(${demo_name}
            PRIVATE monkey_core
    )
endforeach()