# ! many lines commented out here, included for demo of full codebase release
cmake_minimum_required(VERSION 3.20)
project(hobgoblin CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# MSVC warnings & runtime
if(MSVC)
    add_compile_options(/W4 /permissive-)
endif()

include(FetchContent)

# GLFW
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)

# spdlog
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.13.0
)
FetchContent_MakeAvailable(spdlog)

# glm
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)

# nlohmann/json (header-only)
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)


#add_subdirectory(engine/core)
#add_subdirectory(engine/platform)
add_subdirectory(engine/render)
#add_subdirectory(engine/ecs)
#add_subdirectory(engine/scene)
#add_subdirectory(engine/physics)
#add_subdirectory(engine/dev)
#add_subdirectory(engine/app)
#add_subdirectory(engine/content)
# aggregate interface target that bubbles up includes/links
add_library(hobgoblin INTERFACE)
target_link_libraries(hobgoblin INTERFACE
    #engine_core
    #engine_platform
    engine_render
    #engine_ecs
	#engine_scene
	#engine_physics
	#engine_dev
	#engine_app
	#engine_content
)

add_subdirectory(tools/meshpack)
#add_subdirectory(apps/goblin_slayer)


add_compile_definitions(NOMINMAX WIN32_LEAN_AND_MEAN)
