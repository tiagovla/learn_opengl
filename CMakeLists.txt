cmake_minimum_required(VERSION 3.14)
project(learn_opengl)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_SHALLOW TRUE
  GIT_PRIGRESS TRUE
  GIT_TAG 3.4)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_LIBRARY_TYPE STATIC)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
  glew-cmake
  GIT_REPOSITORY https://github.com/Perlmint/glew-cmake
  GIT_TAG glew-cmake-2.2.0)
FetchContent_MakeAvailable(glew-cmake)

find_package(OpenGL REQUIRED)

add_executable(main src/main.cpp)

target_link_libraries(main PRIVATE glfw libglew_static OpenGL::GL)
