cmake_minimum_required(VERSION 3.1)
project(ld40)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(Freetype REQUIRED)

add_executable(ld40 ${SRC}
        src/main.cpp
        src/Blob.cpp src/Blob.h
        src/globals.h
        src/PlayerBlob.cpp src/PlayerBlob.h src/globals.cpp src/AiBlob.cpp src/AiBlob.h)

set_property(TARGET ld40 PROPERTY CXX_STANDARD 14)

target_include_directories(ld40 PRIVATE
        ${GLM_INCLUDE_DIRS}
        ${GLFW_INCLUDE_DIRS}
        ${FREETYPE_INCLUDE_DIR_freetype2}
)

target_link_libraries(ld40
        glfw
        ${OPENGL_gl_LIBRARY}
        ${FREETYPE_LIBRARY_RELEASE}
)

file(COPY "font/NotoSans-Black.ttf" DESTINATION ".")
