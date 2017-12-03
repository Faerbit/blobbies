cmake_minimum_required(VERSION 3.1)
project(ld40)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

file(GLOB SRC "src/*.cpp")

add_executable(ld40 ${SRC} src/Blob.cpp src/Blob.h)

set_property(TARGET ld40 PROPERTY CXX_STANDARD 14)

target_include_directories(ld40 PRIVATE
        ${GLM_INCLUDE_DIRS}
        ${GLFW_INCLUDE_DIRS}
        )
target_link_libraries(ld40
        glfw
        ${OPENGL_gl_LIBRARY}
)
