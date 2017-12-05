cmake_minimum_required(VERSION 3.1)
project(blobbies)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(Freetype REQUIRED)

add_executable(blobbies ${SRC}
        src/main.cpp
        src/Blob.cpp src/Blob.h
        src/globals.h
        src/PlayerBlob.cpp src/PlayerBlob.h src/globals.cpp src/AiBlob.cpp src/AiBlob.h)

set_property(TARGET blobbies PROPERTY CXX_STANDARD 14)

target_include_directories(blobbies PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}
        ${GLM_INCLUDE_DIRS}
        ${GLFW_INCLUDE_DIRS}
        ${FREETYPE_INCLUDE_DIR_freetype2}
)

target_link_libraries(blobbies
        glfw
        ${OPENGL_gl_LIBRARY}
        ${FREETYPE_LIBRARY_RELEASE}
)

execute_process(COMMAND xxd -i NotoSans-Black.ttf
        OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/font.h"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/font")
